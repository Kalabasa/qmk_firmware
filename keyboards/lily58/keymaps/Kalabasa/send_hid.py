#!/usr/bin/env python3
import os
import sys
import termios
import tty
import hid

VENDOR_ID = 0x04D8
PRODUCT_ID = 0xEB2D
USAGE_PAGE = 0xFF60
USAGE = 0x61
REPORT_SIZE = 32

USER_CONFIG_SIZE = 266
SALT_SIZE = 10
SLOT_SIZE = 64
SLOT_COUNT = 4
CHECKSUM_SIZE = 4
TEXT_SIZE = SLOT_SIZE - CHECKSUM_SIZE

SPECK_KEY_WORDS = 4
SPECK_BLOCK_SIZE = 8
SPECK_ROUNDS = 27
KDF_ITERATIONS = 10000

INSTRUCTION_READ = 0
INSTRUCTION_WRITE_CIPHERTEXT = 1
INSTRUCTION_WRITE_SALT = 2

KC_A = 0x04
LETTERS = "abcdefghijklmnopqrstuvwxyz"

def send(device, payload):
    if len(payload) > REPORT_SIZE:
        sys.exit("payload too long")
    device.write(bytes([0x00]) + bytes(payload).ljust(REPORT_SIZE, b"\x00"))

def read_all(device):
    send(device, [INSTRUCTION_READ])
    data = b""
    while len(data) < USER_CONFIG_SIZE:
        data += device.read(REPORT_SIZE, timeout=1000)
    return data[:USER_CONFIG_SIZE]

def write_ciphertext(device, offset, ciphertext):
    chunk_size = REPORT_SIZE - 3
    for index in range(0, len(ciphertext), chunk_size):
        chunk = ciphertext[index:index + chunk_size]
        send(device, [INSTRUCTION_WRITE_CIPHERTEXT, offset + index, len(chunk)] + list(chunk))

def passphrase_keycodes(passphrase):
    keycodes = bytearray()
    for char in passphrase:
        if char in LETTERS:
            keycodes.append(KC_A + LETTERS.index(char))
        else:
            sys.exit("passphrase must be lowercase letters")
    return bytes(keycodes)

def derive_slot(keycodes):
    hash = 2166136261
    for keycode in keycodes:
        hash ^= keycode
        hash = (hash * 16777619) & 0xFFFFFFFF
    return hash % SLOT_COUNT

def rotate_left(value, bits):
    return ((value << bits) | (value >> (32 - bits))) & 0xFFFFFFFF

def rotate_right(value, bits):
    return ((value >> bits) | (value << (32 - bits))) & 0xFFFFFFFF

def derive_key(keycodes, salt):
    seed = []
    for word in range(SPECK_KEY_WORDS):
        hash = 2166136261 ^ word
        hash = (hash * 16777619) & 0xFFFFFFFF
        for byte in salt:
            hash ^= byte
            hash = (hash * 16777619) & 0xFFFFFFFF
        for keycode in keycodes:
            hash ^= keycode
            hash = (hash * 16777619) & 0xFFFFFFFF
        seed.append(hash)

    round_keys = expand_key(seed)
    x, y = seed[0], seed[1]
    key = []
    for iteration in range(KDF_ITERATIONS):
        previous_x, previous_y = x, y
        x, y = encrypt_block(round_keys, x, y)
        x ^= previous_x
        y ^= previous_y
        if iteration == KDF_ITERATIONS // 2 - 1:
            key = [x, y]
    return key + [x, y]

def expand_key(key):
    l = [key[1], key[2], key[3]]
    round_keys = [key[0]]
    for i in range(SPECK_ROUNDS - 1):
        l[i % 3] = ((round_keys[i] + rotate_right(l[i % 3], 8)) & 0xFFFFFFFF) ^ i
        round_keys.append(rotate_left(round_keys[i], 3) ^ l[i % 3])
    return round_keys

def encrypt_block(round_keys, x, y):
    for round_key in round_keys:
        x = ((rotate_right(x, 8) + y) & 0xFFFFFFFF) ^ round_key
        y = rotate_left(y, 3) ^ x
    return x, y

def checksum(data):
    hash = 2166136261
    for byte in data:
        hash ^= byte
        hash = (hash * 16777619) & 0xFFFFFFFF
    return hash

def encrypt(plaintext, keycodes, salt):
    text = plaintext.encode()
    if len(text) > TEXT_SIZE:
        sys.exit("plaintext too long")
    padded = text.ljust(TEXT_SIZE, b"\x00")
    padded += checksum(padded).to_bytes(4, "big")
    round_keys = expand_key(derive_key(keycodes, salt))
    ciphertext = b""
    previous_x = 0
    previous_y = 0
    for offset in range(0, SLOT_SIZE, SPECK_BLOCK_SIZE):
        block = padded[offset:offset + SPECK_BLOCK_SIZE]
        x = int.from_bytes(block[:4], "big") ^ previous_x
        y = int.from_bytes(block[4:], "big") ^ previous_y
        previous_x, previous_y = encrypt_block(round_keys, x, y)
        ciphertext += previous_x.to_bytes(4, "big") + previous_y.to_bytes(4, "big")
    return ciphertext

def masked_input(prompt):
    sys.stdout.write(prompt)
    sys.stdout.flush()
    descriptor = sys.stdin.fileno()
    previous_settings = termios.tcgetattr(descriptor)
    text = ""
    try:
        tty.setraw(descriptor)
        while True:
            char = sys.stdin.read(1)
            if char in ("\r", "\n"):
                break
            if char == "\x03":
                raise KeyboardInterrupt
            if char == "\x7f":
                if text:
                    text = text[:-1]
                    sys.stdout.write("\b \b")
            else:
                text += char
                sys.stdout.write("*")
            sys.stdout.flush()
    finally:
        termios.tcsetattr(descriptor, termios.TCSADRAIN, previous_settings)
    sys.stdout.write("\r\n")
    return text

def open_device():
    for info in hid.enumerate(VENDOR_ID, PRODUCT_ID):
        if info["usage_page"] == USAGE_PAGE and info["usage"] == USAGE:
            return hid.Device(path=info["path"])
    sys.exit("raw hid interface not found")

usage_text = "read | write_ciphertext | randomize_ciphertext | write_salt"
if len(sys.argv) < 2:
    sys.exit(usage_text)

device = open_device()
if sys.argv[1] == "read":
    print(read_all(device).hex())
elif sys.argv[1] == "randomize_ciphertext":
    write_ciphertext(device, 0, os.urandom(SLOT_SIZE * SLOT_COUNT))
elif sys.argv[1] == "write_salt":
    send(device, [INSTRUCTION_WRITE_SALT] + list(os.urandom(SALT_SIZE)))
elif sys.argv[1] == "write_ciphertext":
    salt = read_all(device)[SLOT_SIZE * SLOT_COUNT:SLOT_SIZE * SLOT_COUNT + SALT_SIZE]
    plaintext = masked_input("plaintext: ")
    while True:
        passphrase = masked_input("passphrase: ")
        keycodes = passphrase_keycodes(passphrase)
        slot = derive_slot(keycodes)
        print(f"slot: {slot}")
        answer = input("write? [y]es [n]o, change passphrase [c]ancel: ")
        if answer == "y":
            break
        if answer == "c":
            sys.exit("cancelled")
    write_ciphertext(device, slot * SLOT_SIZE, encrypt(plaintext, keycodes, salt))
else:
    sys.exit(usage_text)

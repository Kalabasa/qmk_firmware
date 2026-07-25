#pragma once
#include "quantum.h"

#define SPECK_KEY_WORDS 4
#define SPECK_BLOCK_SIZE 8
#define SPECK_ROUNDS 27

static uint32_t rotate_left(uint32_t value, uint8_t bits) {
  return (value << bits) | (value >> (32 - bits));
}

static uint32_t rotate_right(uint32_t value, uint8_t bits) {
  return (value >> bits) | (value << (32 - bits));
}

static void expand_key(const uint32_t *key, uint32_t *round_keys) {
  uint32_t l[3] = {key[1], key[2], key[3]};
  round_keys[0] = key[0];
  for (uint8_t i = 0; i < SPECK_ROUNDS - 1; i++) {
    l[i % 3] = (round_keys[i] + rotate_right(l[i % 3], 8)) ^ i;
    round_keys[i + 1] = rotate_left(round_keys[i], 3) ^ l[i % 3];
  }
}

static uint32_t load_word(const uint8_t *bytes) {
  return ((uint32_t)bytes[0] << 24) | ((uint32_t)bytes[1] << 16) |
         ((uint32_t)bytes[2] << 8) | bytes[3];
}

static void store_word(uint32_t word, uint8_t *bytes) {
  bytes[0] = word >> 24;
  bytes[1] = word >> 16;
  bytes[2] = word >> 8;
  bytes[3] = word;
}

static void encrypt_block(const uint32_t *round_keys, uint32_t *x, uint32_t *y) {
  for (uint8_t round = 0; round < SPECK_ROUNDS; round++) {
    *x = ((rotate_right(*x, 8) + *y) ^ round_keys[round]);
    *y = rotate_left(*y, 3) ^ *x;
  }
}

static void decrypt_block(const uint32_t *round_keys, uint32_t *x, uint32_t *y) {
  for (int8_t round = SPECK_ROUNDS - 1; round >= 0; round--) {
    *y = rotate_right(*x ^ *y, 3);
    *x = rotate_left((*x ^ round_keys[round]) - *y, 8);
  }
}

static void speck_decrypt_cbc(const uint32_t *key, const uint8_t *ciphertext, uint8_t *plaintext, uint8_t length) {
  uint32_t round_keys[SPECK_ROUNDS];
  expand_key(key, round_keys);
  uint32_t previous_x = 0;
  uint32_t previous_y = 0;
  for (uint8_t offset = 0; offset < length; offset += SPECK_BLOCK_SIZE) {
    uint32_t x = load_word(ciphertext + offset);
    uint32_t y = load_word(ciphertext + offset + 4);
    uint32_t cipher_x = x;
    uint32_t cipher_y = y;
    decrypt_block(round_keys, &x, &y);
    store_word(x ^ previous_x, plaintext + offset);
    store_word(y ^ previous_y, plaintext + offset + 4);
    previous_x = cipher_x;
    previous_y = cipher_y;
  }
}

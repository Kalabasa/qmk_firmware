#include "passwd.h"

#include "speck.h"

#define PASSPHRASE_MAX 40
#define KDF_ITERATIONS 10000

typedef enum { OFF, REPLAY } passwd_state_t;

static passwd_state_t state = OFF;
static uint8_t passphrase[PASSPHRASE_MAX];
static uint8_t passphrase_len = 0;

static void derive_key(const uint8_t *typed, uint8_t len, const uint8_t *salt, uint32_t *key) {
  uint32_t seed[SPECK_KEY_WORDS];
  for (uint8_t word = 0; word < SPECK_KEY_WORDS; word++) {
    uint32_t hash = 2166136261u;
    hash ^= word;
    hash *= 16777619u;
    for (uint8_t i = 0; i < PASSWD_SALT_SIZE; i++) {
      hash ^= salt[i];
      hash *= 16777619u;
    }
    for (uint8_t i = 0; i < len; i++) {
      hash ^= typed[i];
      hash *= 16777619u;
    }
    seed[word] = hash;
  }

  uint32_t round_keys[SPECK_ROUNDS];
  expand_key(seed, round_keys);
  uint32_t x = seed[0];
  uint32_t y = seed[1];
  for (uint16_t iteration = 0; iteration < KDF_ITERATIONS; iteration++) {
    uint32_t previous_x = x;
    uint32_t previous_y = y;
    encrypt_block(round_keys, &x, &y);
    x ^= previous_x;
    y ^= previous_y;
    if (iteration == KDF_ITERATIONS / 2 - 1) {
      key[0] = x;
      key[1] = y;
    }
  }
  key[2] = x;
  key[3] = y;
}

static uint8_t derive_slot(const uint8_t *typed, uint8_t len) {
  uint32_t hash = 2166136261u;
  for (uint8_t i = 0; i < len; i++) {
    hash ^= typed[i];
    hash *= 16777619u;
  }
  return hash % PASSWD_SLOT_COUNT;
}

static uint32_t checksum(const uint8_t *bytes, uint8_t len) {
  uint32_t hash = 2166136261u;
  for (uint8_t i = 0; i < len; i++) {
    hash ^= bytes[i];
    hash *= 16777619u;
  }
  return hash;
}

static void replay_password(uint8_t *ciphertext, const uint8_t *salt) {
  uint32_t key[SPECK_KEY_WORDS];
  char plaintext[PASSWD_TEXT_SIZE + 1];
  uint8_t slot = derive_slot(passphrase, passphrase_len);
  derive_key(passphrase, passphrase_len, salt, key);

  uint8_t decrypted[PASSWD_SLOT_SIZE];
  speck_decrypt_cbc(key, ciphertext + slot * PASSWD_SLOT_SIZE, decrypted, PASSWD_SLOT_SIZE);
  if (load_word(decrypted + PASSWD_TEXT_SIZE) != checksum(decrypted, PASSWD_TEXT_SIZE)) return;

  memcpy(plaintext, decrypted, PASSWD_TEXT_SIZE);
  plaintext[PASSWD_TEXT_SIZE] = '\0';
  send_string(plaintext);
}

static void reset_passwd_state(void) {
  state = OFF;
  passphrase_len = 0;
  memset(passphrase, 0, PASSPHRASE_MAX);
}

bool process_passwd(uint16_t keycode, keyrecord_t *record, uint8_t *ciphertext, const uint8_t *salt) {
  if (keycode == passwd_toggle_key) {
    if (!record->event.pressed) {
      if (state == OFF) {
        state = REPLAY;
        passphrase_len = 0;
      } else {
        reset_passwd_state();
      }
    }
    return true;
  }

  if (state == REPLAY) {
    if (record->event.pressed) {
      if (keycode == KC_ENTER) {
        replay_password(ciphertext, salt);
        reset_passwd_state();
      } else if (passphrase_len < PASSPHRASE_MAX) {
        passphrase[passphrase_len++] = keycode;
      }
      return false;
    }
  }

  return true;
}

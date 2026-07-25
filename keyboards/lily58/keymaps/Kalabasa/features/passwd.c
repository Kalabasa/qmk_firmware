#include "passwd.h"

#define PASSPHRASE_MAX 20

typedef enum { OFF, REPLAY } passwd_state_t;

static passwd_state_t state = OFF;
static uint8_t passphrase[PASSPHRASE_MAX];
static uint8_t passphrase_len = 0;

static void derive_key(const uint8_t *typed, uint8_t len, uint8_t *key) {
  // TODO https://github.com/BareRose/lonesha256/blob/master/lonesha256.h
  memset(key, 0, PASSWD_SLOT_SIZE);
  memcpy(key, typed, len);
}

static uint8_t derive_slot(const uint8_t *typed, uint8_t len) {
  uint32_t hash = 2166136261u;
  for (uint8_t i = 0; i < len; i++) {
    hash ^= typed[i];
    hash *= 16777619u;
  }
  return hash % PASSWD_SLOT_COUNT;
}

static void replay_password(uint8_t *cipher) {
  uint8_t key[PASSWD_SLOT_SIZE];
  uint8_t slot = derive_slot(passphrase, passphrase_len);
  derive_key(passphrase, passphrase_len, key);
  for (uint8_t i = 0; i < PASSWD_SLOT_SIZE; i++) {
    uint8_t code = cipher[i + slot * PASSWD_SLOT_SIZE] ^ key[i];                                                                                                    
    if (code == KC_NO) break;                                                                                                             
    tap_code(code);
  }
}

static void reset_passwd_state(void) {
  state = OFF;
  passphrase_len = 0;
  memset(passphrase, 0, PASSPHRASE_MAX);
}

bool process_passwd(uint16_t keycode, keyrecord_t *record, uint8_t *cipher) {
  if (keycode == passwd_toggle_key) {
    if (!record->event.pressed) {
      if (state == OFF) {
        state = REPLAY;
        passphrase_len = 0;
      } else {
        reset_passwd_state();
      }
    }
    return false;
  }

  if (state != REPLAY) return true;

  if (record->event.pressed) {
    if (keycode == KC_ENTER) {
      replay_password(cipher);
      reset_passwd_state();
    } else if (passphrase_len < PASSPHRASE_MAX) {
      passphrase[passphrase_len++] = keycode;
    }
  }

  return false;
}

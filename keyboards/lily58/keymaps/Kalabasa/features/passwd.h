#pragma once
#include "quantum.h"

#define PASSWD_SLOT_SIZE 64
#define PASSWD_SLOT_COUNT 4
#define PASSWD_CHECKSUM_SIZE 4
#define PASSWD_TEXT_SIZE (PASSWD_SLOT_SIZE - PASSWD_CHECKSUM_SIZE)
#define PASSWD_SALT_SIZE 10
#define PASSWD_CIPHERTEXT_SIZE (PASSWD_SLOT_SIZE * PASSWD_SLOT_COUNT)

extern const uint16_t passwd_toggle_key;

bool process_passwd(uint16_t keycode, keyrecord_t *record, uint8_t *data, const uint8_t *salt);

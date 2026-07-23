#pragma once
#include "quantum.h"

#define PASSWD_SLOT_SIZE 32
#define PASSWD_SLOT_COUNT 4

extern const uint16_t passwd_toggle_key;

bool process_passwd(uint16_t keycode, keyrecord_t *record, char *data);

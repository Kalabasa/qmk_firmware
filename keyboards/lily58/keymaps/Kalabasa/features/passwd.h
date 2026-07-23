#pragma once
#include "quantum.h"

extern const uint16_t passwd_toggle_key;
extern uint8_t PASSWD_SLOT_SIZE;
extern uint8_t PASSWD_SLOT_COUNT;

bool process_passwd(uint16_t keycode, keyrecord_t *record, char *data);

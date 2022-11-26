#include "bitwise_f.h"

static uint8_t pressed_keys = 0;
static uint8_t accumulator = 0;

uint8_t F_KEYS[] = {
  KC_F1,
  KC_F2,
  KC_F3,
  KC_F4,
  KC_F5,
  KC_F6,
  KC_F7,
  KC_F8,
  KC_F9,
  KC_F10,
  KC_F11,
  KC_F12,
  KC_F13,
  KC_F14,
  KC_F15,
};

bool process_bitwise_f(uint16_t keycode, keyrecord_t *record) {
  int index = -1;
  for (int i = 0; i < NUM_BITWISE_F_KEYS; ++i) {
    if (keycode == bitwise_f_keys[i]) {
      index = i;
      break;
    }
  }

  if (index == -1) {
    return true;
  }

  if (record->event.pressed) {
    pressed_keys++;
    accumulator |= (1 << index);
  } else {
    pressed_keys--;
    if (pressed_keys <= 0) {
      pressed_keys = 0;
      tap_code(F_KEYS[accumulator - 1]);
      accumulator = 0;
    }
  }

  return false;
}

#include "baybayin.h"

bool process_baybayin(uint16_t keycode, keyrecord_t *record) {
  int base_key = keycode & 0xFF;
  if (base_key == KC_A) {
    if (!record->event.pressed) {
      send_unicode_string("ᜀ");
    }
    return false;
  }
  return true;
}
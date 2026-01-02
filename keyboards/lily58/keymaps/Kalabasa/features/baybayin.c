#include "baybayin.h"

const char* KUDLIT_I = "ᜒ";
const char* KUDLIT_U = "ᜓ";
const char* KUDLIT_KRUS = "᜔";
const char* PAMUDPOD = "᜕";

char* get_base_baybayin(uint16_t keycode) {
  switch (keycode) {
    case KC_K:
      return "ᜃ";
    case KC_G:
      return "ᜄ";
    case KC_T:
      return "ᜆ";
    case KC_D:
    case KC_R:
      return "ᜇ";
    case KC_N:
      return "ᜈ";
    case KC_P:
      return "ᜉ";
    case KC_B:
      return "ᜊ";
    case KC_M:
      return "ᜋ";
    case KC_Y:
      return "ᜌ";
    case KC_L:
      return "ᜎ";
    case KC_W:
      return "ᜏ";
    case KC_S:
      return "ᜐ";
    case KC_H:
      return "ᜑ";
    case KC_A:
      return "ᜀ";
    case KC_I:
    case KC_E:
      return "ᜁ";
    case KC_U:
    case KC_O:
      return "ᜂ";
    default:
      return NULL;
  }
}

bool is_consonant(uint16_t keycode) {
  return keycode == KC_K
    || keycode == KC_G
    || keycode == KC_T
    || keycode == KC_D
    || keycode == KC_R
    || keycode == KC_N
    || keycode == KC_P
    || keycode == KC_B
    || keycode == KC_M
    || keycode == KC_Y
    || keycode == KC_L
    || keycode == KC_W
    || keycode == KC_S
    || keycode == KC_H;
}

static uint16_t curr_keycode = 0;
static uint16_t prev_keycode = 0;
static uint16_t prev_key_timer;

bool process_baybayin(uint16_t keycode, keyrecord_t *record) {
  curr_keycode = keycode & 0xFF;

  if (!curr_keycode || get_mods()) {
    prev_keycode = 0;
    return true;
  }

  char* baybayin = get_base_baybayin(curr_keycode);
  if (!baybayin) {
    prev_keycode = 0;
    return true;
  }

  if (prev_keycode && timer_elapsed(prev_key_timer) > 3000) {
    prev_keycode = 0;
  }

  if (record->event.pressed) {
    if (is_consonant(curr_keycode)) {
      if (prev_keycode == KC_N && curr_keycode == KC_G) {
        tap_code(KC_BACKSPACE); // delete virama
        tap_code(KC_BACKSPACE); // delete 'ᜈ'
        send_unicode_string("ᜅ");
        send_unicode_string(PAMUDPOD);
      } else {
        send_unicode_string(baybayin);
        send_unicode_string(PAMUDPOD);
      }
    } else {
      if (is_consonant(prev_keycode)) {
        if (curr_keycode == KC_A) {
          tap_code(KC_BACKSPACE); // delete virama
        } else if (curr_keycode == KC_I || curr_keycode == KC_E) {
          tap_code(KC_BACKSPACE); // delete virama
          send_unicode_string(KUDLIT_I);
        } else if (curr_keycode == KC_U || curr_keycode == KC_O) {
          tap_code(KC_BACKSPACE); // delete virama
          send_unicode_string(KUDLIT_U);
        }
      } else {
        send_unicode_string(baybayin);
      }
    }
    prev_keycode = curr_keycode;
    prev_key_timer = timer_read();
  }
  return false;
}
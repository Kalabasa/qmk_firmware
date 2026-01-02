#include "baybayin.h"

const char* KUDLIT_I = "ᜒ";
const char* KUDLIT_U = "ᜓ";
const char* KUDLIT_KRUS = "᜔";
const char* PAMUDPOD = "᜕";

// A-Z mapped to baybayin in UTF-8.
// Since all chars are in range U+1700..U+171F, the UTF-8 encoding of each char is always 3 bytes.
// Letters with no equivalent use 3 null bytes.
// Thus, this can be indexed in multiples of 3.
char* CHARS = "ᜀᜊ\0\0\0ᜇᜁ\0\0\0ᜄᜑᜁ\0\0\0ᜃᜎᜋᜈᜂᜉ\0\0\0ᜇᜐᜆᜂ\0\0\0ᜏ\0\0\0ᜌ\0\0\0";

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
  static char buf[4] = "\0\0\0\0";
  curr_keycode = QK_MODS_GET_BASIC_KEYCODE(keycode);

  if (
    !(curr_keycode >= KC_A && curr_keycode <= KC_Z)
    || get_mods()
  ) {
    if (record->event.pressed) {
      prev_keycode = 0;
    }
    return true;
  }

  char* baybayin_ptr = CHARS + (curr_keycode - KC_A) * 3;
  if (!*baybayin_ptr) {
    return false;
  }

  if (prev_keycode && timer_elapsed(prev_key_timer) > 2000) {
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
        strncpy(buf, baybayin_ptr, 3);
        send_unicode_string(buf);
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
        strncpy(buf, baybayin_ptr, 3);
        send_unicode_string(buf);
      }
    }
    prev_keycode = curr_keycode;
    prev_key_timer = timer_read();
  }
  return false;
}
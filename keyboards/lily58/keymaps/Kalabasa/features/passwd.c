#include "passwd.h"

typedef enum { OFF, CAPTURE, REPLAY } passwd_state_t;

typedef struct {
  char letters[32];
  uint8_t shift_bits[4];
} passwd_slot_t;

static passwd_slot_t buffer;
static uint8_t buffer_letter_count = 0;

bool process_passwd(uint16_t keycode, keyrecord_t *record, void *data) {
  static passwd_state_t state = OFF;

  if (keycode == passwd_toggle_key) {
    if (record->event.pressed) {
      state = (state + 1) % 3;
    }
    return false;
  }

  switch (state) {
    case OFF:
      return true;
    case CAPTURE:
    case REPLAY:
      if (record->event.pressed) {
        if (keycode == KC_ESCAPE || keycode == KC_ENTER || buffer_letter_count >= sizeof(buffer.letters) - 1) {
          buffer_letter_count = 0;
          state = OFF;
        } else {
        }
      }
      return false;
  }

  return true;
}

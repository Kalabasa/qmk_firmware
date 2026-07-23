#include "passwd.h"

typedef enum { OFF, CAPTURE, REPLAY } passwd_state_t;

bool process_passwd(uint16_t keycode, keyrecord_t *record, uint16_t toggle_key, char *data, uint8_t slot_size, uint8_t slot_count) {
  static passwd_state_t state = OFF;

  if (keycode == toggle_key) {
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
      if (keycode == KC_ESCAPE) {
        state = OFF;
      } else if (keycode == KC_ENTER) {
        state = OFF; 
      }
      return false;
  }
}

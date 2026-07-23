#include "passwd.h"

typedef enum { OFF, CAPTURE, REPLAY } passwd_state_t;

static char buffer[PASSWD_SLOT_SIZE];
static uint8_t buffer_len = 0;

bool process_passwd(uint16_t keycode, keyrecord_t *record, char *data) {
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
      if (keycode == KC_ESCAPE) {
        state = OFF;
      } else if (keycode == KC_ENTER) {
        state = OFF; 
      }
      return false;
  }

  return true;
}

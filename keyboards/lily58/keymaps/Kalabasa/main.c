#include QMK_KEYBOARD_H
#include "progmem.h"

const key_override_t comma_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_COMMA, KC_COMMA);
const key_override_t bslash_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSLASH, KC_BSLASH);

const key_override_t **key_overrides = (const key_override_t *[]){
    &comma_key_override,
    &bslash_key_override,
    NULL,
};

layer_state_t layer_state_set_user(layer_state_t state) {
  if (IS_LAYER_ON_STATE(state, 1)) {
    register_code(KC_LSHIFT);
  } else {
    unregister_code(KC_LSHIFT);
  }
  return state;
}


#ifdef OLED_ENABLE

static char layer_ind_frame[] = {
  0x80, 0x81, 0x82, 0x83,
  0xA0, 0x20, 0x20, 0xA1,
  0xA2, 0x20, 0x20, 0xA3,
  0xC0, 0xC1, 0xC2, 0xC3,
};

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  return OLED_ROTATION_270;
}

void oled_task_user(void) {
  oled_write("Hello World!", false);
  // oled_set_cursor_px(2, 4);
  oled_write_raw(layer_ind_frame, 4 * sizeof(char));
  // oled_write_raw_P(layer_ind_frame, 4 * sizeof(char));
  // oled_set_cursor_px(2, 5);
  // oled_write_raw_P(&layer_ind_frame[4], 4 * sizeof(char));
  // oled_set_cursor_px(2, 6);
  // oled_write_raw_P(&layer_ind_frame[8], 4 * sizeof(char));
  // oled_set_cursor_px(2, 7);
  // oled_write_raw_P(&layer_ind_frame[1], 4 * sizeof(char));
}

#endif // OLED_ENABLE

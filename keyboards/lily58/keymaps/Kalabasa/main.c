#include QMK_KEYBOARD_H
#include "progmem.h"

const key_override_t comma_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_COMMA, KC_COMMA);
const key_override_t bslash_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSLASH, KC_BSLASH);

const key_override_t **key_overrides = (const key_override_t *[]){
  &comma_key_override,
  &bslash_key_override,
  NULL,
};

void update_layer_ind(unsigned int layer);
void update_mode_ind(unsigned int layer);

layer_state_t layer_state_set_user(layer_state_t state) {
  if (IS_LAYER_ON_STATE(state, 1)) {
    register_code(KC_LSHIFT);
  } else {
    unregister_code(KC_LSHIFT);
  }

  unsigned int layer = get_highest_layer(state);
  update_layer_ind(layer);
  update_mode_ind(layer);

  return state;
}

bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LT(1, KC_SPC):
      return true;
    default:
      return false;
  }
}


typedef enum {
  HIDE = 0,
  SHOW_BOTH,
  SHOW_LEFT,
  SHOW_RIGHT,
} layer_ind_state_t;

static layer_ind_state_t layer_ind_state = HIDE;
static char layer_ind[] = {
  0x80, 0x81, 0x82, 0x83, 0,
  0xA0, 0x20, 0x20, 0xA1, 0,
  0xA2, 0x20, 0x20, 0xA3, 0,
  0xC0, 0xC1, 0xC2, 0xC3, 0,
};

static bool mode_ind_state = false;
static char mode_ind[] = {
  0x90, 0x91, 0x92, 0x93, 0,
  0xB0, 0x20, 0x20, 0xB1, 0,
  0xB2, 0x20, 0x20, 0xB3, 0,
  0xD0, 0xD1, 0xD2, 0xD3, 0,
};

void update_layer_ind(unsigned int layer) {
  switch (layer) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      layer_ind_state = layer <= 3 ? SHOW_LEFT : SHOW_RIGHT;
      int offset = 2 * (layer - 1);
      layer_ind[6] = 0x84 + offset;
      layer_ind[7] = 0x85 + offset;
      layer_ind[11] = 0xA4 + offset;
      layer_ind[12] = 0xA5 + offset;
      break;
    case 9:
      layer_ind_state = SHOW_BOTH;
      layer_ind[6] = 0x96;
      layer_ind[7] = 0x97;
      layer_ind[11] = 0xB4;
      layer_ind[12] = 0xB5;
      break;
    default:
      layer_ind_state = HIDE;
  }
}

void update_mode_ind(unsigned int layer) {
  switch (layer) {
    case 8:
      mode_ind_state = true;
      mode_ind[6] = 0x94;
      mode_ind[7] = 0x95;
      mode_ind[11] = 0xB4;
      mode_ind[12] = 0xB5;
      break;
    default:
      mode_ind_state = false;
  }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  return OLED_ROTATION_270;
}

void render_indicator(char* data) {
  oled_set_cursor(0, 6);
  oled_write_ln(data, false);
  oled_write_ln(data + 5, false);
  oled_write_ln(data + 10, false);
  oled_write_ln(data + 15, false);
}

void render_modifiers(unsigned int layer) {
  if (host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK)) {
    oled_set_cursor_px(0, 0);
    oled_write_char(0xc4, false);
  }

  unsigned int mods = get_mods();
  if (mods & MOD_MASK_GUI) {
    oled_set_cursor_px(4, 15);
    oled_write_char(0xc5, false);
  }
  if (mods & MOD_MASK_ALT) {
    oled_set_cursor_px(11, 15);
    oled_write_char(0xc6, false);
  }
  if (mods & MOD_MASK_CTRL) {
    oled_set_cursor_px(17, 15);
    oled_write_char(0xc7, false);
  }
  if (layer != 1 && mods & MOD_MASK_SHIFT) {
    oled_set_cursor_px(24, 15);
    oled_write_char(0xc8, false);
  }
}

void oled_task_user(void) {
  oled_clear();

  unsigned int layer = get_highest_layer(layer_state);

  if (!is_keyboard_master()) {
    update_layer_ind(layer);
    update_mode_ind(layer);
  }

  bool left = is_keyboard_left();
  bool show_left = layer_ind_state == SHOW_LEFT;
  if (layer_ind_state && (layer_ind_state == SHOW_BOTH || left == show_left)) {
    render_indicator(layer_ind);
  } else if (mode_ind_state) {
    render_indicator(mode_ind);
  }

  render_modifiers(layer);
}

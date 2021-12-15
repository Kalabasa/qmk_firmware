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

typedef enum {
  HIDE = 0,
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
      layer_ind_state = layer <= 4 ? SHOW_LEFT : SHOW_RIGHT;
      int offset = 2 * (layer - 1);
      layer_ind[6] = 0x84 + offset;
      layer_ind[7] = 0x85 + offset;
      layer_ind[11] = 0xA4 + offset;
      layer_ind[12] = 0xA5 + offset;
      break;
    default:
      layer_ind_state = HIDE;
  }
}

void update_mode_ind(unsigned int layer) {
  switch (layer) {
    case 8:
      mode_ind_state = true;
      int offset = 2 * (layer - 8);
      mode_ind[6] = 0x94 + offset;
      mode_ind[7] = 0x95 + offset;
      mode_ind[11] = 0xB4 + offset;
      mode_ind[12] = 0xB5 + offset;
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

void oled_task_user(void) {
  if (!is_keyboard_master()) {
    unsigned int layer = get_highest_layer(layer_state);
    update_layer_ind(layer);
    update_mode_ind(layer);
  }
  
  bool left = is_keyboard_left();
  bool show_left = layer_ind_state == SHOW_LEFT;
  if (layer_ind_state && left == show_left) {
    render_indicator(layer_ind);
  } else if (mode_ind_state) {
    render_indicator(mode_ind);
  } else {
    oled_clear();
  }
}

#include QMK_KEYBOARD_H
#include "keycodes.h"
#include "progmem.h"
#include "features/custom_shift_keys.h"

const uint32_t PROGMEM unicode_map[] = {
  [EMO_Q] = U'😭',
  [EMO_W] = U'👋',
  [EMO_F] = U'🥺',
  [EMO_P] = U'🎉',
  [EMO_A] = U'😂',
  [EMO_R] = U'😃',
  [EMO_S] = U'🙂',
  [EMO_T] = U'🤔',
  [UP_HAND] = U'☝',
  [DOWN_HAND] = U'👇',
  [LEFT_HAND] = U'👈',
  [RIGHT_HAND] = U'👉',
  [UP] = U'↑',
  [DOWN] = U'↓',
  [LEFT] = U'←',
  [RIGHT] = U'→',
  [UP_BOX] = 0x2b06, // fixme
  [DOWN_BOX] = 0x2b07,
  [LEFT_BOX] = 0x2b05,
  [RIGHT_BOX] = 0x27a1,
  [CROSS] = U'❌',
  [LIKE] = U'👍',
  [CHECK] = U'✔',
  [CHECK_BOX] = U'✅',
};

typedef enum {
  OS_LINUX = 0,
  OS_MACOS,
  // OS_WINDOWS,
} os_t;

// For OS-aware shortcuts
static os_t os = OS_LINUX;

// 0: Neutral, 1: Open bracket held, 2: Open & close brackets held
static int bracket_state = 0;

// For unshifted keys in layer 1
const custom_shift_key_t custom_shift_keys[] = {
  {KC_SEMICOLON, KC_SEMICOLON},
  {KC_SLASH, KC_SLASH},
};
uint8_t NUM_CUSTOM_SHIFT_KEYS =
    sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);

uint16_t get_primary_mod(void);
uint16_t get_desktop_mod(void);
uint16_t get_word_mod(void);
uint16_t get_emoji_picker_hotkey(void);
void (*get_record_func(keyrecord_t *record))(uint16_t);
void update_layer_ind(unsigned int layer);
void update_mode_ind(unsigned int layer);

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (!process_custom_shift_keys(keycode, record)) return false;

  void (*record_func)(uint16_t) = get_record_func(record);

  switch (keycode) {
    case LCG_NRM: // Ctrl as primary modifier (Linux/Windows)
      if (record->event.pressed) {
        os = OS_LINUX;
        set_unicode_input_mode(UC_LNX);
      }
      return true;
    case LCG_SWP: // GUI as primary modifier (macOS)
      if (record->event.pressed) {
        os = OS_MACOS;
        set_unicode_input_mode(UC_MAC);
      }
      return true;

    /*
    Some shortcuts
    */
    case BRACKET_BACK:
      record_func(get_primary_mod() | KC_LEFT_BRACKET);
      return false;
    case BRACKET_FORWARD:
      record_func(get_primary_mod() | KC_RIGHT_BRACKET);
      return false;
    case DESKTOP_UP:
      record_func(get_desktop_mod() | KC_UP);
      return false;
    case DESKTOP_DOWN:
      record_func(get_desktop_mod() | KC_DOWN);
      return false;
    case DESKTOP_LEFT:
      record_func(get_desktop_mod() | KC_LEFT);
      return false;
    case DESKTOP_RIGHT:
      record_func(get_desktop_mod() | KC_RIGHT);
      return false;
    case WORD_NEXT:
      record_func(get_word_mod() | KC_RIGHT);
      return false;
    case WORD_PREV:
      record_func(get_word_mod() | KC_LEFT);
      return false;
    case EMOJI:
      if (!record->event.pressed) {
        tap_code16(get_emoji_picker_hotkey());
      }
      return false;

    /*
    Word actions
    */
    case WORD_SELECT:
    case WORD_DELETE:
      if (record->event.pressed) {
        uint16_t word_mod = get_word_mod();
        unregister_code(KC_LSHIFT);
        unregister_code(KC_RSHIFT);
        tap_code(KC_LEFT);
        tap_code16(word_mod | KC_RIGHT);
        tap_code16(word_mod | S(KC_LEFT));
        if (keycode == WORD_DELETE) {
          tap_code(KC_BACKSPACE);
        }
      }
      return false;

    /*
    Put cursor inside after typing empty pair of brackets
    FSM:
      (0)  → [ down →→→  (1)  → ] down →→→  (2)  → ] up →→→  ((KC_LEFT))
       ↑                  ↓ [ up             ↓ [ ] up
       ↑←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←

    */
    case KC_LEFT_PAREN:
    case KC_LEFT_BRACKET:
    case KC_LEFT_CURLY_BRACE:
    case KC_LEFT_ANGLE_BRACKET:
      if (record->event.pressed) {
        bracket_state = 1;
      } else if (!record->event.pressed) {
        bracket_state = 0;
      }
      return true;
    case KC_RIGHT_PAREN:
    case KC_RIGHT_BRACKET:
    case KC_RIGHT_CURLY_BRACE:
    case KC_RIGHT_ANGLE_BRACKET:
      if (record->event.pressed) {
        if (bracket_state == 1) bracket_state++;
      } else if (!record->event.pressed) {
        if (bracket_state == 2) {
          bracket_state = 0;
          unregister_code16(keycode);
          tap_code(KC_LEFT);
          return false;
        }
      }
      return true;

    default:
      return true;
  }
}

layer_state_t layer_state_set_user(layer_state_t state) {
  if (IS_LAYER_ON_STATE(state, 1)) { // Shift layer
    register_code(KC_LSHIFT);
  } else {
    unregister_code(KC_LSHIFT);
  }

  swap_hands = IS_LAYER_ON_STATE(state, 9); // Swap QWERTY

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

uint16_t get_primary_mod(void) {
  switch (os) {
    case OS_LINUX: return QK_LCTL;
    case OS_MACOS: return QK_LGUI;
  }
  return 0;
}

uint16_t get_desktop_mod(void) {
  switch (os) {
    case OS_LINUX: return QK_LCTL | QK_LGUI;
    case OS_MACOS: return QK_LCTL;
  }
  return 0;
}

uint16_t get_word_mod(void) {
  switch (os) {
    case OS_LINUX: return QK_LCTL;
    case OS_MACOS: return QK_LALT;
  }
  return 0;
}

uint16_t get_emoji_picker_hotkey(void) {
  switch (os) {
    case OS_LINUX: return G(KC_SEMICOLON);
    case OS_MACOS: return C(G(KC_SPACE));
  }
  return 0;
}

void (*get_record_func(keyrecord_t *record))(uint16_t) {
  if (record->event.pressed) {
    return &register_code16;
  } else {
    return &unregister_code16;
  }
}



//=============================================================================
//
//  LED Rendering
//
//=============================================================================

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
      layer_ind_state = layer <= 2 || layer == 6 ? SHOW_LEFT : SHOW_RIGHT;
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
      layer_ind[11] = 0xB6;
      layer_ind[12] = 0xB7;
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

void render_os(void) {
  oled_set_cursor(2 + os, 0);
  oled_write_char(0xd4 + os, false);
}

void render_indicator(char* data) {
  oled_set_cursor(1, 6);
  oled_write(data, false);
  oled_set_cursor(1, 7);
  oled_write_ln(data + 5, false);
  oled_set_cursor(1, 8);
  oled_write_ln(data + 10, false);
  oled_set_cursor(1, 9);
  oled_write_ln(data + 15, false);
}

void render_modifiers(unsigned int layer) {
  if (host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK)) {
    oled_set_cursor_px(0, 0);
    oled_write_char(0xc4, false);
  }

  unsigned int mods = get_mods();
  const int dx = is_keyboard_left() ? -7 : 7;
  int x = is_keyboard_left() ? (4 - dx*3) : 4;
  if (mods & MOD_MASK_CTRL) {
    oled_set_cursor_px(x, 15);
    oled_write_char(0xc7, false);
  }
  x += dx;
  if (mods & MOD_MASK_ALT) {
    oled_set_cursor_px(x, 15);
    oled_write_char(0xc6, false);
  }
  x += dx;
  if (mods & MOD_MASK_GUI) {
    oled_set_cursor_px(x, 15);
    oled_write_char(0xc5, false);
  }
  x += dx;
  if (layer != 1 && mods & MOD_MASK_SHIFT) {
    oled_set_cursor_px(x, 15);
    oled_write_char(0xc8, false);
  }
}

bool oled_task_user(void) {
  oled_clear();

  if (is_keyboard_master()) render_os();

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

  return false;
}

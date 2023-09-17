#include QMK_KEYBOARD_H
#include "keycodes.h"
#include "progmem.h"
#include "features/custom_shift_keys.h"
#include "features/layer_lock.h"
#include "features/bitwise_f.h"

#define LAYER_BASE 0
#define LAYER_SHIFT 1
#define LAYER_EMOJI 6
#define LAYER_QWERTY 8

extern keymap_config_t keymap_config;

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
  [LS_QUOTE] = U'‘',
  [RS_QUOTE] = U'’',
  [LD_QUOTE] = U'“',
  [RD_QUOTE] = U'”',
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

// 0: Inactive, 1: GAME_CHAT key down, 2: Active
static int game_chat_state = 0;

// For unshifted keys in layer 1
const custom_shift_key_t custom_shift_keys[] = {
  {KC_SEMICOLON, KC_SEMICOLON},
  {KC_GRAVE, KC_GRAVE},
  {KC_BACKSLASH, KC_BACKSLASH},
};
uint8_t NUM_CUSTOM_SHIFT_KEYS = sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);

// Bitwise FXX input keys
const uint16_t bitwise_f_keys[] = { F_B1, F_B2, F_B3, F_B4 };
uint8_t NUM_BITWISE_F_KEYS = sizeof(bitwise_f_keys) / sizeof(uint16_t);


static int led_timer = 0;

uint16_t get_primary_mod(void);
uint16_t get_desktop_mod(void);
uint16_t get_word_mod(void);
uint16_t get_emoji_picker_hotkey(void);
void (*get_record_func(keyrecord_t *record))(uint16_t);
void update_layer_ind(unsigned int layer);
void update_mode_ind(unsigned int layer);

void keyboard_post_init_user(void) {
  os = keymap_config.swap_lctl_lgui ? OS_MACOS : OS_LINUX;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (!process_layer_lock(keycode, record, LAYER_LOCK)) return false;
  if (!process_custom_shift_keys(keycode, record)) return false;
  if (!process_bitwise_f(keycode, record)) return false;

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
    Line actions
    */
    case KC_HOME:
      if (os == OS_MACOS) {
        // Command + Left moves cursor to beginning of line
        record_func(QK_LGUI | KC_LEFT);
        return false;
      } else {
        return true;
      }
    case KC_END:
      if (os == OS_MACOS) {
        // Command + Right moves cursor to end of line
        record_func(QK_LGUI | KC_RIGHT);
        return false;
      } else {
        return true;
      }

    /*
    Put cursor inside after typing empty pair of brackets
    bracket_state machine:
      (0) -- '[' down --> (1) -- ']' down --> (2) -- ']' up --> ((KC_LEFT))
       ^                   |                   |
       |                   | '[' up            | '[' or ']' up
       '---------------------------------------'
    */
    case KC_LEFT_PAREN:
    case KC_LEFT_BRACKET:
    case KC_LEFT_CURLY_BRACE:
    case KC_LEFT_ANGLE_BRACKET:
      if (record->event.pressed) {
        bracket_state = 1;
      } else {
        bracket_state = 0;
      }
      return true;
    case KC_RIGHT_PAREN:
    case KC_RIGHT_BRACKET:
    case KC_RIGHT_CURLY_BRACE:
    case KC_RIGHT_ANGLE_BRACKET:
      if (record->event.pressed) {
        if (bracket_state == 1) bracket_state++;
      } else {
        if (bracket_state == 2) {
          bracket_state = 0;
          unregister_code16(keycode);
          tap_code(KC_LEFT);
          return false;
        }
      }
      return true;

    /*
    Temporarily activate normal typing on GAME_CHAT + ENTER.
    Return to QWERTY after second ENTER or an ESCAPE.
    game_chat_state machine:
      (0) -- GC --> (1) -- ENT --> (2)
       ^             | GC up        | ENT/ESC
       '----------------------------'
    */
    case GAME_CHAT:
      if (record->event.pressed) {
        if (game_chat_state == 0) game_chat_state++;
      } else {
        if (game_chat_state == 1) game_chat_state--;
      }
      return false;
    case KC_ENTER:
      if (!record->event.pressed) {
        if (game_chat_state == 1) {
          game_chat_state++;
          layer_move(LAYER_BASE);
        } else if (game_chat_state == 2) {
          game_chat_state = 0;
          layer_move(LAYER_QWERTY);
        }
      }
      return true;
    case KC_ESCAPE:
      if (!record->event.pressed && game_chat_state == 2) {
        game_chat_state = 0;
        layer_move(LAYER_QWERTY);
      }
      return true;

    default:
      return true;
  }
}

layer_state_t layer_state_set_user(layer_state_t state) {
  if (IS_LAYER_ON_STATE(state, LAYER_SHIFT)) {
    register_code(KC_LSHIFT);
  } else {
    unregister_code(KC_LSHIFT);
  }

  // swap_hands = IS_LAYER_ON_STATE(state, 9); // Swap QWERTY

  unsigned int layer = get_highest_layer(state);
  update_layer_ind(layer);
  update_mode_ind(layer);

  return state;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LT(1, KC_SPC):
      return TAPPING_TERM * 2 / 3;
    default:
      return TAPPING_TERM;
  }
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
      layer_ind_state = layer <= 2 ? SHOW_LEFT : SHOW_RIGHT;
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
    case LAYER_QWERTY:
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
  } else {
    // Blink QWERTY indicator when game chat active
    if (game_chat_state == 2) {
      update_mode_ind(
        (led_timer++ % 16) < 4
          ? LAYER_QWERTY
          : LAYER_BASE
      );
    }
  }

  bool left = is_keyboard_left();
  bool show_left = layer_ind_state == SHOW_LEFT;
  if (layer_ind_state && (layer_ind_state == SHOW_BOTH || left == show_left)) {
    bool blink = is_layer_locked(layer) || layer == LAYER_EMOJI;
    if (!blink || (led_timer++ % 24) < 16) {
      render_indicator(layer_ind);
    }
  } else if (mode_ind_state) {
    render_indicator(mode_ind);
  }

  render_modifiers(layer);

  return false;
}

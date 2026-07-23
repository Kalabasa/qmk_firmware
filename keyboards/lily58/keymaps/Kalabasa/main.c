#include QMK_KEYBOARD_H
#include "keycodes.h"
#include "progmem.h"
#include "features/baybayin.h"
#include "features/bitwise_f.h"
#include "features/kana.h"
#include "features/passwd.h"

#define LAYER_BASE 0
#define LAYER_SHIFT 1
#define LAYER_SYMBOL 2
#define LAYER_NUM 3
#define LAYER_FUNC 4
#define LAYER_NAV 5
#define LAYER_EMOJI 6
#define LAYER_QWERTY 8
#define LAYER_BAYBAYIN 9
#define LAYER_KANA 10
#define MODAL_LAYERS ((1u << LAYER_QWERTY) | (1u << LAYER_BAYBAYIN) | (1u << LAYER_KANA))

extern keymap_config_t keymap_config;

typedef struct user_eeconfig_t {
  char passwd[128];
} user_eeconfig_t;
user_eeconfig_t user_config;

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

// For OS-aware shortcuts
static os_variant_t os = OS_LINUX;

// 0: Neutral, 1: Open bracket held, 2: Open & close brackets held
static int bracket_state = 0;

// 0: Inactive, 1: Active
static int game_chat_state = 0;

// Password manager config
const uint16_t passwd_toggle_key = PASS;
uint8_t PASSWD_SLOT_SIZE = 32;
uint8_t PASSWD_SLOT_COUNT = 4;

// Bitwise FXX input keys
const uint16_t bitwise_f_keys[] = { F_B1, F_B2, F_B3, F_B4 };
uint8_t NUM_BITWISE_F_KEYS = sizeof(bitwise_f_keys) / sizeof(uint16_t);


uint16_t get_primary_mod(void);
uint16_t get_desktop_mod(void);
uint16_t get_word_mod(void);
uint16_t get_emoji_picker_hotkey(void);
void (*get_record_func(keyrecord_t *record))(uint16_t);
void update_layer_ind(layer_state_t state);
void update_mode_ind(layer_state_t state);
void show_toast(const char* message, int time);


void keyboard_post_init_user(void) {
  os = keymap_config.swap_lctl_lgui ? OS_MACOS : OS_LINUX;
  eeconfig_read_user_datablock(&user_config, 0, sizeof(user_config));
}

bool process_detected_host_os_user(os_variant_t detected_os) {
  switch (detected_os) {
    case OS_MACOS:
    case OS_IOS:
      os = detected_os;
      keymap_config.swap_lctl_lgui = true;
      set_unicode_input_mode(UNICODE_MODE_MACOS);
      break;
    case OS_LINUX:
      os = detected_os;
      keymap_config.swap_lctl_lgui = false;
      set_unicode_input_mode(UNICODE_MODE_LINUX);
      break;
    default:
      break;
  }
  return true;
}

void eeconfig_init_user(void) {
  memset(&user_config, 0, sizeof(user_config));
  eeconfig_update_user_datablock_field(user_config, passwd);
}

bool process_f_keys(uint16_t keycode, keyrecord_t *record) {
  static uint8_t f_key_result = 0;
  static char f_key_msg[] = "F  ";

  if (!process_bitwise_f(keycode, record, &f_key_result)) {
    if (f_key_result) {
      f_key_msg[1] = '0' + f_key_result;
      f_key_msg[2] = ' ';
      if (f_key_result >= 10) {
        f_key_msg[2] = '0' + f_key_result % 10;
        f_key_msg[1] = '0' + f_key_result / 10;
      }
      show_toast(f_key_msg, 10);
    }
    return false;
  }

  return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (layer_state_is(LAYER_BAYBAYIN) && !process_baybayin(keycode, record)) return false;
  if (layer_state_is(LAYER_KANA) && !process_kana(keycode, record)) return false;
  if (!process_passwd(keycode, record, user_config.passwd)) return false;
  if (!process_f_keys(keycode, record)) return false;

  void (*record_func)(uint16_t) = get_record_func(record);

  switch (keycode) {
    /*
    Unshifted keys on layer 1
    */
    case KC_GRAVE:
    case KC_BACKSLASH:
    case KC_SEMICOLON:
      if (layer_state_is(LAYER_SHIFT)) {
        if (record->event.pressed) {
          unregister_code(KC_LSFT);
        } else {
          register_code(KC_LSFT);
        }
      }
      return true;

    case CG_NORM: // Ctrl as primary modifier (Linux/Windows)
      if (record->event.pressed) {
        os = OS_LINUX;
        set_unicode_input_mode(UNICODE_MODE_LINUX);
      }
      return true;
    case CG_LSWP: // GUI as primary modifier (macOS)
      if (record->event.pressed) {
        os = OS_MACOS;
        set_unicode_input_mode(UNICODE_MODE_MACOS);
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
        unregister_code(KC_LSFT);
        unregister_code(KC_RSFT);
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
    Temporarily activate normal typing on GAME_CHAT.
    Return to QWERTY after ENTER or ESCAPE or TO(0).
    game_chat_state machine:
      (0) -- GC --> (1)
       ^-------------' ENT/ESC/TO(0)
    */
    case GAME_CHAT:
      if (record->event.pressed) {
        if (game_chat_state == 0) {
          game_chat_state++;
          layer_move(LAYER_BASE);
        }
      }
      return false;
    case KC_ENTER: // reset on send
    case KC_ESCAPE: // reset on cancel
    case TO(0): // reset without doing in-game action
      if (!record->event.pressed && game_chat_state == 1) {
        game_chat_state = 0;
        if (keycode != TO(0)) layer_move(LAYER_QWERTY);
      }
      return true;

    default:
      return true;
  }
}

layer_state_t layer_state_set_user(layer_state_t state) {
  if (IS_LAYER_ON_STATE(state, LAYER_SHIFT)) {
    register_code(KC_LSFT);
  } else {
    unregister_code(KC_LSFT);
  }

  update_layer_ind(state);
  update_mode_ind(state);

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
    default: return 0;
  }
}

uint16_t get_desktop_mod(void) {
  switch (os) {
    case OS_LINUX: return QK_LCTL | QK_LGUI;
    case OS_MACOS: return QK_LCTL;
    default: return 0;
  }
}

uint16_t get_word_mod(void) {
  switch (os) {
    case OS_LINUX: return QK_LCTL;
    case OS_MACOS: return QK_LALT;
    default: return 0;
  }
}

uint16_t get_emoji_picker_hotkey(void) {
  switch (os) {
    case OS_LINUX: return G(KC_SEMICOLON);
    case OS_MACOS: return C(G(KC_SPACE));
    default: return 0;
  }
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

static const char* toast_msg = 0;
static int toast_timer = 0;
static int led_timer = 0;

void update_layer_ind(layer_state_t state) {
  unsigned int layer = get_highest_layer(state & ~MODAL_LAYERS);
  switch (layer) {
    case LAYER_SHIFT:
    case LAYER_SYMBOL:
    case LAYER_NUM:
    case LAYER_FUNC:
    case LAYER_NAV:
    case LAYER_EMOJI:
      layer_ind_state = layer <= LAYER_SYMBOL ? SHOW_LEFT : SHOW_RIGHT;
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

void update_mode_ind(layer_state_t state) {
  if (layer_state_cmp(state, LAYER_QWERTY) || game_chat_state == 1) {
    mode_ind_state = true;
    mode_ind[6] = 0x94;
    mode_ind[7] = 0x95;
    mode_ind[11] = 0xB4;
    mode_ind[12] = 0xB5;
  } else if (layer_state_cmp(state, LAYER_BAYBAYIN)) {
    mode_ind_state = true;
    mode_ind[6] = 0x98;
    mode_ind[7] = 0x99;
    mode_ind[11] = 0xB8;
    mode_ind[12] = 0xB9;
  } else if (layer_state_cmp(state, LAYER_KANA)) {
    mode_ind_state = true;
    mode_ind[6] = 0x9A;
    mode_ind[7] = 0x9B;
    mode_ind[11] = 0xBA;
    mode_ind[12] = 0xBB;
  } else {
    mode_ind_state = false;
  }
}

void show_toast(const char* message, int time) {
  toast_msg = message;
  toast_timer = time;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  return OLED_ROTATION_270;
}

void render_os(void) {
  int offset = 0;
  switch (os) {
    case OS_LINUX: offset = 0; break;
    case OS_MACOS: offset = 1; break;
    default: return;
  }
  oled_set_cursor(2 + offset, 0);
  oled_write_char(0xd4 + offset, false);
}

void render_indicator(const char* data) {
  oled_set_cursor(1, 6);
  oled_write(data, false);
  oled_set_cursor(1, 7);
  oled_write(data + 5, false);
  oled_set_cursor(1, 8);
  oled_write(data + 10, false);
  oled_set_cursor(1, 9);
  oled_write(data + 15, false);
}

void render_modifiers(unsigned int layer) {
  if (host_keyboard_led_state().caps_lock) {
    oled_set_cursor(0, 0);
    oled_write_char(0xc4, false);
  }

  unsigned int mods = get_mods();
  const int dx = is_keyboard_left() ? -1 : 1;
  int x = (5 - dx * 3) / 2;
  int y = 14;
  if (mods & MOD_MASK_CTRL) {
    oled_set_cursor(x, y);
    oled_write_char(0xc7, false);
  }
  x += dx;
  if (mods & MOD_MASK_ALT) {
    oled_set_cursor(x, y);
    oled_write_char(0xc6, false);
  }
  x += dx;
  if (mods & MOD_MASK_GUI) {
    oled_set_cursor(x, y);
    oled_write_char(0xc5, false);
  }
  x += dx;
  if (layer != 1 && mods & MOD_MASK_SHIFT) {
    oled_set_cursor(x, y);
    oled_write_char(0xc8, false);
  }
}

bool oled_task_user(void) {
  oled_clear();

  if (toast_msg) {
    oled_set_cursor(0, 10);
    oled_write(toast_msg, false);
    if (--toast_timer <= 0) {
      toast_msg = NULL;
    }
  }

  if (is_keyboard_master()) render_os();

  if (!is_keyboard_master()) {
    update_layer_ind(layer_state);
    update_mode_ind(layer_state);
  }

  unsigned int layer = get_highest_layer(layer_state & ~MODAL_LAYERS);
  bool left = is_keyboard_left();
  bool show_left = layer_ind_state == SHOW_LEFT;
  bool blink_layer = is_layer_locked(layer) || layer == LAYER_EMOJI || game_chat_state == 1;
  bool blink_mode = game_chat_state == 1;
  if (
    layer_ind_state
    && (!blink_layer || (led_timer % 16) < 8)
    && (layer_ind_state == SHOW_BOTH || left == show_left)
  ) {
    render_indicator(layer_ind);
  } else if (
    mode_ind_state
    && (!blink_mode || (led_timer % 16) >= 8)
  ) {
    render_indicator(mode_ind);
  }
  
  render_modifiers(layer);
  
  led_timer++;
  return false;
}
#include "kana.h"

// A-Z mapped to kana in UTF-8. Every row is one Roman letter.
// Since all chars are in range U+3040..U+30FF, the UTF-8 encoding of every char is always 3 bytes.
static char* HIRAGANA[] = {
  // A I U E O
  "あああああ", // A'
  "ばびぶべぼ", // B
  NULL,         // C
  "だぢづでど", // D
  "えええええ", // E'
  "ふふふふふ", // F*
  "がぎぐげご", // G
  "はひふへほ", // H
  "いいいいい", // I'
  "じじじじじ", // J*
  "かきくけこ", // K
  "らりるれろ", // L
  "まみむめも", // M
  "なにぬねの", // N
  "おおおおお", // O'
  "ぱぴぷぺぽ", // P
  NULL,         // Q
  "らりるれろ", // R
  "さしすせそ", // S
  "たちつてと", // T
  "ううううう", // U'
  "ゔゔゔゔゔ", // V*
  "わゐゑうう", // W*
  NULL,         // X
  "やいゆえよ", // Y*
  "ざじずぜぞ"  // Z
};

static bool is_consonant(uint16_t keycode) {
  return keycode == KC_B
    || keycode == KC_D
    || keycode == KC_G
    || keycode == KC_H
    || keycode == KC_J
    || keycode == KC_K
    || keycode == KC_L
    || keycode == KC_M
    || keycode == KC_N
    || keycode == KC_P
    || keycode == KC_R
    || keycode == KC_S
    || keycode == KC_T
    || keycode == KC_V
    || keycode == KC_W
    || keycode == KC_Y
    || keycode == KC_Z;
}

static int vowel_offset(uint16_t keycode) {
  switch (keycode) {
    case KC_A: return 0;
    case KC_I: return 3;
    case KC_U: return 6;
    case KC_E: return 9;
    case KC_O: return 12;
    default: return 0;
  }
}

static uint16_t curr_keycode = 0;
static uint16_t prev_keycode = 0;
static uint16_t preprev_keycode = 0;
static uint16_t prev_key_timer;

static bool match(char* seq) {
  return preprev_keycode - KC_A == seq[0] - 'a'
    && prev_keycode - KC_A == seq[1] - 'a'
    && curr_keycode - KC_A == seq[2] - 'a';
}

bool process_kana(uint16_t keycode, keyrecord_t *record) {
  static char buf[4] = "\0\0\0\0";
  curr_keycode = QK_MODS_GET_BASIC_KEYCODE(keycode);

  if (
    !record->event.pressed
    || !(curr_keycode >= KC_A && curr_keycode <= KC_Z)
    || get_mods()
  ) {
    if (record->event.pressed) {
      prev_keycode = preprev_keycode = 0;
    }
    return true;
  }

  if (prev_keycode && timer_elapsed(prev_key_timer) > 2000) {
    prev_keycode = preprev_keycode = 0;
  }
  
  if (record->event.pressed) {
    // the original letter is always sent for any other uses
    tap_code(curr_keycode);

    if (HIRAGANA[curr_keycode - KC_A]) {
      if (is_consonant(curr_keycode)) {
        if (curr_keycode == KC_N) {
          tap_code(KC_BACKSPACE); // delete 'n'
          send_unicode_string("ん");
        } else if (curr_keycode == prev_keycode) {
          tap_code(KC_BACKSPACE); // delete repeated letter
          tap_code(KC_BACKSPACE); // delete repeated letter
          send_unicode_string("っ");
        }
      } else { // curr_keycode is vowel
        if (is_consonant(prev_keycode)) {
          int backspaces = preprev_keycode == prev_keycode ? 1 : 2;
          int letter_idx = prev_keycode - KC_A;
          int offset = vowel_offset(curr_keycode);
          char* extra = NULL;
          
          // Hepburn support
          if (match("shi")) {
            backspaces = 3;
            letter_idx = KC_S - KC_A;
          } else if (match("chi") || match("tsu")) {
            backspaces = 3;
            letter_idx = KC_T - KC_A;
          } else if (match("dzu")) {
            backspaces = 3;
            letter_idx = KC_D - KC_A;
          } else if (
            (curr_keycode == KC_A || curr_keycode == KC_U || curr_keycode == KC_O)
            && (
              (prev_keycode == KC_Y && is_consonant(preprev_keycode))
              || (prev_keycode == KC_H && (preprev_keycode == KC_S || preprev_keycode == KC_C))
              || prev_keycode == KC_J
            )
          ) {
            backspaces = prev_keycode == KC_J ? 2 : 3;
            letter_idx = (prev_keycode == KC_J ? KC_J : (preprev_keycode == KC_C ? KC_T : preprev_keycode)) - KC_A;
            offset = vowel_offset(KC_I);
            switch (curr_keycode) {
              case KC_A: extra = "ゃ"; break;
              case KC_U: extra = "ゅ"; break;
              case KC_O: extra = "ょ"; break;
            }
          }
          
          while (backspaces-- > 0) tap_code(KC_BACKSPACE);
          strncpy(buf, HIRAGANA[letter_idx] + offset, 3);
          send_unicode_string(buf);
          if (extra) send_unicode_string(extra);
        } else { // prev_keycode is vowel
          tap_code(KC_BACKSPACE); // delete vowel letter
          strncpy(buf, HIRAGANA[curr_keycode - KC_A], 3);
          send_unicode_string(buf);
        }
      }
    }

    preprev_keycode = prev_keycode;
    prev_keycode = curr_keycode;
    prev_key_timer = timer_read();
  }

  return false;
}
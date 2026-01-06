#include "kana.h"

// A-Z mapped to kana in UTF-8. Every row is one Roman letter.
// Since all chars are in range U+3040..U+30FF, the UTF-8 encoding of every char is always 3 bytes.
static char HIRAGANA[26][5*3] = {
  // A I U E O
  "あああああ", // A'
  "ばびぶべぼ", // B
  "ちちちちち", // C
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
  "かきくけこ", // Q*
  "らりるれろ", // R
  "さしすせそ", // S
  "たちつてと", // T
  "ううううう", // U'
  "ゔゔゔゔゔ", // V*
  "わゐゑうう", // W*
  "かきくけこ", // X*
  "やいゆえよ", // Y*
  "ざじずぜぞ"  // Z
};

static bool is_consonant(uint16_t keycode) {
  return keycode == KC_B
    || keycode == KC_C
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

typedef struct {
  uint8_t backspaces;
  uint8_t letter_kc;
  uint8_t vowel_kc;
  uint8_t youon_kc;
} syllable_t;

static bool match(const char* seq) {
  return preprev_keycode - KC_A == seq[0] - 'a'
    && prev_keycode - KC_A == seq[1] - 'a'
    && curr_keycode - KC_A == seq[2] - 'a';
}

// called when prev_keycode is a consonant and curr_keycode is a vowel
// returns the identified syllable
syllable_t process_syllable(void) {
  bool preprev_cons = is_consonant(preprev_keycode);
  syllable_t result;

  if (preprev_cons) {
    if (match("shi")) {
      result.backspaces = 2;
      result.letter_kc = KC_S;
      result.vowel_kc = curr_keycode;
      result.youon_kc = 0;
      return result;
    } else if (match("chi") || match("tsu")) {
      result.backspaces = 2;
      result.letter_kc = KC_T;
      result.vowel_kc = curr_keycode;
      result.youon_kc = 0;
      return result;
    } else if (match("dzu")) {
      result.backspaces = 2;
      result.letter_kc = KC_D;
      result.vowel_kc = curr_keycode;
      result.youon_kc = 0;
      return result;
    }
  }

  bool youon_vowel = curr_keycode == KC_A || curr_keycode == KC_U || curr_keycode == KC_O;
  if (prev_keycode == KC_J && youon_vowel) {
    result.backspaces = 1;
    result.letter_kc = KC_J;
    result.vowel_kc = KC_I;
    result.youon_kc = curr_keycode;
    return result;
  }

  if (
    (preprev_cons && prev_keycode == KC_Y && youon_vowel)
    || ((preprev_keycode == KC_S || preprev_keycode == KC_C) && prev_keycode == KC_H && youon_vowel)
  ) {
    result.backspaces = 2;
    result.letter_kc = preprev_keycode == KC_C ? KC_T : preprev_keycode;
    result.vowel_kc = KC_I;
    result.youon_kc = curr_keycode;
    return result;
  }

  result.backspaces = 1;
  result.letter_kc = prev_keycode; // must be consonant
  result.vowel_kc = curr_keycode; // must be vowel
  result.youon_kc = 0;
  return result;
}

bool process_kana(uint16_t keycode, keyrecord_t *record) {
  static char buf[4] = "\0\0\0\0";
  curr_keycode = QK_MODS_GET_BASIC_KEYCODE(keycode);

  bool consume = (curr_keycode >= KC_A && curr_keycode <= KC_Z)
              || (prev_keycode == KC_N && curr_keycode == KC_QUOTE);

  if (
    !record->event.pressed
    || get_mods()
    || !consume
  ) {
    if (record->event.pressed) {
      prev_keycode = preprev_keycode = 0;
    }
    return true;
  }

  if (prev_keycode && timer_elapsed(prev_key_timer) > 2000) {
    prev_keycode = preprev_keycode = 0;
  }

  bool curr_cons = is_consonant(curr_keycode);
  bool prev_cons = is_consonant(prev_keycode);

  if (record->event.pressed) {
    if (curr_keycode == KC_N) {
      send_unicode_string("ん");
    } else if (curr_keycode == KC_QUOTE) {
      prev_keycode = preprev_keycode = 0;
    } else if (curr_cons && curr_keycode == prev_keycode) {
      tap_code(KC_BACKSPACE); // delete repeated consonant
      send_unicode_string("っ");
      tap_code(curr_keycode);
    } else if (curr_cons) {
      // start of a syllable
      // send roman letter for immediate feedback, delete later
      tap_code(curr_keycode);
    } else if (!prev_cons) {
      // independent vowel
      strncpy(buf, &HIRAGANA[curr_keycode - KC_A][0], 3);
      send_unicode_string(buf);
    } else { // prev_cons && curr_vowel
      // end of a syllable
      syllable_t syllable = process_syllable();
      while (syllable.backspaces-- > 0) tap_code(KC_BACKSPACE);
      char* kana_ptr = &HIRAGANA[syllable.letter_kc - KC_A][vowel_offset(syllable.vowel_kc)];
      // every possible kana character is 3 bytes long (utf-8)
      strncpy(buf, kana_ptr, 3);
      send_unicode_string(buf);
      switch (syllable.youon_kc) {
        case KC_A: send_unicode_string("ゃ"); break;
        case KC_U: send_unicode_string("ゅ"); break;
        case KC_O: send_unicode_string("ょ"); break;
      }
    }

    preprev_keycode = prev_keycode;
    prev_keycode = curr_keycode;
    prev_key_timer = timer_read();
  }

  return false;
}
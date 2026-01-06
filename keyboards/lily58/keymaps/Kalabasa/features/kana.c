#include "kana.h"

// A-Z mapped to kana in UTF-8. Every row is one Roman letter. Columns are vowels.
// Since all chars are in range U+3040..U+30FF, the UTF-8 encoding of every char is always 3 bytes.
// 拗 (0xE6 0x8B 0x97) indicates youon (never in the I column)
// 捨 (0xE6 0x8D 0xA8) indicates additional small vowel (never in the U column)
static char MAP[28][5*3] = {
  // A I U E O
  "あああああ", // A (vowel)
  "ばびぶべぼ", // B
  "拗ち拗捨拗", // C
  "だぢづでど", // D
  "えええええ", // E (vowel)
  "捨捨ふ捨捨", // F
  "がぎぐげご", // G
  "はひふへほ", // H
  "いいいいい", // I (vowel)
  "拗じ拗捨拗", // J
  "かきくけこ", // K
  "らりるれろ", // L
  "まみむめも", // M
  "なにぬねの", // N
  "おおおおお", // O (vowel)
  "ぱぴぷぺぽ", // P
  "捨捨く捨捨", // Q
  "らりるれろ", // R
  "さしすせそ", // S
  "たちつてと", // T
  "ううううう", // U (vowel)
  "捨捨ゔ捨捨", // V
  "わ捨う捨捨", // W
  "かきくけこ", // X (invalid)
  "やいゆえよ", // Y
  "ざじずぜぞ", // Z
  // Extra mappings
  "ゃぃゅぇょ", // youon
  "ぁぃぅぇぉ", // small vowels
};

static bool is_consonant(uint16_t keycode) {
  return keycode == KC_B
    || keycode == KC_C
    || keycode == KC_D
    || keycode == KC_F
    || keycode == KC_G
    || keycode == KC_H
    || keycode == KC_J
    || keycode == KC_K
    || keycode == KC_L
    || keycode == KC_M
    || keycode == KC_N
    || keycode == KC_P
    || keycode == KC_Q
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

static bool is_youon(uint16_t consonant_keycode, uint16_t vowel_keycode) {
  // 拗 = 0xE6 0x8B 0x97
  // Check if second byte is 8B
  return MAP[consonant_keycode - KC_A][vowel_offset(vowel_keycode) + 1] == 0x8B;
}

static bool is_small_vowel(uint16_t consonant_keycode, uint16_t vowel_keycode) {
  // 捨 = 0xE6 0x8D 0xA8
  // Check if second byte is 8D
  return MAP[consonant_keycode - KC_A][vowel_offset(vowel_keycode) + 1] == 0x8D;
}

static bool is_sibiliant(uint16_t keycode) {
  return keycode == KC_C
    || keycode == KC_J
    || keycode == KC_S
    || keycode == KC_T
    || keycode == KC_Z;
}

static uint16_t curr_keycode = 0;
static uint16_t prev_keycode = 0;
static uint16_t preprev_keycode = 0;
static uint16_t prev_key_timer;

const char* YOUON_PTR = &MAP[26][0];
const char* SMALL_PTR = &MAP[27][0];

typedef struct {
  uint8_t backspaces;
  uint8_t consonant_kc;
  uint8_t vowel_kc;
  const char* extra_char_ptr;
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
    if (match("shi")) { // し
      result.backspaces = 2;
      result.consonant_kc = KC_S;
      result.vowel_kc = curr_keycode;
      result.extra_char_ptr = NULL;
      return result;
    } else if (match("chi") || match("tsu")) { // ち,つ
      result.backspaces = 2;
      result.consonant_kc = KC_T;
      result.vowel_kc = curr_keycode;
      result.extra_char_ptr = NULL;
      return result;
    } else if (match("dzu")) { // づ
      result.backspaces = 2;
      result.consonant_kc = KC_D;
      result.vowel_kc = curr_keycode;
      result.extra_char_ptr = NULL;
      return result;
    }
  }

  // CCV => きゃ,きゅ,きょ,...
  bool youon_vowel = curr_keycode == KC_A || curr_keycode == KC_U || curr_keycode == KC_O;
  if (
    (preprev_cons && prev_keycode == KC_Y && youon_vowel)
    || ((preprev_keycode == KC_S || preprev_keycode == KC_C) && prev_keycode == KC_H && youon_vowel)
  ) {
    result.backspaces = 2;
    result.consonant_kc = preprev_keycode == KC_C ? KC_T : preprev_keycode;
    result.vowel_kc = KC_I;
    result.extra_char_ptr = YOUON_PTR + vowel_offset(curr_keycode);
    return result;
  }

  // じゃ,じゅ,じょ
  if (is_youon(prev_keycode, curr_keycode)) {
    result.backspaces = 1;
    result.consonant_kc = prev_keycode;
    result.vowel_kc = KC_I;
    result.extra_char_ptr = YOUON_PTR + vowel_offset(curr_keycode);
    return result;
  }

  // ファ,ヴァ,ジェ,...
  if (is_small_vowel(prev_keycode, curr_keycode)) {
    result.backspaces = 1;
    result.consonant_kc = prev_keycode;
    result.vowel_kc = is_sibiliant(prev_keycode) ? KC_I : KC_U;
    result.extra_char_ptr = SMALL_PTR + vowel_offset(curr_keycode);
    return result;
  }

  // a regular syllable
  result.backspaces = 1;
  result.consonant_kc = prev_keycode; // must be consonant
  result.vowel_kc = curr_keycode; // must be vowel
  result.extra_char_ptr = NULL;
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
      strncpy(buf, &MAP[curr_keycode - KC_A][0], 3);
      send_unicode_string(buf);
    } else { // prev_cons && curr_vowel
      // end of a syllable
      syllable_t syllable = process_syllable();

      while (syllable.backspaces-- > 0) tap_code(KC_BACKSPACE);

      char* kana_char_ptr = &MAP[syllable.consonant_kc - KC_A][vowel_offset(syllable.vowel_kc)];
      // every possible kana character is 3 bytes long (utf-8)
      strncpy(buf, kana_char_ptr, 3);
      send_unicode_string(buf);

      if (syllable.extra_char_ptr) {
        strncpy(buf, syllable.extra_char_ptr, 3);
        send_unicode_string(buf);
      }
    }

    preprev_keycode = prev_keycode;
    prev_keycode = curr_keycode;
    prev_key_timer = timer_read();
  }

  return false;
}
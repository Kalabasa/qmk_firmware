#include "kana.h"

enum {
  ROW_EXTRA_YOUON = 'z' - 'a' + 1,
  ROW_EXTRA_CH,
  ROW_EXTRA_SH,
  ROW_COUNT
};

// A-Z mapped to kana in UTF-8.
//
// Since kana is a syllabary, it's a 2D map:
//   1. Rows map to one Roman letter in ASCII order.
//   2. Columns map to vowels A, I, U, E, O, in that order.
//   3. Thus, a syllable is a row-column pair.
//
// Every char takes up exactly 3 bytes (due to UTF-8 encoding of chars in this range U+3040..U+30FF).
// Thus it's possible address any syllable by its letter-vowel pair.
// - MAP[letter_idx][vowel_idx * 3]
//
// Special cases:
// - Vowels are the same for all columns
// - XA, XI, XU, XE, XO are mapped to small vowel kana
// - 拗 (0xE6 0x8B 0x97) indicates youon (never in the I column)
// - 捨 (0xE6 0x8D 0xA8) indicates additional small vowel must be used
static char MAP[ROW_COUNT][5*3] = {
  // A I U E O
  "あああああ", // A (vowel)
  "ばびぶべぼ", // B
  "拗ち拗捨拗", // C
  "だ捨づでど", // D
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
  "た捨つてと", // T
  "ううううう", // U (vowel)
  "捨捨ゔ捨捨", // V
  "わ捨う捨捨", // W
  "ぁぃぅぇぉ", // X (small vowels)
  "やいゆえよ", // Y
  "ざじずぜぞ", // Z
  [ROW_EXTRA_YOUON] = "ゃぃゅぇょ",
  [ROW_EXTRA_CH]    = "拗ち拗捨拗",
  [ROW_EXTRA_SH]    = "拗し拗捨拗",
};

const unsigned int ROW_SMALL_VOWELS = 'x' - 'a';

static bool is_consonant(uint16_t keycode) {
  int row = keycode - KC_A;
  if (row < 0 || row > KC_Z - KC_A) return false;
  // vowels have the same char for all columns
  char byte1 = MAP[row][1];
  char byte2 = MAP[row][2];
  for (int i = 3; i < 15; i += 3) {
    if (byte1 != MAP[row][i + 1] || byte2 != MAP[row][i + 2]) return true;
  }

  return false;
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
  // The actual rules are encoded in the MAP via special value 拗.
  return strncmp("拗", &MAP[consonant_keycode - KC_A][vowel_offset(vowel_keycode)], 3) == 0;
}

static bool is_small_vowel(uint16_t consonant_keycode, uint16_t vowel_keycode) {
  // The actual rules are encoded in the MAP via special value 捨.
  return strncmp("捨", &MAP[consonant_keycode - KC_A][vowel_offset(vowel_keycode)], 3) == 0;
}

// This determines which vowel ending (-I vs -U) will be used for the base syllable when using small vowels
// e.g.
//   ちぇ uses -I as in [TI + smallE]
//   ふぁ uses -U as in [FU + smallA]
// Returns KC_I or KC_U
static uint16_t get_base_vowel(uint16_t consonant_keycode, uint16_t vowel_keycode) {
  // The actual rules are encoded in the MAP:
  // In any row containing syllables that require small vowels,
  // the syllable that doesn't require a small vowel is the base vowel.
  for (const uint16_t* p = (uint16_t[]){ KC_I, KC_E, KC_U, KC_O, KC_A }; *p != KC_A; p++) {
    char* kana_ptr = &MAP[consonant_keycode - KC_A][vowel_offset(*p)];
    if (strncmp("拗", kana_ptr, 3) != 0 && strncmp("捨", kana_ptr, 3) != 0) {
      return *p;
    }
  }
  // Not expected if MAP is set up correctly
  return 0;
}

// convert a UTF-8 encoded hiragana character to katakana in-place
static void to_katakana(char* kana) {
  if (strncmp(kana, "ぁ", 3) < 0 || strncmp(kana, "ゔ", 3) > 0) {
    return;
  }

  // decode utf8 to unicode code point
  // ignoring the first byte, it's always 0xE3
  uint32_t codepoint = ((kana[1] & 0b00111111) << 6) | (kana[2] & 0b00111111);
  codepoint += (uint32_t)L'ア' - (uint32_t)L'あ';
  kana[1] = 0x80 | ((codepoint >> 6) & 0b00111111);
  kana[2] = 0x80 | (codepoint & 0b00111111);
}

static uint16_t curr_keycode = 0;
static uint16_t prev_keycode = 0;
static uint16_t preprev_keycode = 0;
static uint16_t prev_key_timer;

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
  result.backspaces = 0;

  // three-letter combinations
  if (preprev_cons) {
    if (match("shi")) { // し
      result.backspaces += 2;
      result.consonant_kc = KC_S;
      result.vowel_kc = curr_keycode;
      result.extra_char_ptr = NULL;
      return result;
    } else if (match("chi") || match("tsu")) { // ち,つ
      result.backspaces += 2;
      result.consonant_kc = KC_T;
      result.vowel_kc = curr_keycode;
      result.extra_char_ptr = NULL;
      return result;
    } else if (match("dzu")) { // づ
      result.backspaces += 2;
      result.consonant_kc = KC_D;
      result.vowel_kc = curr_keycode;
      result.extra_char_ptr = NULL;
      return result;
    }

    // にゃ,にゅ,きょ,...
    bool youon_vowel = curr_keycode == KC_A || curr_keycode == KC_U || curr_keycode == KC_O;
    if (preprev_cons && prev_keycode == KC_Y && youon_vowel) {
      result.backspaces += 2;
      result.consonant_kc = preprev_keycode;
      result.vowel_kc = KC_I;
      result.extra_char_ptr = &MAP[ROW_EXTRA_YOUON][vowel_offset(curr_keycode)];
      return result;
    }

    // Map C+H and S+H to the CH/SH rows in the map
    if (preprev_keycode == KC_C && prev_keycode == KC_H) {
      result.backspaces += 1;
      preprev_keycode = 0;
      // not a 'keycode' anymore, just an index 
      prev_keycode = KC_A + ROW_EXTRA_CH;
    } else if (preprev_keycode == KC_S && prev_keycode == KC_H) {
      result.backspaces += 1;
      preprev_keycode = 0;
      prev_keycode = KC_A + ROW_EXTRA_SH;
    }
  }

  // じゃ,じゅ,じょ
  if (is_youon(prev_keycode, curr_keycode)) {
    result.backspaces += 1;
    result.consonant_kc = prev_keycode;
    result.vowel_kc = KC_I;
    result.extra_char_ptr = &MAP[ROW_EXTRA_YOUON][vowel_offset(curr_keycode)];
    return result;
  }

  // ファ,ヴァ,ジェ,...
  if (is_small_vowel(prev_keycode, curr_keycode)) {
    result.backspaces += 1;
    result.consonant_kc = prev_keycode;
    result.vowel_kc = get_base_vowel(prev_keycode, curr_keycode);
    result.extra_char_ptr = &MAP[ROW_SMALL_VOWELS][vowel_offset(curr_keycode)];
    return result;
  }

  // a regular syllable
  result.backspaces += 1;
  result.consonant_kc = prev_keycode; // must be consonant
  result.vowel_kc = curr_keycode; // must be vowel
  result.extra_char_ptr = NULL;
  return result;
}

// Send 3 bytes of UTF-8 encoded kana character
void send_kana_unicode(const char* kana_ptr) {
  static char buf[4] = "\0\0\0\0";
  strncpy(buf, kana_ptr, 3);
  if (get_mods() & MOD_MASK_SHIFT) {
    to_katakana(buf);
  }
  send_unicode_string(buf);
}

bool process_kana(uint16_t keycode, keyrecord_t *record) {
  curr_keycode = QK_MODS_GET_BASIC_KEYCODE(keycode);

  bool consume = (curr_keycode >= KC_A && curr_keycode <= KC_Z)
              || (prev_keycode == KC_N && curr_keycode == KC_QUOTE);

  if (
    !record->event.pressed
    || (get_mods() & ~MOD_MASK_SHIFT)
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
      send_kana_unicode(&MAP[curr_keycode - KC_A][0]);
    } else { // prev_cons && curr_vowel
      // end of a syllable
      syllable_t syllable = process_syllable();

      while (syllable.backspaces-- > 0) tap_code(KC_BACKSPACE);

      send_kana_unicode(&MAP[syllable.consonant_kc - KC_A][vowel_offset(syllable.vowel_kc)]);

      if (syllable.extra_char_ptr) {
        send_kana_unicode(syllable.extra_char_ptr);
      }
    }

    preprev_keycode = prev_keycode;
    prev_keycode = curr_keycode;
    prev_key_timer = timer_read();
  }

  return false;
}
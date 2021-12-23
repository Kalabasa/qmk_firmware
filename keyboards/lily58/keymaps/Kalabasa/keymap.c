#include QMK_KEYBOARD_H
#include "unicode.c"

/* THIS FILE WAS GENERATED!
 *
 * This file was generated by qmk json2c. You may or may not want to
 * edit it directly.
 */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT(KC_GESC, LGUI(KC_LBRC), LGUI(KC_RBRC), C(S(KC_TAB)), LCTL(KC_TAB), KC_NO, KC_NO, RCTL(KC_LEFT), RCTL(KC_DOWN), RCTL(KC_UP), RCTL(KC_RGHT), C(G(KC_SPC)), KC_QUES, KC_Q, KC_W, KC_F, KC_P, KC_B, KC_J, KC_L, KC_U, KC_Y, KC_EQL, KC_QUOT, LSFT_T(KC_TAB), KC_A, KC_R, KC_S, KC_T, KC_G, KC_M, KC_N, KC_E, KC_I, KC_O, RCTL_T(KC_ENT), KC_LALT, KC_Z, KC_X, KC_C, KC_D, KC_V, KC_NO, KC_NO, KC_K, KC_H, KC_DOT, KC_SCLN, KC_SLSH, RCTL(KC_SPC), TT(3), TT(2), KC_LGUI, LT(1,KC_SPC), KC_BSPC, TT(5), TT(6), TT(4)),
	[1] = LAYOUT(KC_ESC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_EXLM, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_UNDS, KC_DQUO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_ENT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_COMM, KC_COLN, KC_BSLS, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO),
	[2] = LAYOUT(KC_NO, KC_PERC, KC_AT, KC_HASH, KC_TILD, UC(0x20B1), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_GRV, KC_ASTR, KC_DLR, KC_LCBR, KC_RCBR, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_MINS, KC_LBRC, KC_RBRC, KC_LPRN, KC_RPRN, KC_AMPR, KC_SPC, KC_RGUI, KC_RALT, KC_RCTL, KC_RSFT, KC_ENT, KC_PLUS, KC_CIRC, KC_LT, KC_SLSH, KC_GT, KC_PIPE, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, MO(2), KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO),
	[3] = LAYOUT(KC_F, KC_E, KC_D, KC_C, KC_B, KC_A, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_1, KC_2, KC_3, KC_HASH, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_4, KC_5, KC_6, KC_1, KC_0, KC_NO, KC_SPC, KC_RGUI, KC_RALT, KC_RCTL, KC_RSFT, KC_ENT, KC_7, KC_8, KC_9, KC_PDOT, KC_X, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_DOT, KC_NO, KC_NO, KC_NO, MO(3), KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO),
	[4] = LAYOUT(LCTL(KC_EJCT), TO(8), TO(12), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_VOLD, KC_VOLU, KC_BRID, KC_BRIU, C(G(KC_Q)), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_F1, KC_F2, KC_F3, KC_F4, KC_NO, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, KC_NO, KC_MPLY, KC_MUTE, KC_F5, KC_F6, KC_F7, KC_F8, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_MPRV, KC_MNXT, KC_F9, KC_F10, KC_F11, KC_F12, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO, MO(4)),
	[5] = LAYOUT(KC_ESC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RSFT(KC_TAB), KC_TAB, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, SGUI(KC_Z), LGUI(KC_X), LGUI(KC_C), LGUI(KC_V), LGUI(KC_L), LGUI(KC_A), KC_NO, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, KC_NO, LGUI(KC_Z), KC_LEFT, KC_UP, KC_DOWN, KC_RGHT, KC_ENT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_BSPC, KC_DEL, KC_HOME, KC_PGUP, KC_PGDN, KC_END, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, MO(5), KC_NO, KC_NO),
	[6] = LAYOUT(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_MS_L, KC_MS_U, KC_MS_D, KC_MS_R, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, MO(6), KC_NO),
	[8] = LAYOUT(KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_EQL, KC_GRV, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_MINS, KC_TAB, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_NO, KC_ENT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, SH_OS, TG(8), KC_N, KC_M, KC_LEFT, KC_UP, KC_DOWN, KC_RGHT, KC_LGUI, KC_LCTL, KC_LALT, KC_SPC, KC_BSPC, KC_RALT, KC_RCTL, KC_RGUI),
	[12] = LAYOUT(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, X(UC_TL_B), X(UC_TL_H), X(UC_TL_G), X(UC_TL_R), KC_NO, X(UC_TL_Y), X(UC_TL_W), KC_NO, KC_NO, KC_NO, KC_NO, X(UC_TL_A), X(UC_TL_S), X(UC_TL_NG), X(UC_TL_N), X(UC_TL_D), X(UC_TL_P), XP(UC_TL_KI, UC_TL_I), XP(UC_TL_KU, UC_TL_U), X(UC_TL_M), X(UC_TL_T), KC_ENT, KC_NO, KC_NO, KC_NO, KC_NO, X(UC_TL_K), KC_NO, KC_NO, TG(12), KC_NO, X(UC_TL_L), X(UC_DOUBLE_DANDA), X(UC_DANDA), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, LSFT_T(KC_SPC), KC_BSPC, X(UC_TL_VIRAMA), KC_NO, KC_NO)
};


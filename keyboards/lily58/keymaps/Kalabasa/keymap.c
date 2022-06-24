#include QMK_KEYBOARD_H
#include "unicode.c"

/* THIS FILE WAS GENERATED!
 *
 * This file was generated by qmk json2c. You may or may not want to
 * edit it directly.
 */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT(KC_ESC, RCTL(KC_LBRC), RCTL(KC_RBRC), RCTL(S(KC_TAB)), RCTL(KC_TAB), KC_NO, KC_NO, C(G(KC_LEFT)), C(G(KC_UP)), C(G(KC_DOWN)), C(G(KC_RIGHT)), KC_BSPC, KC_QUES, KC_Q, KC_W, KC_F, KC_P, KC_B, KC_J, KC_L, KC_U, KC_Y, KC_EQL, KC_QUOT, KC_TAB, RGUI_T(KC_A), LSFT_T(KC_R), LALT_T(KC_S), RCTL_T(KC_T), KC_G, KC_M, RCTL_T(KC_N), RALT_T(KC_E), RSFT_T(KC_I), RGUI_T(KC_O), KC_ENT, KC_NO, KC_Z, KC_X, KC_C, KC_D, KC_V, KC_NO, KC_NO, KC_K, KC_H, KC_DOT, KC_SCLN, KC_SLSH, KC_NO, KC_LGUI, MO(2), KC_LCTL, LT(1,KC_SPC), KC_SPC, MO(5), MO(3), MO(4)),
	[1] = LAYOUT(KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_EXLM, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_UNDS, KC_DQUO, KC_TRNS, KC_A, KC_R, KC_S, KC_T, KC_TRNS, KC_TRNS, KC_N, KC_E, KC_I, KC_O, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_COMM, KC_COLN, KC_BSLS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO),
	[2] = LAYOUT(KC_NO, KC_PERC, KC_AT, KC_HASH, KC_TILD, UC(0x20B1), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_GRV, KC_ASTR, KC_DLR, KC_LCBR, KC_RCBR, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_MINS, KC_LBRC, KC_RBRC, KC_LPRN, KC_RPRN, KC_AMPR, KC_NO, KC_RCTL, KC_RALT, KC_RSFT, KC_RGUI, KC_TRNS, KC_PLUS, KC_CIRC, KC_LT, KC_SLSH, KC_GT, KC_PIPE, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO),
	[3] = LAYOUT(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_PSLS, KC_PAST, KC_PMNS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LPRN, KC_RPRN, KC_7, KC_8, KC_9, KC_PPLS, KC_NO, KC_RGUI, KC_LSFT, KC_LALT, KC_RCTL, KC_NO, KC_NO, KC_0, KC_4, KC_5, KC_6, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_1, KC_2, KC_3, KC_DOT, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_TRNS, KC_NO),
	[4] = LAYOUT(KC_NO, TO(8), TO(12), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_VOLD, KC_VOLU, KC_BRID, KC_BRIU, KC_NO, LCG_SWP, LCG_NRM, KC_NO, KC_NO, KC_NO, KC_NO, KC_PSCR, KC_F1, KC_F2, KC_F3, KC_F4, KC_NO, KC_RGUI, KC_LSFT, KC_LALT, KC_RCTL, KC_NO, KC_MPLY, KC_MUTE, KC_F5, KC_F6, KC_F7, KC_F8, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_MPRV, KC_MNXT, KC_F9, KC_F10, KC_F11, KC_F12, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_TRNS),
	[5] = LAYOUT(KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RSFT(KC_TAB), KC_TAB, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_AGIN, KC_UNDO, KC_CUT, KC_COPY, KC_PSTE, KC_NO, KC_NO, KC_RGUI, KC_LSFT, KC_LALT, KC_RCTL, KC_NO, KC_DEL, KC_LEFT, KC_UP, KC_DOWN, KC_RGHT, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_HOME, KC_PGUP, KC_PGDN, KC_END, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO),
	[6] = LAYOUT(KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	[7] = LAYOUT(KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	[8] = LAYOUT(KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC, KC_GRV, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSLS, KC_TAB, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_UP, KC_ENT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, OSL(9), TG(8), KC_N, KC_M, KC_DOT, KC_LEFT, KC_DOWN, KC_RGHT, KC_LGUI, KC_LALT, KC_LCTL, KC_SPC, KC_SPC, KC_RCTL, KC_RALT, KC_RGUI),
	[9] = LAYOUT(KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	[10] = LAYOUT(KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	[11] = LAYOUT(KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	[12] = LAYOUT(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_BSPC, KC_NO, KC_NO, X(UC_TL_B), X(UC_TL_H), X(UC_TL_G), X(UC_TL_R), KC_NO, X(UC_TL_Y), X(UC_TL_W), KC_NO, KC_NO, KC_NO, KC_NO, X(UC_TL_A), X(UC_TL_S), X(UC_TL_NG), X(UC_TL_N), X(UC_TL_D), X(UC_TL_P), XP(UC_TL_KI, UC_TL_I), XP(UC_TL_KU, UC_TL_U), X(UC_TL_M), X(UC_TL_T), KC_ENT, KC_NO, KC_NO, KC_NO, KC_NO, X(UC_TL_K), KC_NO, KC_NO, TG(12), KC_NO, X(UC_TL_L), X(UC_DOUBLE_DANDA), X(UC_DANDA), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, LSFT_T(KC_SPC), KC_SPC, KC_NO, KC_NO, KC_NO)
};


#include QMK_KEYBOARD_H
#include "keycodes.h" 

/* THIS FILE WAS GENERATED!
 *
 * This file was generated by qmk json2c. You may or may not want to
 * edit it directly.
 */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT(KC_ESC, BRACKET_BACK, BRACKET_FORWARD, RCTL(S(KC_TAB)), RCTL(KC_TAB), KC_NO, OSL(6), DESKTOP_LEFT, DESKTOP_UP, DESKTOP_DOWN, DESKTOP_RIGHT, KC_BSPC, KC_QUES, KC_Q, KC_W, KC_F, KC_P, KC_B, KC_J, KC_L, KC_U, KC_Y, KC_EQL, KC_QUOT, KC_TAB, LSFT_T(KC_A), LGUI_T(KC_R), LALT_T(KC_S), LCTL_T(KC_T), KC_G, KC_M, LCTL_T(KC_N), LALT_T(KC_E), LGUI_T(KC_I), LSFT_T(KC_O), KC_ENT, KC_RCTL, KC_Z, KC_X, KC_C, KC_D, KC_V, KC_NO, KC_NO, KC_K, KC_H, KC_DOT, KC_COMM, KC_SLSH, KC_RGUI, KC_LGUI, MO(2), KC_LCTL, LT(1,KC_SPC), KC_SPC, MO(5), MO(3), MO(4)),
	[1] = LAYOUT(KC_TRNS, KC_NO, KC_TILD, KC_AT, KC_HASH, KC_NO, KC_NO, KC_NO, KC_DLR, KC_NO, KC_NO, KC_TRNS, KC_EXLM, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_UNDS, KC_DQUO, KC_TRNS, KC_A, KC_R, KC_S, KC_T, KC_TRNS, KC_TRNS, KC_N, KC_E, KC_I, KC_O, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_COLN, KC_SCLN, KC_ASTR, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO),
	[2] = LAYOUT(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_GRV, KC_LBRC, KC_RBRC, KC_LCBR, KC_RCBR, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_BSLS, KC_PIPE, KC_AMPR, KC_LPRN, KC_RPRN, KC_PERC, KC_NO, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, KC_TRNS, KC_CIRC, KC_LT, KC_PLUS, KC_MINS, KC_EQL, KC_GT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO, LAYER_LOCK, KC_TRNS, KC_NO, KC_NO, KC_NO),
	[3] = LAYOUT(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_RPRN, KC_PAST, KC_PSLS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LPRN, KC_7, KC_8, KC_9, KC_PMNS, KC_NO, KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL, KC_NO, KC_NO, KC_0, KC_4, KC_5, KC_6, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_PPLS, KC_1, KC_2, KC_3, KC_DOT, KC_NO, KC_NO, KC_NO, KC_TRNS, LAYER_LOCK, KC_NO, KC_TRNS, KC_NO),
	[4] = LAYOUT(KC_NO, TO(8), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_VOLD, KC_VOLU, KC_BRID, KC_BRIU, KC_NO, KC_NO, LCG_SWP, KC_NO, LCG_NRM, KC_NO, KC_NO, KC_PSCR, KC_F1, KC_F2, KC_F3, KC_F4, KC_NO, KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL, KC_NO, KC_NO, KC_MUTE, KC_F5, KC_F6, KC_F7, KC_F8, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_F13, KC_F14, KC_F9, KC_F10, KC_F11, KC_F12, KC_NO, KC_NO, KC_NO, KC_TRNS, LAYER_LOCK, KC_NO, KC_NO, KC_TRNS),
	[5] = LAYOUT(KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RSFT(KC_TAB), KC_TAB, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, WORD_DELETE, WORD_PREV, WORD_SELECT, KC_NO, WORD_NEXT, KC_NO, KC_NO, KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL, KC_NO, KC_DEL, KC_LEFT, KC_UP, KC_DOWN, KC_RGHT, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_HOME, KC_PGUP, KC_PGDN, KC_END, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, LAYER_LOCK, KC_TRNS, KC_NO, KC_NO),
	[6] = LAYOUT(TG(6), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, EMOJI, KC_NO, KC_NO, KC_NO, KC_NO, X(CROSS), KC_NO, X(EMO_Q), X(EMO_W), X(EMO_F), X(EMO_P), KC_NO, KC_NO, X(LEFT_HAND), X(UP_HAND), X(DOWN_HAND), X(RIGHT_HAND), X(LIKE), KC_NO, X(EMO_A), X(EMO_R), X(EMO_S), X(EMO_T), KC_NO, KC_NO, X(LEFT), X(UP), X(DOWN), X(RIGHT), X(CHECK), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, TG(6), KC_NO, X(LEFT_BOX), X(UP_BOX), X(DOWN_BOX), X(RIGHT_BOX), X(CHECK_BOX), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),
	[8] = LAYOUT(KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC, KC_GRV, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSLS, KC_TAB, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_UP, KC_ENT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, OSL(9), TG(8), KC_N, KC_M, KC_DOT, KC_LEFT, KC_DOWN, KC_RGHT, KC_LGUI, KC_LALT, KC_LCTL, KC_SPC, KC_SPC, KC_RCTL, KC_RALT, KC_RGUI),
};


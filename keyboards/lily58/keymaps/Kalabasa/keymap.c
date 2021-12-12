#include QMK_KEYBOARD_H

/* THIS FILE WAS GENERATED!
 *
 * This file was generated by qmk json2c. You may or may not want to
 * edit it directly.
 */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT(KC_ESC, KC_NO, KC_NO, KC_NO, C(S(KC_TAB)), LCTL(KC_TAB), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, C(G(KC_SPC)), KC_NO, KC_Q, KC_W, KC_F, KC_P, KC_B, KC_J, KC_L, KC_U, KC_Y, KC_EQL, KC_QUOT, LGUI_T(KC_TAB), KC_A, KC_R, KC_S, KC_T, KC_G, KC_M, KC_N, KC_E, KC_I, KC_O, RCTL_T(KC_ENT), KC_SLSH, KC_Z, KC_X, KC_C, KC_D, KC_V, KC_NO, TG(8), KC_K, KC_H, KC_DOT, KC_SCLN, KC_QUES, KC_NO, MO(4), MO(3), MO(2), LT(1,KC_SPC), KC_BSPC, MO(5), MO(6), KC_RALT),
	[1] = LAYOUT(LSFT(KC_ESC), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, LSFT(KC_Q), LSFT(KC_W), LSFT(KC_F), LSFT(KC_P), LSFT(KC_B), LSFT(KC_J), LSFT(KC_L), LSFT(KC_U), LSFT(KC_Y), KC_UNDS, KC_DQUO, KC_TRNS, LSFT(KC_A), LSFT(KC_R), LSFT(KC_S), LSFT(KC_T), LSFT(KC_G), LSFT(KC_M), LSFT(KC_N), LSFT(KC_E), LSFT(KC_I), LSFT(KC_O), LSFT(KC_ENT), KC_BSLS, LSFT(KC_Z), LSFT(KC_X), LSFT(KC_C), LSFT(KC_D), LSFT(KC_V), KC_NO, KC_NO, LSFT(KC_K), LSFT(KC_H), KC_COMM, KC_COLN, KC_EXLM, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, LSFT(KC_BSPC), KC_NO, KC_NO, KC_TRNS),
	[2] = LAYOUT(LGUI(KC_GRV), KC_NO, KC_PERC, KC_AT, KC_HASH, KC_ASTR, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_CIRC, KC_PIPE, KC_AMPR, KC_DLR, KC_PLUS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_GRV, KC_LBRC, KC_RBRC, KC_LPRN, KC_RPRN, KC_MINS, KC_NO, KC_RGUI, KC_RALT, KC_RCTL, KC_RSFT, KC_NO, KC_TILD, KC_LT, KC_GT, KC_LCBR, KC_RCBR, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_TRNS),
	[3] = LAYOUT(KC_NO, KC_A, KC_B, KC_C, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_F, KC_7, KC_8, KC_9, KC_HASH, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_E, KC_4, KC_5, KC_6, KC_0, KC_1, KC_NO, KC_RGUI, KC_RALT, KC_RCTL, KC_RSFT, KC_NO, KC_D, KC_1, KC_2, KC_3, KC_X, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_TRNS),
	[4] = LAYOUT(KC_BRID, KC_BRIU, KC_VOLD, KC_VOLU, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, LCTL(KC_EJCT), KC_F1, KC_F2, KC_F3, KC_F4, KC_MPLY, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, C(G(KC_Q)), KC_F5, KC_F6, KC_F7, KC_F8, KC_MUTE, KC_NO, KC_NO, KC_RGUI, KC_RALT, KC_RCTL, KC_RSFT, KC_NO, KC_F9, KC_F10, KC_F11, KC_F12, KC_MPRV, KC_MNXT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_TRNS),
	[5] = LAYOUT(KC_ESC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, SGUI(KC_Z), LGUI(KC_X), LGUI(KC_C), LGUI(KC_V), LGUI(KC_L), KC_DEL, KC_TAB, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, KC_NO, LGUI(KC_Z), KC_LEFT, KC_UP, KC_DOWN, KC_RGHT, KC_BSPC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_HOME, KC_PGUP, KC_PGDN, KC_END, KC_ENT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_NO),
	[6] = LAYOUT(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_MS_L, KC_MS_U, KC_MS_D, KC_MS_R, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO),
	[7] = LAYOUT(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),
	[8] = LAYOUT(KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_GRV, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TAB, KC_A, KC_S, KC_D, KC_F, KC_G, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_LALT, KC_LGUI, KC_SPC, KC_NO, KC_NO, KC_NO, KC_NO)
};


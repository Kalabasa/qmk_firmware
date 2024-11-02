/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// #define MASTER_LEFT
#define MASTER_RIGHT
#define SPLIT_LAYER_STATE_ENABLE
// #define SPLIT_LED_STATE_ENABLE
#define SPLIT_MODS_ENABLE

#define UNICODE_SELECTED_MODES UC_LNX

#define TAPPING_TERM 200
#define TAPPING_TERM_PER_KEY
// TAPPING_FORCE_HOLD feature will be replaced by QUICK_TAP_TERM. https://docs.qmk.fm/ChangeLog/20230226#quick-tap-term
#define TAPPING_FORCE_HOLD_PER_KEY

#define PERMISSIVE_HOLD
#define IGNORE_MOD_TAP_INTERRUPT

#define OLED_TIMEOUT 10000
#define OLED_FADE_OUT
#define OLED_FADE_OUT_INTERVAL 15
#define OLED_BRIGHTNESS 64
#define OLED_COLUMN_OFFSET 2

#undef OLED_FONT_H
#define OLED_FONT_H "keymaps/Kalabasa/font.c"

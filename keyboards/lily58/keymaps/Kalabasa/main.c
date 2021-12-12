#include QMK_KEYBOARD_H

// TODO
// * Swap Brightness keys
// * Easier Ctrl+Tab

//SSD1306 OLED update loop, make sure to enable OLED_ENABLE=yes in rules.mk
#ifdef OLED_ENABLE

// oled_rotation_t oled_init_user(oled_rotation_t rotation) {
//   if (is_keyboard_left())
//     return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
//   return rotation;
// }

void oled_task_user(void) {
  oled_write_ln("Hello World!", false);
}
#endif // OLED_ENABLE

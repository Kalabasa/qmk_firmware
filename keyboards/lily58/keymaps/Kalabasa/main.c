#include QMK_KEYBOARD_H

//SSD1306 OLED update loop, make sure to enable OLED_ENABLE=yes in rules.mk
#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  return OLED_ROTATION_270;
}

void oled_task_user(void) {
  oled_write_ln("Hello World!", false);
}
#endif // OLED_ENABLE

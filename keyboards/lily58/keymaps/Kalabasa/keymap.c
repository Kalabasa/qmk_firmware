#include QMK_KEYBOARD_H
#include "./keymap.generated.c";

// TODO
// * Swap Brightness keys
// * Fix OLED
// * Easier Ctrl+Tab

//SSD1306 OLED update loop, make sure to enable OLED_ENABLE=yes in rules.mk
#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (is_keyboard_left())
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  return rotation;
}

// When you add source files to SRC in rules.mk, you can use functions.
// const char *read_layer_state(void);
// const char *read_logo(void);
// void set_keylog(uint16_t keycode, keyrecord_t *record);
// const char *read_keylog(void);
// const char *read_keylogs(void);

// const char *read_mode_icon(bool swap);
// const char *read_host_led_state(void);
// void set_timelog(void);
// const char *read_timelog(void);

void oled_task_user(void) {
  oled_write_ln("Hello World!", false);
  // if (is_keyboard_left()) {
  //   // If you want to change the display of OLED, you need to change here
  //   // oled_write(read_logo(), false);
  //   // oled_write_ln(read_layer_state(), false);
  //   // oled_write_ln(read_keylog(), false);
  //   // oled_write_ln(read_keylogs(), false);
  //   // oled_write_ln(read_mode_icon(keymap_config.swap_lalt_lgui), false);
  //   //oled_write_ln(read_host_led_state(), false);
  //   //oled_write_ln(read_timelog(), false);
  // } else {
  //   // oled_write(read_logo(), false);
  //   // oled_write_ln(read_layer_state(), false);
  // }
}
#endif // OLED_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//   if (record->event.pressed) {
//     #ifdef OLED_ENABLE
//     set_keylog(keycode, record);
//     #endif
//     set_timelog();
//   }
  return true;
}

# Build Options
#   change to "no" to disable the options, or define them in the Makefile in
#   the appropriate keymap folder that will get included automatically
#
BOOTMAGIC_ENABLE = no       # Enable Bootmagic Lite
MOUSEKEY_ENABLE = no        # Mouse keys
EXTRAKEY_ENABLE = yes        # Audio control and System control
SWAP_HANDS_ENABLE = yes      # Enable one-hand typing
OLED_ENABLE = yes     # OLED display
KEY_OVERRIDE_ENABLE = no # redundant - features/custom_shift_keys
UNICODEMAP_ENABLE = yes

# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no    # Breathing sleep LED during USB suspend

SRC += main.c swap.c features/custom_shift_keys.c features/layer_lock.c features/bitwise_f.c

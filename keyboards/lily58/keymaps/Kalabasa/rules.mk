# Build Options
#   change to "no" to disable the options, or define them in the Makefile in
#   the appropriate keymap folder that will get included automatically
#
BOOTMAGIC_ENABLE = no       # Enable Bootmagic Lite
MOUSEKEY_ENABLE = no        # Mouse keys
EXTRAKEY_ENABLE = yes        # Audio control and System control
SWAP_HANDS_ENABLE = no      # Nice to have, but I don't use it
OLED_ENABLE = yes     # OLED display
UNICODEMAP_ENABLE = yes
OS_DETECTION_ENABLE = yes
LAYER_LOCK_ENABLE = yes

# size optimizations
LTO_ENABLE = yes
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
AVR_USE_MINIMAL_PRINTF = yes
MUSIC_ENABLE = no

# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no    # Breathing sleep LED during USB suspend

SRC += main.c features/bitwise_f.c

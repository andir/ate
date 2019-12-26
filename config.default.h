#ifndef BLACK
#define BLACK "#1c1b19"
#endif
#ifndef RED
#define RED "#ef2f27"
#endif
#ifndef GREEN
#define GREEN "#519f50"
#endif
#ifndef YELLOW
#define YELLOW "#fbb829"
#endif
#ifndef BLUE
#define BLUE "#2c78bf"
#endif
#ifndef MAGENTA
#define MAGENTA "#e02c6d"
#endif
#ifndef CYAN
#define CYAN "#0aaeb3"
#endif
#ifndef WHITE
#define WHITE "#918175"
#endif

#ifndef BRIGHT_BLACK
#define BRIGHT_BLACK "#2d2c29"
#endif
#ifndef BRIGHT_RED
#define BRIGHT_RED "#f75341"
#endif
#ifndef BRIGHT_GREEN
#define BRIGHT_GREEN "#98bc37"
#endif
#ifndef BRIGHT_YELLOW
#define BRIGHT_YELLOW "#fed06e"
#endif
#ifndef BRIGHT_BLUE
#define BRIGHT_BLUE "#68a8e4"
#endif
#ifndef BRIGHT_MAGENTA
#define BRIGHT_MAGENTA "#ff5c8f"
#endif
#ifndef BRIGHT_CYAN
#define BRIGHT_CYAN "#53fde9"
#endif
#ifndef BRIGHT_WHITE
#define BRIGHT_WHITE "#fce8c3"
#endif

#ifndef BACKGROUND_COLOR
#define BACKGROUND_COLOR "#1c1b19"
#endif
#ifndef FOREGROUND_COLOR
#define FOREGROUND_COLOR "#fce8c3"
#endif

#ifndef BACKGROUND_OPACITY
#define BACKGROUND_OPACITY 0.8
#endif

// The values hereafter should be set via config.ate.keybindings (see
// default.nix)

#ifndef RESET_FONT_KEYVAL
#define RESET_FONT_KEYVAL "0"
#endif
#ifndef RESET_FONT_MODIFIER_MASK
#define RESET_FONT_MODIFIER_MASK GDK_CONTROL_MASK
#endif

#ifndef INCREMENT_FONT_KEYVAL
#define INCREMENT_FONT_KEYVAL "plus"
#endif
#ifndef INCREMENT_FONT_MODIFIER_MASK
#define INCREMENT_FONT_MODIFIER_MASK GDK_CONTROL_MASK | GDK_SHIFT_MASK
#endif

#ifndef DECREMENT_FONT_KEYVAL
#define DECREMENT_FONT_KEYVAL "underscore"
#endif
#ifndef DECREMENT_FONT_MODIFIER_MASK
#define DECREMENT_FONT_MODIFIER_MASK GDK_CONTROL_MASK | GDK_SHIFT_MASK
#endif

#ifndef PASTE_PRIMARY_KEYVAL
#define PASTE_PRIMARY_KEYVAL "y"
#endif
#ifndef PASTE_PRIMARY_MODIFIER_MASK
#define PASTE_PRIMARY_MODIFIER_MASK GDK_MOD1_MASK | GDK_SHIFT_MASK
#endif

#ifndef PASTE_CLIPBOARD_KEYVAL
#define PASTE_CLIPBOARD_KEYVAL "y"
#endif
#ifndef PASTE_CLIPBOARD_MODIFIER_MASK
#define PASTE_CLIPBOARD_MODIFIER_MASK GDK_CONTROL_MASK | GDK_SHIFT_MASK
#endif

#ifndef PIPECMD_KEYVAL
#define PIPECMD_KEYVAL "u"
#endif
#ifndef PIPECMD_MODIFIER_MASK
#define PIPECMD_MODIFIER_MASK GDK_MOD1_MASK | GDK_SHIFT_MASK
#endif

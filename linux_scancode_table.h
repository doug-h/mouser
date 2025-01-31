#pragma once
// The original table is in the SDL2 source code.
// This is the reverse mapping

/* clang-format off */
#define SCANCODE_SDL_TO_LINUX(X) \
       X( SDL_SCANCODE_ESCAPE,             KEY_ESC )\
       X( SDL_SCANCODE_1,                  KEY_1)\
       X( SDL_SCANCODE_2,                  KEY_2)\
       X( SDL_SCANCODE_3,                  KEY_3)\
       X( SDL_SCANCODE_4,                  KEY_4)\
       X( SDL_SCANCODE_5,                  KEY_5)\
       X( SDL_SCANCODE_6,                  KEY_6)\
       X( SDL_SCANCODE_7,                  KEY_7)\
       X( SDL_SCANCODE_8,                  KEY_8)\
       X( SDL_SCANCODE_9,                  KEY_9)\
       X( SDL_SCANCODE_0,                  KEY_0)\
       X( SDL_SCANCODE_MINUS,              KEY_MINUS)\
       X( SDL_SCANCODE_EQUALS,             KEY_EQUAL)\
       X( SDL_SCANCODE_BACKSPACE,          KEY_BACKSPACE)\
       X( SDL_SCANCODE_TAB,                KEY_TAB)\
       X( SDL_SCANCODE_Q,                  KEY_Q)\
       X( SDL_SCANCODE_W,                  KEY_W)\
       X( SDL_SCANCODE_E,                  KEY_E)\
       X( SDL_SCANCODE_R,                  KEY_R)\
       X( SDL_SCANCODE_T,                  KEY_T)\
       X( SDL_SCANCODE_Y,                  KEY_Y)\
       X( SDL_SCANCODE_U,                  KEY_U)\
       X( SDL_SCANCODE_I,                  KEY_I)\
       X( SDL_SCANCODE_O,                  KEY_O)\
       X( SDL_SCANCODE_P,                  KEY_P)\
       X( SDL_SCANCODE_LEFTBRACKET,        KEY_LEFTBRACE)\
       X( SDL_SCANCODE_RIGHTBRACKET,       KEY_RIGHTBRACE)\
       X( SDL_SCANCODE_RETURN,             KEY_ENTER)\
       X( SDL_SCANCODE_LCTRL,              KEY_LEFTCTRL)\
       X( SDL_SCANCODE_A,                  KEY_A)\
       X( SDL_SCANCODE_S,                  KEY_S)\
       X( SDL_SCANCODE_D,                  KEY_D)\
       X( SDL_SCANCODE_F,                  KEY_F)\
       X( SDL_SCANCODE_G,                  KEY_G)\
       X( SDL_SCANCODE_H,                  KEY_H)\
       X( SDL_SCANCODE_J,                  KEY_J)\
       X( SDL_SCANCODE_K,                  KEY_K)\
       X( SDL_SCANCODE_L,                  KEY_L)\
       X( SDL_SCANCODE_SEMICOLON,          KEY_SEMICOLON)\
       X( SDL_SCANCODE_APOSTROPHE,         KEY_APOSTROPHE)\
       X( SDL_SCANCODE_GRAVE,              KEY_GRAVE)\
       X( SDL_SCANCODE_LSHIFT,             KEY_LEFTSHIFT)\
       X( SDL_SCANCODE_BACKSLASH,          KEY_BACKSLASH)\
       X( SDL_SCANCODE_Z,                  KEY_Z)\
       X( SDL_SCANCODE_X,                  KEY_X)\
       X( SDL_SCANCODE_C,                  KEY_C)\
       X( SDL_SCANCODE_V,                  KEY_V)\
       X( SDL_SCANCODE_B,                  KEY_B)\
       X( SDL_SCANCODE_N,                  KEY_N)\
       X( SDL_SCANCODE_M,                  KEY_M)\
       X( SDL_SCANCODE_COMMA,              KEY_COMMA)\
       X( SDL_SCANCODE_PERIOD,             KEY_DOT)\
       X( SDL_SCANCODE_SLASH,              KEY_SLASH)\
       X( SDL_SCANCODE_RSHIFT,             KEY_RIGHTSHIFT)\
       X( SDL_SCANCODE_KP_MULTIPLY,        KEY_KPASTERISK)\
       X( SDL_SCANCODE_LALT,               KEY_LEFTALT)\
       X( SDL_SCANCODE_SPACE,              KEY_SPACE)\
       X( SDL_SCANCODE_CAPSLOCK,           KEY_CAPSLOCK)\
       X( SDL_SCANCODE_F1,                 KEY_F1)\
       X( SDL_SCANCODE_F2,                 KEY_F2)\
       X( SDL_SCANCODE_F3,                 KEY_F3)\
       X( SDL_SCANCODE_F4,                 KEY_F4)\
       X( SDL_SCANCODE_F5,                 KEY_F5)\
       X( SDL_SCANCODE_F6,                 KEY_F6)\
       X( SDL_SCANCODE_F7,                 KEY_F7)\
       X( SDL_SCANCODE_F8,                 KEY_F8)\
       X( SDL_SCANCODE_F9,                 KEY_F9)\
       X( SDL_SCANCODE_F10,                KEY_F10)\
       X( SDL_SCANCODE_NUMLOCKCLEAR,       KEY_NUMLOCK)\
       X( SDL_SCANCODE_SCROLLLOCK,         KEY_SCROLLLOCK)\
       X( SDL_SCANCODE_KP_7,               KEY_KP7)\
       X( SDL_SCANCODE_KP_8,               KEY_KP8)\
       X( SDL_SCANCODE_KP_9,               KEY_KP9)\
       X( SDL_SCANCODE_KP_MINUS,           KEY_KPMINUS)\
       X( SDL_SCANCODE_KP_4,               KEY_KP4)\
       X( SDL_SCANCODE_KP_5,               KEY_KP5)\
       X( SDL_SCANCODE_KP_6,               KEY_KP6)\
       X( SDL_SCANCODE_KP_PLUS,            KEY_KPPLUS)\
       X( SDL_SCANCODE_KP_1,               KEY_KP1)\
       X( SDL_SCANCODE_KP_2,               KEY_KP2)\
       X( SDL_SCANCODE_KP_3,               KEY_KP3)\
       X( SDL_SCANCODE_KP_0,               KEY_KP0)\
       X( SDL_SCANCODE_KP_PERIOD,          KEY_KPDOT)\
       X( SDL_SCANCODE_LANG5,              KEY_ZENKAKUHANKAKU)\
       X( SDL_SCANCODE_NONUSBACKSLASH,     KEY_102ND)\
       X( SDL_SCANCODE_F11,                KEY_F11)\
       X( SDL_SCANCODE_F12,                KEY_F12)\
       X( SDL_SCANCODE_INTERNATIONAL1,     KEY_RO)\
       X( SDL_SCANCODE_LANG3,              KEY_KATAKANA)\
       X( SDL_SCANCODE_LANG4,              KEY_HIRAGANA)\
       X( SDL_SCANCODE_INTERNATIONAL4,     KEY_HENKAN)\
       X( SDL_SCANCODE_INTERNATIONAL2,     KEY_KATAKANAHIRAGANA)\
       /* TODO - check what this does */ \
       /*X( SDL_SCANCODE_INTERNATIONAL5,     KEY_MUHENKAN)\ */ \
       X( SDL_SCANCODE_INTERNATIONAL5,     KEY_KPJPCOMMA)\
       X( SDL_SCANCODE_KP_ENTER,           KEY_KPENTER)\
       X( SDL_SCANCODE_RCTRL,              KEY_RIGHTCTRL)\
       X( SDL_SCANCODE_KP_DIVIDE,          KEY_KPSLASH)\
       X( SDL_SCANCODE_SYSREQ,             KEY_SYSRQ)\
       X( SDL_SCANCODE_RALT,               KEY_RIGHTALT)\
       X( SDL_SCANCODE_HOME,               KEY_HOME)\
       X( SDL_SCANCODE_UP,                 KEY_UP)\
       X( SDL_SCANCODE_PAGEUP,             KEY_PAGEUP)\
       X( SDL_SCANCODE_LEFT,               KEY_LEFT)\
       X( SDL_SCANCODE_RIGHT,              KEY_RIGHT)\
       X( SDL_SCANCODE_END,                KEY_END)\
       X( SDL_SCANCODE_DOWN,               KEY_DOWN)\
       X( SDL_SCANCODE_PAGEDOWN,           KEY_PAGEDOWN)\
       X( SDL_SCANCODE_INSERT,             KEY_INSERT)\
       X( SDL_SCANCODE_DELETE,             KEY_DELETE)\
       X( SDL_SCANCODE_MUTE,               KEY_MUTE)\
       X( SDL_SCANCODE_VOLUMEDOWN,         KEY_VOLUMEDOWN)\
       X( SDL_SCANCODE_VOLUMEUP,           KEY_VOLUMEUP)\
       X( SDL_SCANCODE_POWER,              KEY_POWER)\
       X( SDL_SCANCODE_KP_EQUALS,          KEY_KPEQUAL)\
       X( SDL_SCANCODE_KP_PLUSMINUS,       KEY_KPPLUSMINUS)\
       X( SDL_SCANCODE_PAUSE,              KEY_PAUSE)\
       X( SDL_SCANCODE_KP_COMMA,           KEY_KPCOMMA)\
       X( SDL_SCANCODE_LANG1,              KEY_HANGEUL)\
       X( SDL_SCANCODE_LANG2,              KEY_HANJA)\
       X( SDL_SCANCODE_INTERNATIONAL3,     KEY_YEN)\
       X( SDL_SCANCODE_LGUI,               KEY_LEFTMETA)\
       X( SDL_SCANCODE_RGUI,               KEY_RIGHTMETA)\
       X( SDL_SCANCODE_APPLICATION,        KEY_COMPOSE)\
       X( SDL_SCANCODE_STOP,               KEY_STOP)\
       X( SDL_SCANCODE_AGAIN,              KEY_AGAIN)\
       X( SDL_SCANCODE_UNDO,               KEY_UNDO)\
       X( SDL_SCANCODE_COPY,               KEY_COPY)\
       X( SDL_SCANCODE_PASTE,              KEY_PASTE)\
       X( SDL_SCANCODE_FIND,               KEY_FIND)\
       X( SDL_SCANCODE_CUT,                KEY_CUT)\
       X( SDL_SCANCODE_HELP,               KEY_HELP)\
       X( SDL_SCANCODE_MENU,               KEY_MENU)\
       X( SDL_SCANCODE_CALCULATOR,         KEY_CALC)\
       X( SDL_SCANCODE_SLEEP,              KEY_SLEEP)\
       X( SDL_SCANCODE_APP1,               KEY_PROG1)\
       X( SDL_SCANCODE_APP2,               KEY_PROG2)\
       X( SDL_SCANCODE_WWW,                KEY_WWW)\
       X( SDL_SCANCODE_MAIL,               KEY_MAIL)\
       X( SDL_SCANCODE_AC_BOOKMARKS,       KEY_BOOKMARKS)\
       X( SDL_SCANCODE_COMPUTER,           KEY_COMPUTER)\
       X( SDL_SCANCODE_AC_BACK,            KEY_BACK)\
       X( SDL_SCANCODE_AC_FORWARD,         KEY_FORWARD)\
       X( SDL_SCANCODE_EJECT,              KEY_EJECTCD)\
       /* TODO - check what this does */ \
       /* X( SDL_SCANCODE_EJECT,              KEY_EJECTCLOSECD)*/ \
       X( SDL_SCANCODE_AUDIONEXT,          KEY_NEXTSONG)\
       X( SDL_SCANCODE_AUDIOPLAY,          KEY_PLAYPAUSE)\
       X( SDL_SCANCODE_AUDIOPREV,          KEY_PREVIOUSSONG)\
       X( SDL_SCANCODE_AUDIOSTOP,          KEY_STOPCD)\
       X( SDL_SCANCODE_AUDIOREWIND,        KEY_REWIND)\
       X( SDL_SCANCODE_AC_HOME,            KEY_HOMEPAGE)\
       X( SDL_SCANCODE_AC_REFRESH,         KEY_REFRESH)\
       X( SDL_SCANCODE_KP_LEFTPAREN,       KEY_KPLEFTPAREN)\
       X( SDL_SCANCODE_KP_RIGHTPAREN,      KEY_KPRIGHTPAREN)\
       /* TODO - check what this does */ \
       /* X( SDL_SCANCODE_AGAIN,              KEY_REDO)\ */ \
       X( SDL_SCANCODE_F13,                KEY_F13)\
       X( SDL_SCANCODE_F14,                KEY_F14)\
       X( SDL_SCANCODE_F15,                KEY_F15)\
       X( SDL_SCANCODE_F16,                KEY_F16)\
       X( SDL_SCANCODE_F17,                KEY_F17)\
       X( SDL_SCANCODE_F18,                KEY_F18)\
       X( SDL_SCANCODE_F19,                KEY_F19)\
       X( SDL_SCANCODE_F20,                KEY_F20)\
       X( SDL_SCANCODE_F21,                KEY_F21)\
       X( SDL_SCANCODE_F22,                KEY_F22)\
       X( SDL_SCANCODE_F23,                KEY_F23)\
       X( SDL_SCANCODE_F24,                KEY_F24)\
        /* TODO - check what this does */ \
       /*X( SDL_SCANCODE_AUDIOPLAY,          KEY_PLAYCD)\ */\
       /* TODO - check what this does */ \
       /*X( SDL_SCANCODE_AUDIOPLAY,          KEY_PLAY)\*/\
       X( SDL_SCANCODE_AUDIOFASTFORWARD,   KEY_FASTFORWARD)\
       X( SDL_SCANCODE_PRINTSCREEN,        KEY_PRINT)\
       /* TODO - check what this does */ \
       /*X( SDL_SCANCODE_MAIL,               KEY_EMAIL)\ */\
       X( SDL_SCANCODE_AC_SEARCH,          KEY_SEARCH)\
       X( SDL_SCANCODE_ALTERASE,           KEY_ALTERASE)\
       X( SDL_SCANCODE_CANCEL,             KEY_CANCEL)\
       X( SDL_SCANCODE_BRIGHTNESSDOWN,     KEY_BRIGHTNESSDOWN)\
       X( SDL_SCANCODE_BRIGHTNESSUP,       KEY_BRIGHTNESSUP)\
       X( SDL_SCANCODE_MEDIASELECT,        KEY_MEDIA)\
       X( SDL_SCANCODE_DISPLAYSWITCH,      KEY_SWITCHVIDEOMODE)\
       X( SDL_SCANCODE_KBDILLUMTOGGLE,     KEY_KBDILLUMTOGGLE)\
       X( SDL_SCANCODE_KBDILLUMDOWN,       KEY_KBDILLUMDOWN)\
       X( SDL_SCANCODE_KBDILLUMUP,         KEY_KBDILLUMUP)
/* clang-format on */

void FillTable(uint8_t *table)
{
#define FILL_TABLE(SDL_CODE, LINUX_CODE) table[SDL_CODE] = LINUX_CODE;

  SCANCODE_SDL_TO_LINUX(FILL_TABLE);
#undef FILL_TABLE
}

int GetLinuxScancode(SDL_Scancode s)
{
  static uint8_t sdl_linux_scancode_table[SDL_NUM_SCANCODES];
  static bool table_initialised;

  if (!table_initialised) {
    FillTable(sdl_linux_scancode_table);
    table_initialised = true;
  }

  return sdl_linux_scancode_table[s];
}
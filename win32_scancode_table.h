#pragma once

// The original table is in the SDL2 source code.
// This is the reverse mapping

#define SCANCODE_SDL_TO_WINDOWS(X)                                             \
  X(SDL_SCANCODE_ESCAPE, 1)                                                    \
  X(SDL_SCANCODE_1, 2)                                                         \
  X(SDL_SCANCODE_2, 3)                                                         \
  X(SDL_SCANCODE_3, 4)                                                         \
  X(SDL_SCANCODE_4, 5)                                                         \
  X(SDL_SCANCODE_5, 6)                                                         \
  X(SDL_SCANCODE_6, 7)                                                         \
  X(SDL_SCANCODE_7, 8)                                                         \
  X(SDL_SCANCODE_8, 9)                                                         \
  X(SDL_SCANCODE_9, 10)                                                        \
  X(SDL_SCANCODE_0, 11)                                                        \
  X(SDL_SCANCODE_MINUS, 12)                                                    \
  X(SDL_SCANCODE_EQUALS, 13)                                                   \
  X(SDL_SCANCODE_BACKSPACE, 14)                                                \
  X(SDL_SCANCODE_TAB, 15)                                                      \
  X(SDL_SCANCODE_Q, 16)                                                        \
  X(SDL_SCANCODE_W, 17)                                                        \
  X(SDL_SCANCODE_E, 18)                                                        \
  X(SDL_SCANCODE_R, 19)                                                        \
  X(SDL_SCANCODE_T, 20)                                                        \
  X(SDL_SCANCODE_Y, 21)                                                        \
  X(SDL_SCANCODE_U, 22)                                                        \
  X(SDL_SCANCODE_I, 23)                                                        \
  X(SDL_SCANCODE_O, 24)                                                        \
  X(SDL_SCANCODE_P, 25)                                                        \
  X(SDL_SCANCODE_LEFTBRACKET, 26)                                              \
  X(SDL_SCANCODE_RIGHTBRACKET, 27)                                             \
  X(SDL_SCANCODE_RETURN, 28)                                                   \
  X(SDL_SCANCODE_LCTRL, 29)                                                    \
  X(SDL_SCANCODE_A, 30)                                                        \
  X(SDL_SCANCODE_S, 31)                                                        \
  X(SDL_SCANCODE_D, 32)                                                        \
  X(SDL_SCANCODE_F, 33)                                                        \
  X(SDL_SCANCODE_G, 34)                                                        \
  X(SDL_SCANCODE_H, 35)                                                        \
  X(SDL_SCANCODE_J, 36)                                                        \
  X(SDL_SCANCODE_K, 37)                                                        \
  X(SDL_SCANCODE_L, 38)                                                        \
  X(SDL_SCANCODE_SEMICOLON, 39)                                                \
  X(SDL_SCANCODE_APOSTROPHE, 40)                                               \
  X(SDL_SCANCODE_GRAVE, 41)                                                    \
  X(SDL_SCANCODE_LSHIFT, 42)                                                   \
  X(SDL_SCANCODE_BACKSLASH, 43)                                                \
  X(SDL_SCANCODE_Z, 44)                                                        \
  X(SDL_SCANCODE_X, 45)                                                        \
  X(SDL_SCANCODE_C, 46)                                                        \
  X(SDL_SCANCODE_V, 47)                                                        \
  X(SDL_SCANCODE_B, 48)                                                        \
  X(SDL_SCANCODE_N, 49)                                                        \
  X(SDL_SCANCODE_M, 50)                                                        \
  X(SDL_SCANCODE_COMMA, 51)                                                    \
  X(SDL_SCANCODE_PERIOD, 52)                                                   \
  X(SDL_SCANCODE_SLASH, 53)                                                    \
  X(SDL_SCANCODE_RSHIFT, 54)                                                   \
  X(SDL_SCANCODE_PRINTSCREEN, 55)                                              \
  X(SDL_SCANCODE_LALT, 56)                                                     \
  X(SDL_SCANCODE_SPACE, 57)                                                    \
  X(SDL_SCANCODE_CAPSLOCK, 58)                                                 \
  X(SDL_SCANCODE_F1, 59)                                                       \
  X(SDL_SCANCODE_F2, 60)                                                       \
  X(SDL_SCANCODE_F3, 61)                                                       \
  X(SDL_SCANCODE_F4, 62)                                                       \
  X(SDL_SCANCODE_F5, 63)                                                       \
  X(SDL_SCANCODE_F6, 64)                                                       \
  X(SDL_SCANCODE_F7, 65)                                                       \
  X(SDL_SCANCODE_F8, 66)                                                       \
  X(SDL_SCANCODE_F9, 67)                                                       \
  X(SDL_SCANCODE_F10, 68)                                                      \
  X(SDL_SCANCODE_NUMLOCKCLEAR, 69)                                             \
  X(SDL_SCANCODE_SCROLLLOCK, 70)                                               \
  X(SDL_SCANCODE_HOME, 71)                                                     \
  X(SDL_SCANCODE_UP, 72)                                                       \
  X(SDL_SCANCODE_PAGEUP, 73)                                                   \
  X(SDL_SCANCODE_KP_MINUS, 74)                                                 \
  X(SDL_SCANCODE_LEFT, 75)                                                     \
  X(SDL_SCANCODE_KP_5, 76)                                                     \
  X(SDL_SCANCODE_RIGHT, 77)                                                    \
  X(SDL_SCANCODE_KP_PLUS, 78)                                                  \
  X(SDL_SCANCODE_END, 79)                                                      \
  X(SDL_SCANCODE_DOWN, 80)                                                     \
  X(SDL_SCANCODE_PAGEDOWN, 81)                                                 \
  X(SDL_SCANCODE_INSERT, 82)                                                   \
  X(SDL_SCANCODE_DELETE, 83)                                                   \
  X(SDL_SCANCODE_NONUSBACKSLASH, 86)                                           \
  X(SDL_SCANCODE_F11, 87)                                                      \
  X(SDL_SCANCODE_F12, 88)                                                      \
  X(SDL_SCANCODE_PAUSE, 89)                                                    \
  X(SDL_SCANCODE_LGUI, 91)                                                     \
  X(SDL_SCANCODE_RGUI, 92)                                                     \
  X(SDL_SCANCODE_APPLICATION, 93)                                              \
  X(SDL_SCANCODE_F13, 100)                                                     \
  X(SDL_SCANCODE_F14, 101)                                                     \
  X(SDL_SCANCODE_F15, 102)                                                     \
  X(SDL_SCANCODE_F16, 103)                                                     \
  X(SDL_SCANCODE_F17, 104)                                                     \
  X(SDL_SCANCODE_F18, 105)                                                     \
  X(SDL_SCANCODE_F19, 106)                                                     \
  X(SDL_SCANCODE_INTERNATIONAL2, 112)                                          \
  X(SDL_SCANCODE_INTERNATIONAL1, 115)                                          \
  X(SDL_SCANCODE_INTERNATIONAL4, 121)                                          \
  X(SDL_SCANCODE_INTERNATIONAL5, 123)                                          \
  X(SDL_SCANCODE_INTERNATIONAL3, 125)


// We use virtual_key == 0 to mean no match
WORD GetWindowsVKey(SDL_Scancode s)
{
  static uint8_t sdl_windows_scancode_table[SDL_NUM_SCANCODES];
  static bool table_initialised;

  if (!table_initialised) {

#define FILL_TABLE(SDL_CODE, WIN_CODE)                                         \
  sdl_windows_scancode_table[SDL_CODE] = WIN_CODE;
    SCANCODE_SDL_TO_WINDOWS(FILL_TABLE);
#undef FILL_TABLE

    table_initialised = true;
  }

  UINT win_scancode = sdl_windows_scancode_table[s];
  if (win_scancode) {
    return (WORD)MapVirtualKeyA(win_scancode, MAPVK_VSC_TO_VK);
  }
  return 0;
}
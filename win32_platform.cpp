#include "Windows.h"
/* Mixing SDL and win32 api bad idea,
 * for now make sure win32 included before SDL. */
//
#include "platform.h"
#include "win32_scancode_table.h"

void SetMouse(const MouseData& md) {
  SetCursorPos(md.x, md.y);

  INPUT mouse_buttons[3];
  ZeroMemory(mouse_buttons, sizeof(mouse_buttons));

  mouse_buttons[0].type = INPUT_MOUSE;
  mouse_buttons[1].type = INPUT_MOUSE;
  mouse_buttons[2].type = INPUT_MOUSE;

  mouse_buttons[0].mi.dwFlags =
      (md.LClicked()) ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
  mouse_buttons[1].mi.dwFlags =
      (md.MClicked()) ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
  mouse_buttons[2].mi.dwFlags =
      (md.RClicked()) ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;

  SendInput(3, mouse_buttons, sizeof(INPUT));
}

// We use virtual_key == 0 to mean no match (I don't think anything is
// actually mapped to that value)
UINT GetWindowsVKey(SDL_Scancode s) {
  static bool table_initialised;
  if (!table_initialised) {
    PopulateScancodeTable();
    table_initialised = true;
  }

  UINT win_scancode = sdl_windows_scancode_table[s];
  if (win_scancode) {
    return MapVirtualKeyA(win_scancode, MAPVK_VSC_TO_VK);
  }
  return 0;
}

void SetKeys(const KeyData& kd) {
  uint8_t win_keystate[256];
  GetKeyboardState(win_keystate);
  bool any_changed = false;

  for (size_t byte = 0; byte < sizeof(kd.codes); ++byte) {
    if (kd.codes[byte]) {
      uint8_t bits = kd.codes[byte];
      for (int bit = 0; bit < 8; ++bit) {
        if (bits & (1 << bit)) {
          SDL_Scancode s = (SDL_Scancode)(MIN_SCANCODE + 8 * byte + bit);
          UINT v_key = GetWindowsVKey(s);
          if (v_key) {
            std::cout << "Key pressed " << v_key << '\n';
            // TODO - toggles need special treatment
            win_keystate[v_key] |= 128;
            any_changed = true;
          }
        }
      }
    }
  }

  if (any_changed) SetKeyboardState(win_keystate);
}


#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#undef WIN32_LEAN_AND_MEAN
/* Mixing SDL and win32 headers bad idea as SDL has its own windows includes,
 * for now make sure win32 included before anything from SDL. */

#include "input.h"
#include "platform.h"
#include "win32_scancode_table.h"


namespace Platform {
void SetMouse(const MouseData &md)
{
  SetCursorPos(md.x, md.y);

  bool L_button_down = GetKeyState(VK_LBUTTON) & 128;
  bool M_button_down = GetKeyState(VK_MBUTTON) & 128;
  bool R_button_down = GetKeyState(VK_RBUTTON) & 128;

  INPUT mouse_buttons[3];
  memset(mouse_buttons, 0, 3 * sizeof(INPUT));
  mouse_buttons[0].type = INPUT_MOUSE;
  mouse_buttons[1].type = INPUT_MOUSE;
  mouse_buttons[2].type = INPUT_MOUSE;

  int n_buttons_updated = 0;

  if (L_button_down != IsButtonPressed(md, SDL_BUTTON_LEFT)) {
    mouse_buttons[n_buttons_updated++].mi.dwFlags =
        L_button_down ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_LEFTDOWN;
  }
  if (M_button_down != IsButtonPressed(md, SDL_BUTTON_MIDDLE)) {
    mouse_buttons[n_buttons_updated++].mi.dwFlags =
        M_button_down ? MOUSEEVENTF_MIDDLEUP : MOUSEEVENTF_MIDDLEDOWN;
  }
  if (R_button_down != IsButtonPressed(md, SDL_BUTTON_RIGHT)) {
    mouse_buttons[n_buttons_updated++].mi.dwFlags =
        R_button_down ? MOUSEEVENTF_RIGHTUP : MOUSEEVENTF_RIGHTDOWN;
  }

  if (n_buttons_updated)
    SendInput(n_buttons_updated, mouse_buttons, sizeof(INPUT));
}

void SetKeys(SDL_Scancode *keys_to_press, SDL_Scancode *keys_to_release,
             int num_to_press, int num_to_release)
{
  // We commit 'key_buffer_size' inputs at a time
  const int key_buffer_size = 16;
  INPUT key_input[key_buffer_size] = {0};

  do {
    int n = std::min(16, num_to_press);
    for (int i = 0; i < n; ++i) {
      key_input[i].type = INPUT_KEYBOARD;
      // TODO - try using windows scancodes here
      key_input[i].ki.wVk = GetWindowsVKey(keys_to_press[i]);
      key_input[i].ki.dwFlags = 0;
    }
    SendInput(n, key_input, sizeof(INPUT));

    num_to_press -= key_buffer_size;
    keys_to_press += key_buffer_size;
  } while (num_to_press > key_buffer_size);

  do {
    int n = std::min(16, num_to_release);
    for (int i = 0; i < n; ++i) {
      key_input[i].type = INPUT_KEYBOARD;
      // TODO - try using windows scancodes here
      key_input[i].ki.wVk = GetWindowsVKey(keys_to_release[i]);
      key_input[i].ki.dwFlags = KEYEVENTF_KEYUP;
    }
    SendInput(n, key_input, sizeof(INPUT));

    num_to_release -= key_buffer_size;
    keys_to_release += key_buffer_size;
  } while (num_to_release > key_buffer_size);
}
} // namespace Platform
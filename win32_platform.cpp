
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
  // Could combine some inputs...
  INPUT mouse_inputs;
  memset(&mouse_inputs, 0, sizeof(INPUT));
  mouse_inputs.type = INPUT_MOUSE;

  // We only send button inputs if the current state is wrong
  bool L_button_down = GetKeyState(VK_LBUTTON) & 128;
  bool M_button_down = GetKeyState(VK_MBUTTON) & 128;
  bool R_button_down = GetKeyState(VK_RBUTTON) & 128;
  if (L_button_down != IsButtonPressed(md, SDL_BUTTON_LEFT)) {
    mouse_inputs.mi.dwFlags |=
        L_button_down ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_LEFTDOWN;
  }
  if (M_button_down != IsButtonPressed(md, SDL_BUTTON_MIDDLE)) {
    mouse_inputs.mi.dwFlags |=
        M_button_down ? MOUSEEVENTF_MIDDLEUP : MOUSEEVENTF_MIDDLEDOWN;
  }
  if (R_button_down != IsButtonPressed(md, SDL_BUTTON_RIGHT)) {
    mouse_inputs.mi.dwFlags |=
        R_button_down ? MOUSEEVENTF_RIGHTUP : MOUSEEVENTF_RIGHTDOWN;
  }
  if (md.dx != 0 or md.dy != 0) {
    mouse_inputs.mi.dwFlags |= MOUSEEVENTF_MOVE;
    mouse_inputs.mi.dx = md.dx;
    mouse_inputs.mi.dy = md.dy;
  }

  if (md.scroll_amount != 0) {
    mouse_inputs.mi.dwFlags |= MOUSEEVENTF_WHEEL;
    mouse_inputs.mi.mouseData = md.scroll_amount * WHEEL_DELTA;
  }

  if (mouse_inputs.mi.dwFlags != 0) SendInput(1, &mouse_inputs, sizeof(INPUT));
}

void SetKeys(SDL_Scancode *keys_to_press, SDL_Scancode *keys_to_release,
             SDL_Scancode *keys_to_repeat, int num_to_press, int num_to_release,
             int num_to_repeat)
{
  // We commit 'key_buffer_size' inputs at a time
  const int key_buffer_size = 16;
  INPUT key_input[key_buffer_size] = {0};

  while (num_to_press > 0) {
    int n = std::min(key_buffer_size, num_to_press);
    for (int i = 0; i < n; ++i) {
      key_input[i].type = INPUT_KEYBOARD;
      // TODO - try using windows scancodes here
      key_input[i].ki.wVk = GetWindowsVKey(keys_to_press[i]);
      key_input[i].ki.dwFlags = 0;
    }
    SendInput(n, key_input, sizeof(INPUT));

    num_to_press -= n;
    keys_to_press += n;
  }

  while (num_to_release > 0) {
    int n = std::min(key_buffer_size, num_to_release);
    for (int i = 0; i < n; ++i) {
      key_input[i].type = INPUT_KEYBOARD;
      // TODO - try using windows scancodes here
      key_input[i].ki.wVk = GetWindowsVKey(keys_to_release[i]);
      key_input[i].ki.dwFlags = KEYEVENTF_KEYUP;
    }
    SendInput(n, key_input, sizeof(INPUT));

    num_to_release -= n;
    keys_to_release += n;
  }

  // Can't see how to get key repeating with SendInput, so we just unpress/press
  // NOTE - this will completely ignore OS key repeat settings
  while (num_to_release > 0) {
    int n = std::min(key_buffer_size / 2, num_to_repeat);
    for (int i = 0; i < n; ++i) {
      key_input[i].type = INPUT_KEYBOARD;
      key_input[i].ki.wVk = GetWindowsVKey(keys_to_repeat[i]);
      key_input[i].ki.dwFlags = KEYEVENTF_KEYUP;

      key_input[i + 8].type = INPUT_KEYBOARD;
      key_input[i + 8].ki.wVk = GetWindowsVKey(keys_to_repeat[i]);
      key_input[i + 8].ki.dwFlags = 0;
    }
    SendInput(n, key_input, sizeof(INPUT));

    num_to_repeat -= n;
    keys_to_repeat += n;
  }
}
} // namespace Platform

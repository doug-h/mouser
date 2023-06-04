#pragma once

#include <SDL2/SDL.h>

#include <cstdint>
#include <iostream>

// TODO - curate the SDL_Scancode list for keys we actually want to support
// (currently we just do them all from s=4 --> s=231)
const SDL_Scancode MIN_SCANCODE = SDL_SCANCODE_A;
const SDL_Scancode MAX_SCANCODE = SDL_SCANCODE_RGUI;
const size_t N_BYTES = (MAX_SCANCODE - MIN_SCANCODE + 1 + 7) / 8;
// NOTE - This will break if SDL changes their Scancode values between updates!
//  Since we are sending data between different devices we should make sure
//  client/server have compatible SDL2 versions

struct MouseData {
  uint16_t x, y;
  uint8_t button;
  int8_t scroll_amount;
};

// Keyboard state, each key's state is stored as a bit (pressed/not pressed)
struct KeyData {
  uint8_t packed_scancodes[N_BYTES];
};

// Retrieve the state of a button in MouseData
// SDL_button_id is one of SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, SDL_BUTTON_MIDDLE
inline bool IsButtonPressed(const MouseData &md, int SDL_button_id) {
  return SDL_BUTTON(SDL_button_id) & md.button;
}

inline void SetScancode(KeyData &kd, SDL_Scancode s, bool pressed) {
  size_t index = s - MIN_SCANCODE;
  size_t byte_index = index / 8;
  size_t bit = index % 8;

  unsigned char value = pressed;
  kd.packed_scancodes[byte_index] =
      (uint8_t)(kd.packed_scancodes[byte_index] & ~(1 << bit)) | (value << bit);
}

// Pretty-print for debugging
inline std::ostream &operator<<(std::ostream &os, const MouseData &md) {
  os << '(' << md.x << ", " << md.y << ')';  // (x,y)
  os << '[' << (IsButtonPressed(md, SDL_BUTTON_LEFT) ? 'X' : 'O') << ','
     << (IsButtonPressed(md, SDL_BUTTON_MIDDLE) ? 'X' : 'O') << ','
     << (IsButtonPressed(md, SDL_BUTTON_RIGHT) ? 'X' : 'O') << ']';  // [X,O,O]
  os << ((md.scroll_amount > 0)   ? '^'
         : (md.scroll_amount < 0) ? 'v'
                                  : 'o')
     << ", " << (int)md.scroll_amount;
  return os;
}

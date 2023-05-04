#pragma once

#include <SDL2/SDL.h>

// We just copy and send the entire key state every tick.
// Looping through the array every tick is pretty fast and the packet's <32B.
// There are many optimisations that could make this better ...
//  Could only send pressed keys.
//  Most lightweight would be to just send changes, would need to send total
// state every now and again to avoid keys getting stuck on if a packet is
// missed.
//  Could also use VLE since most common, rapidly pressed, keys are in a
// small range (a-z, 0-9), whereas all the weird stuff is rarely used.

// TODO - curate the SDL_Scancode list for keys we actually want to support
// (currently we just do them all from s=4->s=231)
const SDL_Scancode MIN_SCANCODE = SDL_SCANCODE_A;
const SDL_Scancode MAX_SCANCODE = SDL_SCANCODE_RGUI;

// Get next multiple of 8
const size_t N_BYTES = (MAX_SCANCODE - MIN_SCANCODE + 1 + 7) / 8;

struct KeyData {
  KeyData() : data{'k'} {}
  void Set(SDL_Scancode s, bool pressed) {
    size_t index = s - MIN_SCANCODE;
    size_t byte_index = index / 8;
    size_t bit = index % 8;

    unsigned char value = pressed;
    // The compiler might be smart enough to realise this will never overflow
    // and the char casts not needed...
    codes[byte_index] =
        (codes[byte_index] & ~((unsigned char)1 << bit)) | (value << bit);
  }

  union {
    struct {
      uint8_t header = 'k';
      uint8_t codes[N_BYTES];
    };
    uint8_t data[N_BYTES + 1];
  };
};

static_assert(sizeof(KeyData) == (N_BYTES + 1));

inline void UpdateKeyData(const Uint8* sdl_keystate, KeyData& kd) {
  for (SDL_Scancode s = MIN_SCANCODE; s < MAX_SCANCODE;
       s = (SDL_Scancode)(s + 1)) {
    kd.Set(s, sdl_keystate[s]);
  }
}
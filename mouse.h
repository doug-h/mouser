#ifndef MOUSER_MOUSE_H__
#define MOUSER_MOUSE_H__

#include <SDL2/SDL.h>

#include <iostream>

/*  Mouse pos is stored & sent as coord on a 1920x1080 screen, then scaled by
 * client.
 * TODO - query client what current screen size is, otherwise
 * non 16:9 screens will have different x/y sensitivity. */
const int SOURCE_WIDTH = 1920;
const int SOURCE_HEIGHT = 1080;

static_assert(sizeof(char) == 1);
static_assert(sizeof(short) == 2);
static_assert(sizeof(int) == 4);
struct MouseData {

  bool LClicked() const { return (b & SDL_BUTTON(SDL_BUTTON_LEFT)); }
  bool MClicked() const { return (b & SDL_BUTTON(SDL_BUTTON_MIDDLE)); }
  bool RClicked() const { return (b & SDL_BUTTON(SDL_BUTTON_RIGHT)); }

  union {
    struct {
      short x, y;
      char b;
      char _padding[3];
    };
    char data[8];
  };
};

std::ostream &operator<<(std::ostream &os, const MouseData &md) {
  os << '(' << md.x << ", " << md.y << ')';
  os << '[' << ((md.LClicked()) ? 'X' : 'O') << ','
     << ((md.MClicked()) ? 'X' : 'O') << ',' << ((md.RClicked()) ? 'X' : 'O')
     << ']';
  return os;
}
#endif // MOUSER_MOUSE_H__

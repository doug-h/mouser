#pragma once

#include <cstdint>
#include <iostream>

/*  Mouse pos is stored & sent as coord on a 1920x1080 screen, then scaled by
 * client.
 * We send current button state and mouse position, but scroll wheel delta
 * accumulated since last packet.
 * TODO - query client what current screen size is, otherwise
 * non 16:9 screens will have different x/y sensitivity. */
#define SOURCE_WIDTH 1920
#define SOURCE_HEIGHT 1080

#define BUTTON_LMASK (1 << 0)
#define BUTTON_MMASK (1 << 1)
#define BUTTON_RMASK (1 << 2)

struct MouseData {
  MouseData() : data{'m'} {}
  bool LClicked() const { return (button & BUTTON_LMASK); }
  bool MClicked() const { return (button & BUTTON_MMASK); }
  bool RClicked() const { return (button & BUTTON_RMASK); }

  union {
    struct {
      uint8_t header;
      uint8_t padding;

      uint16_t x, y;
      uint8_t button;
      uint8_t scroll_amount;
    };
    uint8_t data[8];
  };
};
static_assert(sizeof(MouseData) == 8);

inline std::ostream &operator<<(std::ostream &os, const MouseData &md) {
  os << '(' << md.x << ", " << md.y << ')';  // (x,y)
  os << '[' << ((md.LClicked()) ? 'X' : 'O') << ','
     << ((md.MClicked()) ? 'X' : 'O') << ',' << ((md.RClicked()) ? 'X' : 'O')
     << ']';  // [X,O,O]
  os << ((md.scroll_amount > 0)   ? '^'
         : (md.scroll_amount < 0) ? 'v'
                                  : 'o')
     << ", " << (int)md.scroll_amount;
  return os;
}

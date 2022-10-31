#include "network.h"

#include "SDL2/SDL.h"

#include <iostream>
#include <limits>
#include <string>
#include <vector>

static_assert(sizeof(char) == 1);
static_assert(sizeof(short) == 2);
static_assert(sizeof(int) == 4);
struct MouseState {
  union {
    struct {
      short x, y;
      char b;
      char _padding[3];
    };
    char data[8];
  };

  void Update() { b = SDL_GetMouseState((int *)&x, (int *)&y); }

  bool LClicked() const { return (b & SDL_BUTTON(SDL_BUTTON_LEFT)); }
  bool MClicked() const { return (b & SDL_BUTTON(SDL_BUTTON_MIDDLE)); }
  bool RClicked() const { return (b & SDL_BUTTON(SDL_BUTTON_RIGHT)); }
};

bool running = true;
MouseState mouse;
bool mouse_needs_update = true;
bool capturing = false;

void StartCapturing() {
  assert(not capturing);
  capturing = true;
  std::cout << "START\n";
  SDL_SetRelativeMouseMode(SDL_TRUE);
}
void StopCapturing() {
  assert(capturing);
  capturing = false;
  std::cout << "STOP\n";
  SDL_SetRelativeMouseMode(SDL_FALSE);
}

#ifdef SERVER
void ProcessEvents() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) { // 0x100
      running = false;
    } else if (e.type == SDL_KEYDOWN) { // 0x300
      if (e.key.keysym.sym == SDLK_ESCAPE) {
        if (capturing) {
          StopCapturing();
        } else {
          // mouse_needs_update = true;
        }
      } else {
        // mouse_needs_update = true;
      }
    } else if (e.type == SDL_KEYUP) { // 0x301
      // mouse_needs_update = true;
    } else if (e.type == SDL_MOUSEMOTION) { // 0x400
      mouse_needs_update = true;
    } else if (e.type == SDL_MOUSEBUTTONDOWN) { // 0x401
      mouse_needs_update = true;
    } else if (e.type == SDL_MOUSEBUTTONUP) { // 0x402
      if (not capturing) {
        StartCapturing();
      } else {
        mouse_needs_update = true;
      }
    } else {
      printf("SDL_Event: 0x%04x\n", e.type);
    }
  }
  if (mouse_needs_update) {
    mouse.Update();
  }
}
#endif
#ifdef CLIENT
void ProcessEvents() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) { // 0x100
      running = false;
    }
  }
}
#endif

std::ostream &operator<<(std::ostream &os, const MouseState &ms) {
  os << '(' << ms.x << ", " << ms.y << ')';
  os << '[' << ((ms.LClicked()) ? 'X' : 'O') << ','
     << ((ms.MClicked()) ? 'X' : 'O') << ',' << ((ms.RClicked()) ? 'X' : 'O')
     << ']';
  return os;
}

int main(int argv, char **args) {
#ifdef CLIENT
  SDL_CreateWindow("Mouser", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                   0, 0, SDL_WINDOW_HIDDEN);
  if (SDL_Init(SDL_INIT_EVENTS) != 0) {
    printf("Unable to initialize SDL: %s", SDL_GetError());
  }
  NetMan nm;

  int rate = 50; // Hz
  int delay = 1000 / rate;

  int tick_count = 0;
  std::vector<std::string> server_history = {"localhost"};
  std::string address;
  if (args[1]) {
    address = args[1];
  } else {
    std::cout << "0. Add new address" << '\n';
    int i = 1;
    for (auto s : server_history) {
      std::cout << (i++) << ". " << s << '\n';
    }
    std::cin >> i;
    if (i == 0) {
      std::cin >> address;
    } else {
      assert(i > 0 and i - 1 < server_history.size());
      address = server_history[i - 1];
    }
  }
  nm.CreateClient(address.c_str());

  while (running) {
    ProcessEvents();
    if (nm.Listen(mouse.data, 8)) {
      std::cout << mouse << '\n';
      SDL_Delay(delay);
      tick_count += delay;
      if (tick_count >= 20 * 1000) {
        running = false;
      }
    }
  }
#endif
#ifdef SERVER
  SDL_CreateWindow("Mouser", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                   200, 100, 0);
  if (SDL_Init(SDL_INIT_EVENTS) != 0) {
    printf("Unable to initialize SDL: %s", SDL_GetError());
  }
  NetMan nm;

  int rate = 50; // Hz
  int delay = 1000 / rate;

  int tick_count = 0;
  nm.Bind();
  nm.PrintAddress();
  mouse.Update();
  while (running) {
    ProcessEvents();

    nm.CheckForMessages();

    if (capturing and mouse_needs_update) {
      nm.Send(mouse.data, 8);
      mouse_needs_update = false;
      SDL_Delay(delay);
      tick_count += delay;
      if (tick_count >= 20 * 1000) {
        running = false;
      }
    }
  }
#endif
  return 0;
}

#include "network.h"

#include "SDL2/SDL.h"

#include <iostream>
#include <limits>

bool running = true;

void ProcessEvents() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    std::cout << "running" << '\n';
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      if (e.key.keysym.sym == SDLK_ESCAPE) {
        SDL_Event e;
        e.type = SDL_QUIT;
        SDL_PushEvent(&e);
      }
    }
  }
}

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

  void Update() { b = SDL_GetGlobalMouseState((int *)&x, (int *)&y); }

  bool LClicked() const { return (b & SDL_BUTTON(SDL_BUTTON_LEFT)); }
  bool MClicked() const { return (b & SDL_BUTTON(SDL_BUTTON_MIDDLE)); }
  bool RClicked() const { return (b & SDL_BUTTON(SDL_BUTTON_RIGHT)); }
};

std::ostream &operator<<(std::ostream &os, const MouseState &ms) {
  os << '(' << ms.x << ", " << ms.y << ')';
  os << '[' << ((ms.LClicked()) ? 'X' : 'O') << ','
     << ((ms.MClicked()) ? 'X' : 'O') << ',' << ((ms.RClicked()) ? 'X' : 'O')
     << ']';
  return os;
}

int main(int argv, char **args) {
  SDL_CreateWindow("Mouser", 0, 0, 0, 0, SDL_WINDOW_HIDDEN);
  if (SDL_Init(SDL_INIT_EVENTS) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
  }
  NetMan nm;
  MouseState ms;

  int rate = 50; // Hz
  int delay = 1000 / rate;

  int tick_count = 0;
#ifdef CLIENT
  nm.CreateClient(args[1]);
  while (running) {
    ProcessEvents();
    //if (nm.Listen(ms.data, 8)) {
    //  std::cout << ms << '\n';
    //}

    int l =  nm.LatencyTest();
    if(l >= 0) {
    std::cout << "RTT: " << l << "ms." << '\n';
    }
    else {
      std::cout << "RTT: timed out." << '\n';
    }

    SDL_Delay(delay);
    tick_count += delay;
    if (tick_count >= 20 * 1000) {
      running = false;
    }
  }
#endif
#ifdef SERVER
  delay /= 10;
  nm.Bind();
  nm.PrintAddress();
  while (running) {
    ProcessEvents();
    ms.Update();

    nm.CheckForMessages();

    nm.Send(ms.data, 8);
    SDL_Delay(delay);
    tick_count += delay;
    if (tick_count >= 20 * 1000) {
      running = false;
    }
  }
#endif
  return 0;
}

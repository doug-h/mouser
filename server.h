/* Mixing SDL and win32 api bad idea,
 * for now make sure win32 net stuff included before SDL. */

#include "network.h"
// <-- To stop clang format breaking include order -->
#include "mouse.h"

#include <algorithm>
#include <iostream>

class Server {
public:
  Server() : net{}, mouse{} {
    window = SDL_CreateWindow("Mouser", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, 200, 100,
                              SDL_WINDOW_ALWAYS_ON_TOP);
    if (window == nullptr) {
      printf("Unable to create window: %s", SDL_GetError());
    }
    if (SDL_Init(SDL_INIT_EVENTS) != 0) {
      printf("Unable to initialize SDL: %s", SDL_GetError());
    }
    surface = SDL_GetWindowSurface(window);
    if (surface == nullptr) {
      printf("Unable to get surface: %s", SDL_GetError());
    }
    esc = SDL_LoadBMP("../assets/esc.bmp");
    if (esc == nullptr) {
      printf("Unable to load esc.bmp: %s", SDL_GetError());
    }

    net.Bind();
    net.PrintAddress();
  }

  void Run();

private:
  NetMan net;
  MouseData mouse;

  SDL_Window *window;
  SDL_Surface *surface;
  SDL_Surface *esc;

  const int rate = 50; // Hz
  const int delay = 1000 / rate;

  bool running = true;
  bool capturing = false;

  bool mouse_has_updated = true;

  void ProcessEvents();
  void StartCapturing();
  void StopCapturing();

  void Quit() { running = false; };
};

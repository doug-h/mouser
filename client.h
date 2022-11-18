/* Mixing SDL and win32 api bad idea,
 * for now make sure win32 included before SDL. */

#include "network.h"
// <-- To stop clang format breaking include order -->
#include "mouse.h"

class Client {
public:
  Client() : net{}, mouse{} {
    if (SDL_CreateWindow("Mouser", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, 0, 0,
                         SDL_WINDOW_HIDDEN) == nullptr) {

      printf("Unable to create window: %s", SDL_GetError());
    }
    if (SDL_Init(SDL_INIT_EVENTS) != 0) {
      printf("Unable to initialize SDL: %s", SDL_GetError());
    }
  }

  void Connect(const char*);
  void Run();


private:
  NetMan net;
  MouseData mouse;

  // List of known ip addresses
  std::vector<std::string> server_book = {"localhost"};

  int rate = 50; // Hz
  int delay = 1000 / rate;

  bool running = true;
  bool capturing = false;

  bool mouse_has_updated = true;

  void ProcessEvents();
  void StartCapturing();
  void StopCapturing();

  void Quit() { running = false; };
};

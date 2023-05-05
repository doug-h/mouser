/* Mixing SDL and win32 api bad idea,
 * for now make sure win32 net stuff included before SDL. */

#include <SDL2/SDL.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

#include "input.h"
#include "network.h"
#include "platform.h"

class Server {
 public:
  Server();
  void Start();

 private:
  void Send(const char *buffer, int buffer_length);

  void ProcessEvents();
  void CheckForMessages();

  void StartCapturing();
  void StopCapturing();

  void Quit() { running = false; };

  const int rate = 50;  // Hz
  const int delay = 1000 / rate;
  const char *const port = "34197";

  bool running = true;
  bool capturing = false;

  bool mouse_has_updated = true;
  // ticks to wait before we stop sending input
  const int keyboard_sleep_time = rate;
  int keyboard_sleep_counter = 0;

  MousePacket mouse;
  KeyPacket keys;

  // Platform specific
  std::unique_ptr<Socket> socket;

  std::vector<socket_address> clients;

  SDL_Window *window;
  const Uint8 *keyboard_state;
  SDL_Surface *surface;
  SDL_Surface *esc;
};

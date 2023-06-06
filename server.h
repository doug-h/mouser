/* Mixing SDL and win32 api bad idea,
 * for now make sure win32 net stuff included before SDL. */

#include <SDL2/SDL.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

#include "input.h"
#include "packet.h"
#include "platform.h"

class Server {
public:
  Server();
  void Start();

private:
  void Send(const char *buffer, int buffer_length);
  void PackKeyboardState();

  void ProcessEvents();
  void CheckForMessages();

  void StartCapturing();
  void StopCapturing();

  void Quit() { running = false; };

public:
  const int rate = 50; // Hz
  const int delay = 1000 / rate;
  const char *const port = "34197";

private:
  bool running;
  bool capturing;

  bool mouse_has_updated;

  // ticks to wait before we stop sending input
  const int keyboard_sleep_time = rate;
  int keyboard_sleep_counter;

  MousePacket mouse;
  KeyPacket keys;

  std::vector<sockaddr_storage> clients;

  SDL_Window *window;
  const Uint8 *keyboard_state;
  SDL_Surface *surface;
  SDL_Surface *esc;

  // Platform specific
  std::unique_ptr<Socket> socket;
};

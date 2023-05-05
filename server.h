/* Mixing SDL and win32 api bad idea,
 * for now make sure win32 net stuff included before SDL. */

#include <winsock2.h>
#include <ws2tcpip.h>
//
#include <SDL2/SDL.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#include "input.h"
#include "network.h"

class Server {
public:
  Server();
  void Start();

private:
  void CreateSocket();
  void BindSocket(const char *port = "34197");
  void Send(const uint8_t *buffer, int buffer_length);

  void ProcessEvents();
  void CheckForMessages();

  void StartCapturing();
  void StopCapturing();

  void ShutdownSockets() { WSACleanup(); }
  void Quit() { running = false; };

  const int rate = 50; // Hz
  const int delay = 1000 / rate;

  bool running = true;
  bool capturing = false;

  bool mouse_has_updated = true;
  // ticks to wait before we stop sending input
  const int keyboard_sleep_time = rate;
  int keyboard_sleep_counter = 0;

  MousePacket mouse;
  KeyPacket keys;
  SOCKET socket_handle;
  std::vector<sockaddr_in> clients;

  const Uint8 *keyboard_state;

  SDL_Window *window;
  SDL_Surface *surface;
  SDL_Surface *esc;
};

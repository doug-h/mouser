#pragma once

/*
Contains the things that need to be implemented before Mouser will run on a
platform. Since we use SDL2 this is not many things, and could be fewer since
the socket API is pretty much identical between Linux/MacOS & Windows with minor
differences like close() vs closesocket().
 */

#include <SDL2/SDL.h>

#include "input.h"
#include <memory>


// Every platform has a struct called sockaddr_storage, so we just use their
// definition
#ifdef __unix__
// TODO - Test linux
#include <sys/socket.h>
#elif defined(_WIN32) || defined(WIN32)
#include <Winsock2.h>
#endif

// ========= Implement this for each platform ============
class Socket {
public:
  Socket(bool blocking, bool ipv6) : m_blocking(blocking), m_ipv6(ipv6) {}
  virtual ~Socket() {}

  // For Client
  virtual bool Connect(const char *address, const char *port) = 0;
  virtual int Receive(char *buffer, int buffer_len) = 0;
  virtual bool Send(const char *message, int message_length) = 0;

  // For Server
  virtual bool Bind(const char *port) = 0;
  virtual int Listen(char *buffer, int buffer_len,
                     sockaddr_storage &sender) = 0;
  virtual bool SendTo(const sockaddr_storage &to, const char *message,
                      int message_length) = 0;

protected:
  bool m_blocking;
  bool m_ipv6;
};
// ==========================================================
//
//
// ========== Define these for each platform ==============
namespace Platform {
void SetKeys(SDL_Scancode *keys_to_press, SDL_Scancode *keys_to_release,
             SDL_Scancode *keys_to_repeat, int num_to_press, int num_to_release,
             int num_to_repeat);
void SetMouse(const MouseData &);
// Returns base pointer of derived, platform-specific, socket.
std::unique_ptr<Socket> CreateSocket(bool blocking, bool ipv6);
} // namespace Platform
// =======================================================

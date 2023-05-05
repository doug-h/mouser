#include <SDL2/SDL.h>

// Every platform has a struct called sockaddr_storage, so we just use their
// definition
#ifdef __unix__
// TODO - Test linux
#include <sys/socket.h>
#elif defined(_WIN32) || defined(WIN32)
#include <Winsock2.h>
#endif

#include <memory>

#include "input.h"

/* Socket API is pretty much identical between Linux/MacOS & Windows with minor
 differences like close() vs closesocket().
 To make things easier we define an interface for the stuff we want, and
 define it per platform.
 (TBH could just use some #ifdefs instead since the APIs are so similar.) */

// We don't care what this looks like, we just use it to represent an address
using socket_address = sockaddr_storage;

// ========= Implement this for each platform ============
class Socket {
 public:
  Socket(bool blocking, bool ipv6) : m_blocking(blocking), m_ipv6(ipv6) {}

  // For Client
  virtual bool Connect(const char *address, const char *port) = 0;
  virtual int Receive(char *buffer, int buffer_len) = 0;
  virtual bool Send(const char *message, int message_length) = 0;

  // For Server
  virtual bool Bind(const char *port) = 0;
  virtual int Listen(char *buffer, int buffer_len, socket_address &from) = 0;
  virtual bool SendTo(const socket_address &to, const char *message,
                      int message_length) = 0;

  bool m_blocking;
  bool m_ipv6;
};
// ==========================================================
//
//
// ========== Define these for each platform ============
void SetKeys(SDL_Scancode *keys_to_press, SDL_Scancode *keys_to_release,
             int num_to_press, int num_to_release);
void SetMouse(const MouseData &);
// Returns base pointer to derived, platform-specific, socket.
std::unique_ptr<Socket> CreateSocket(bool blocking, bool ipv6);
// =======================================================

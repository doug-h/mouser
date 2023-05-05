#include <SDL2/SDL.h>
#include <memory>

#include "input.h"

// ========= Implement this for each platform ============
class Socket {
public:
  Socket(bool blocking, bool ipv6) : m_blocking(blocking), m_ipv6(ipv6) {}

  virtual bool Connect(const char *address, const char *port) = 0;
  virtual bool Send(const char *message, int message_length) = 0;
  virtual int Listen(uint8_t *buffer, int buffer_len) = 0;

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

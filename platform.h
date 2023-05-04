#include <memory>

#include "keyboard.h"
#include "mouse.h"

#define WIDTH 1920
#define HEIGHT 1080

// Implement these for each platform

void SetMouse(const MouseData& md);
void SetKeys(const KeyData& kd);

class Socket {
 public:
  Socket(bool blocking, bool ipv6) : m_blocking(blocking), m_ipv6(ipv6) {}

  virtual bool Connect(const char* address, const char* port) = 0;
  virtual bool Send(const char* message, int message_length) = 0;
  virtual int Listen(uint8_t* buffer, int buffer_len) = 0;

  bool m_blocking;
  bool m_ipv6;
};

// Returns base pointer to derived, platform-specific, socket.
std::unique_ptr<Socket> CreateSocket(bool blocking, bool ipv6);

#include <errno.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cassert>
#include <cstdint>
#include <cstdio>

#include "platform.h"


// Basic datagram UDP socket using linux sockets
class LinuxSocket final : public Socket {
public:
  LinuxSocket(bool blocking, bool ipv6);
  ~LinuxSocket();

  // For Client
  virtual bool Connect(const char *address, const char *port) override;
  virtual int Receive(char *buffer, int buffer_len) override;
  virtual bool Send(const char *message, int message_length) override;

  // For Server
  virtual bool Bind(const char *port) override;
  virtual int Listen(char *buffer, int buffer_len,
                     sockaddr_storage &from) override;
  virtual bool SendTo(const sockaddr_storage &to, const char *message,
                      int message_length) override;

private:
  int m_handle;
};

LinuxSocket::LinuxSocket(bool blocking, bool ipv6) : Socket(blocking, ipv6)
{
  puts("Loading Linux socket.");

  int domain, type;

  domain = (ipv6) ? AF_INET6 : AF_INET;
  type = (!blocking) ? SOCK_DGRAM | SOCK_NONBLOCK : SOCK_DGRAM;
  m_handle = socket(domain, type, IPPROTO_UDP);

  if (m_handle == -1) {
    int failure = errno;
    printf("Failed to create socket, errno: %d\n", failure);
    assert(0);
  }
}

LinuxSocket::~LinuxSocket()
{
  puts("Shutting down Linux socket.");
  close(m_handle);
}

// Used by clients to connect to address::port
// Since we are 'connectionless' all this does is set a location for later
// send/recv calls
bool LinuxSocket::Connect(const char *address, const char *port)
{
  // TODO - read the docs...
  addrinfo hints, *servinfo;
  int iResult, failure;

  memset(&hints, 0, sizeof(hints));

  // TODO - support both
  hints.ai_family = (m_ipv6) ? AF_INET6 : AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  // get ready to connect
  iResult = getaddrinfo(address, port, &hints, &servinfo);
  if (iResult) {
    printf("LinuxSocket::Connect(): getaddrinfo failed with error: %d.\n",
           iResult);
    return false;
  }
  // servinfo now points to a linked list of 1 or more struct addrinfos,
  // we just connect to the first one
  // TODO - pick
  iResult =
      connect(m_handle, servinfo->ai_addr, (socklen_t)servinfo->ai_addrlen);
  failure = errno;
  freeaddrinfo(servinfo);

  if (iResult) {
    printf("Failed to connect socket, errno: %d\n", failure);
    return false;
  }

  return true;
}


bool LinuxSocket::Send(const char *message, int message_length)
{

  int iResult = (int)send(m_handle, message, message_length, 0);
  if (iResult == -1) {
    int failure = errno;
    printf("Failed to connect socket, errno: %d\n", failure);
    return false;
  }
  return true;
}

int LinuxSocket::Receive(char *buffer, int buffer_len)
{
  ssize_t iResult = recv(m_handle, buffer, buffer_len, MSG_TRUNC);
  if (iResult == -1) {
    int failure = errno;
    if (failure == EAGAIN or failure == EWOULDBLOCK) {
      // No data to receive and socket is non-blocking
      return 0;
    } else {
      // Some other unknown reason
      printf("Error %d. Failed to receive.\n", failure);
      return 0;
    }
  } else if (iResult > buffer_len) {
    puts("Message too large, remainder has been discarded");
    return buffer_len;
  }
  return (int)iResult;
}

bool LinuxSocket::Bind(const char *port)
{
  addrinfo hints, *servinfo;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = (m_ipv6) ? AF_INET6 : AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  ssize_t iResult = getaddrinfo(NULL, port, &hints, &servinfo);
  if (iResult) {
    printf("LinuxSocket::Bind(): getaddrinfo failed with Error %ld.\n",
           iResult);
    return false;
  }
  iResult = bind(m_handle, servinfo->ai_addr, (int)servinfo->ai_addrlen);
  if (iResult == -1) {
    int failure = errno;
    printf("Error %d. Failed to send new_cli message.\n", failure);
    return false;
  }

  return true;
}

int LinuxSocket::Listen(char *buffer, int buffer_len, sockaddr_storage &sender)
{
  socklen_t sender_len = sizeof(sender);

  ssize_t iResult = recvfrom(m_handle, buffer, buffer_len, 0,
                             (sockaddr *)&sender, &sender_len);
  if (iResult == -1) {
    int failure = errno;
    if (failure == EAGAIN or failure == EWOULDBLOCK) {
      // No data to receive and socket is non-blocking
      return 0;
    } else {
      // Some other unknown reason
      printf("Error %d. Failed to receive.\n", failure);
      return 0;
    }
  } else if (iResult > buffer_len) {
    puts("Message too large, remainder has been discarded");
    return buffer_len;
  }
  return (int)iResult;
}

bool LinuxSocket::SendTo(const sockaddr_storage &target, const char *message,
                         int message_length)
{
  ssize_t iResult = sendto(m_handle, message, message_length, 0,
                           (const sockaddr *)&target, sizeof(target));
  if (iResult == -1) {
    int failure = errno;
    printf("Error %d. Failed to receive.\n", failure);
    return false;
  }

  return true;
}

namespace Platform {
std::unique_ptr<Socket> CreateSocket(bool blocking, bool ipv6)
{
  return std::make_unique<LinuxSocket>(blocking, ipv6);
}
} // namespace Platform
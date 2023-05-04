#include <winsock2.h>
#include <ws2tcpip.h>

#include <cassert>
#include <cstdint>
#include <cstdio>

#include "platform.h"

// I think if you don't call WSACleanup when the process crashes
// Windows will clean itself up.
// TODO - Test this ^^^

// Basic datagram UDP socket using Winsock2
class WinSocket final : public Socket {
 public:
  WinSocket(bool blocking, bool ipv6);
  ~WinSocket();

  virtual bool Connect(const char* address, const char* port) override;

  virtual bool Send(const char* message, int message_length) override;
  virtual int Listen(uint8_t* buffer, int buffer_len) override;

 private:
  SOCKET m_handle;
};

std::unique_ptr<Socket> CreateSocket(bool blocking, bool ipv6) {
  return std::make_unique<WinSocket>(blocking, ipv6);
}

WinSocket::WinSocket(bool blocking, bool ipv6) : Socket(blocking, ipv6) {
  puts("Loading WinSock2.");
  int iResult;

  WSADATA WsaData;
  iResult = WSAStartup(MAKEWORD(2, 2), &WsaData);
  if (iResult != NO_ERROR) {
    printf("Failed to initialise Winsock2: %d\n", iResult);
    assert(0);
  }

  int af = (ipv6) ? AF_INET6 : AF_INET;
  m_handle = socket(af, SOCK_DGRAM, IPPROTO_UDP);

  if (m_handle == SOCKET_ERROR) {
    printf("Failed to create socket: %d\n", WSAGetLastError());
    assert(0);
  }

  unsigned long blk = !blocking;

  iResult = ioctlsocket(m_handle, FIONBIO, &blk);
  if (iResult == SOCKET_ERROR) {
    printf("Failed to set blocking: %d\n", WSAGetLastError());
    assert(0);
  }
}

WinSocket::~WinSocket() {
  puts("Shutting down WinSock2.");
  closesocket(m_handle);

  int iResult = WSACleanup();
  if (iResult != NO_ERROR) {
    printf("Failed to cleanup Winsock2: %d\n", iResult);
    assert(0);
  }
}

// Used by clients to connect to address::port
// Since we are 'connectionless', all this does is set a location for later
// send/recv calls
bool WinSocket::Connect(const char* address, const char* port) {
  addrinfo hints, *servinfo;
  int iResult;

  memset(&hints, 0, sizeof(hints));  // clear struct

  // TODO - support both
  hints.ai_family = (m_ipv6) ? AF_INET6 : AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  // get ready to connect
  iResult = getaddrinfo(address, port, &hints, &servinfo);
  if (iResult) {
    printf("Error %d. Failed to find address: %s.\n", iResult, address);
    return false;
  }
  // servinfo now points to a linked list of 1 or more struct addrinfos,
  // we just connect to the first one
  iResult = connect(m_handle, servinfo->ai_addr, servinfo->ai_addrlen);
  freeaddrinfo(servinfo);
  return (iResult != SOCKET_ERROR);
}

bool WinSocket::Send(const char* message, int message_length) {
  int iResult = send(m_handle, message, message_length, 0);
  return (iResult != SOCKET_ERROR);
}

int WinSocket::Listen(uint8_t* buffer, int buffer_len) {
  // In-case this is run on some ancient OS or Windows release a calculator
  static_assert(sizeof(uint8_t) == sizeof(char));

  int iResult = recv(m_handle, (char*)buffer, buffer_len, 0);
  if (iResult == SOCKET_ERROR) {
    switch (WSAGetLastError()) {
      case WSAEWOULDBLOCK:
        // No data to receive and socket is non-blocking
        return 0;
      case WSAEMSGSIZE:
        puts("Message too large, remainder has been discarded");
        return buffer_len;
      default:
        // Some other unknown reason
        printf("Error %d. Failed to listen.\n", WSAGetLastError());
        return 0;
    }
  }
  return iResult;
}

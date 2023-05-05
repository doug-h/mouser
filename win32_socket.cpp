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

  // For Client
  virtual bool Connect(const char *address, const char *port) override;
  virtual int Receive(char *buffer, int buffer_len) override;
  virtual bool Send(const char *message, int message_length) override;

  // For Server
  virtual bool Bind(const char *port) override;
  virtual int Listen(char *buffer, int buffer_len,
                     socket_address &from) override;
  virtual bool SendTo(const socket_address &to, const char *message,
                      int message_length) override;

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
// Since client is 'connection-less' all this does is set a location for later
// send/recv calls
bool WinSocket::Connect(const char *address, const char *port) {
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

bool WinSocket::Send(const char *message, int message_length) {
  int iResult = send(m_handle, message, message_length, 0);
  return (iResult != SOCKET_ERROR);
}

int WinSocket::Receive(char *buffer, int buffer_len) {
  int iResult = recv(m_handle, buffer, buffer_len, 0);
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
        printf("Error %d. Failed to receive.\n", WSAGetLastError());
        return 0;
    }
  }
  return iResult;
}

bool WinSocket::Bind(const char *port) {
  addrinfo hints, *servinfo;
  int iResult;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = (m_ipv6) ? AF_INET6 : AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  iResult = getaddrinfo(NULL, port, &hints, &servinfo);
  if (iResult) {
    printf("getaddrinfo: %d\n", WSAGetLastError());
    return false;
  }
  iResult = bind(m_handle, servinfo->ai_addr, servinfo->ai_addrlen);
  if (iResult == SOCKET_ERROR) {
    printf("Error %d. Failed to send new_cli message.\n", WSAGetLastError());
    return false;
  }

  return true;
}

int WinSocket::Listen(char *buffer, int buffer_len, socket_address &sender) {
  int iResult;
  int sen_len = sizeof(sender);
  iResult =
      recvfrom(m_handle, buffer, buffer_len, 0, (sockaddr *)&sender, &sen_len);
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
        printf("Error %d. Failed to receive.\n", WSAGetLastError());
        return 0;
    }
  }
  return iResult;
}

bool WinSocket::SendTo(const socket_address &target, const char *message,
                       int message_length) {
  int iResult;
  iResult = sendto(m_handle, message, message_length, 0,
                   (const sockaddr *)&target, sizeof(target));
  return (iResult != SOCKET_ERROR);
}
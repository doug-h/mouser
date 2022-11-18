#ifndef MOUSER_NETWORK_H__
#define MOUSER_NETWORK_H__

#include <cassert>
#include <cstdlib>

#include <chrono>
#include <iostream>
#include <vector>

#include <ws2tcpip.h>
#include <winsock2.h>

// TODO -  remove this, no reason to reference count
static int SOCKETCOUNT = 0;

void InitWinsock() {
  WSADATA WsaData;
  int iResult = WSAStartup(MAKEWORD(2, 2), &WsaData);
  if (iResult != NO_ERROR) {
    printf("Failed to initialise Winsock: %d\n", iResult);
    assert(0);
  }
}

void ShutdownWinsock() {
  int iResult = WSACleanup();
  if (iResult != NO_ERROR) {
    printf("Failed to cleanup Winsock: %d\n", iResult);
    assert(0);
  }
}

struct NetMan {
  NetMan() {
    if (!SOCKETCOUNT) {
      InitWinsock();
    }
    ++SOCKETCOUNT;

    //handle = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (handle == SOCKET_ERROR) {
      printf("Failed to create socket: %d\n", WSAGetLastError());
      ShutdownWinsock();
      assert(0);
    }
#ifdef SERVER
    unsigned long blocking = 1;
#endif
#ifdef CLIENT
    unsigned long blocking = 0;
#endif
    int iResult = ioctlsocket(handle, FIONBIO, &blocking);
    if (iResult == SOCKET_ERROR) {
      printf("Failed to set non-blocking: %d\n", WSAGetLastError());
      ShutdownWinsock();
      assert(0);
    }
  }

  ~NetMan() {
    if (handle) {
      closesocket(handle);
    }
    --SOCKETCOUNT;

    if (!SOCKETCOUNT) {
      ShutdownWinsock();
    }
  }

  void PrintAddress() {
    //sockaddr_in6 name;
    sockaddr_in name;
    int namelen = sizeof(name);
    getsockname(handle, (sockaddr *)&name, &namelen);

    char address[64];
    //inet_ntop(name.sin6_family, &(name.sin6_addr), address, sizeof(address));
    inet_ntop(name.sin_family, &(name.sin_addr), address, sizeof(address));
    //std::cout << address << "::" << ntohs(name.sin6_port) << '\n';
    std::cout << address << "::" << ntohs(name.sin_port) << '\n';
  }

  // ============= Client Code ================
  void CreateClient(const char* address) {
    const char *port = "34197";

    addrinfo hints, *servinfo;
    int iResult;

    memset(&hints, 0, sizeof(hints)); // make sure the struct is empty
    //hints.ai_family = AF_INET6;       // IPv6
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_DGRAM;   // UDP

    // get ready to connect
    iResult = getaddrinfo(address, port, &hints, &servinfo);
    if (iResult) {
      printf("Error %d. Failed to find address: %s.\n", iResult,
             address);
      ShutdownWinsock();
      assert(0);
    }

    // servinfo now points to a linked list of 1 or more struct addrinfos

    iResult = connect(handle, servinfo->ai_addr, servinfo->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
      printf("Error %d. Failed to connect.\n", WSAGetLastError());
      ShutdownWinsock();
      assert(0);
    }

    freeaddrinfo(servinfo);

    const char message[8] = "new_cli";
    iResult = send(handle, message, sizeof(message), 0);
    if (iResult == SOCKET_ERROR) {
      printf("Error %d. Failed to send new_cli message.\n", WSAGetLastError());
    }
  }

  bool Listen(char *buffer, int buffer_len) {
    int iResult = recv(handle, buffer, buffer_len, 0);
    if (iResult == SOCKET_ERROR) {
      int code = WSAGetLastError();
      if (code == WSAEWOULDBLOCK) {
        return false;
      } else {
        printf("Error %d. Failed to listen.\n", code);
        ShutdownWinsock();
        assert(0);
      }
    }
    return true;
  };

  int LatencyTest() {
    /* Send client to server
     * Wait for response
     * Returns no. of ms. between*/

    std::srand(std::time(nullptr));
    int id = std::rand();
    static_assert(sizeof(char) == 1);
    static_assert(sizeof(int) == 4);
    char message[8] = "lat";
    message[3] = (id >> 24) & 0xff;
    message[4] = (id >> 16) & 0xff;
    message[5] = (id >> 8) & 0xff;
    message[6] = id & 0xff;

    int iResult = send(handle, message, sizeof(message), 0);
    if (iResult == SOCKET_ERROR) {
      printf("Error %d. Failed to send lat. test.\n", WSAGetLastError());
    }

    auto T1 = std::chrono::high_resolution_clock::now();

    char response[8];
    bool response_received = false;
    int max_iter = 10000000;
    // For now mousestate data is discarded while lat.testing
    while (not response_received and max_iter-- > 0) {
      while (Listen(response, sizeof(response))) {
        response_received = (strcmp(response, message) == 0);
      }
    }
    auto T2 = std::chrono::high_resolution_clock::now();
    auto latency =
        std::chrono::duration_cast<std::chrono::milliseconds>(T2 - T1);

    return (response_received) ? latency.count() : -1;
  };
  // ============= Server Code =============

  void Bind() {
    const char *port = "34197";

    addrinfo hints, *servinfo;
    int iResult;

    memset(&hints, 0, sizeof(hints));
    //hints.ai_family = AF_INET6;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, port, &hints, &servinfo);
    if (iResult) {
      printf("getaddrinfo: %d\n", WSAGetLastError());
      ShutdownWinsock();
      assert(0);
    }
    iResult = bind(handle, servinfo->ai_addr, servinfo->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
      printf("Error %d. Failed to send new_cli message.\n", WSAGetLastError());
      ShutdownWinsock();
      assert(0);
    }
  }

  // Listens for command messages
  void CheckForMessages() {

    //sockaddr_in6 client;
    sockaddr_in client;
    int clientlen = sizeof(client);

    char message[8];
    int iResult = SOCKET_ERROR;

    int max_iter = 10000;
    while (max_iter-- > 0) {
      iResult = recvfrom(handle, message, sizeof(message), 0,
                         (sockaddr *)&client, &clientlen);
      if (iResult == SOCKET_ERROR) {
        int code = WSAGetLastError();
        if (code == WSAEWOULDBLOCK) {
          // Normal?
        } else if (code == WSAECONNRESET) {
          // Handle client disconnecting
        } else {
          printf("Error %d. Failed to listen.\n", code);
          ShutdownWinsock();
          assert(0);
        }
        return;
        // Else we received a packet
      } else if (strcmp(message, "new_cli") == 0) {
        // char address[64];
        //  inet_ntop(client.sin6_family, &(client.sin6_addr), address,
        //            sizeof(address));
        //  std::cout << "New client detected at " << address
        //            << "::" << ntohs(client.sin6_port) << '\n';
        std::cout << "New client accepted." << '\n';
        clients.push_back(client);
      } else if (message[0] == 'l' && message[1] == 'a' && message[2] == 't') {
        sendto(handle, message, sizeof(message), 0, (const sockaddr *)&client,
               sizeof(client));
        std::cout << "Latency test requested." << '\n';
      } else {
        std::cout << "Unrecognised message: " << message << '\n';
        ShutdownWinsock();
        assert(0);
      }
    };
    assert(max_iter > 1);
  }

  void Send(const char *buffer, int bufferlen) {
    for (const auto &a : clients) {
      sendto(handle, buffer, bufferlen, 0, (const sockaddr *)&a, sizeof(a));
    }
  }

  //std::vector<sockaddr_in6> clients;
  std::vector<sockaddr_in> clients;

private:
  SOCKET handle = INVALID_SOCKET;
};

#endif // MOUSER_NETWORK_H__

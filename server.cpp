#include "server.h"

Server::Server()
    : mouse{},
      keys{},
      socket_handle{INVALID_SOCKET},
      window{nullptr},
      surface{nullptr},
      esc{nullptr} {
  window = SDL_CreateWindow("Mouser", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, 200, 100,
                            SDL_WINDOW_ALWAYS_ON_TOP);
  if (window == nullptr) {
    printf("Unable to create window: %s", SDL_GetError());
    assert(0);
  }
  if (SDL_Init(SDL_INIT_EVENTS) != 0) {
    printf("Unable to initialize SDL: %s", SDL_GetError());
    assert(0);
  }
  surface = SDL_GetWindowSurface(window);
  if (surface == nullptr) {
    printf("Unable to get surface: %s", SDL_GetError());
    assert(0);
  }
  esc = SDL_LoadBMP("../assets/esc.bmp");
  if (esc == nullptr) {
    printf("Unable to load esc.bmp: %s", SDL_GetError());
    assert(0);
  }
  WSADATA WsaData;
  int iResult = WSAStartup(MAKEWORD(2, 2), &WsaData);
  if (iResult != NO_ERROR) {
    printf("Failed to initialise Winsock: %d\n", iResult);
    assert(0);
  }

  keyboard_state = SDL_GetKeyboardState(nullptr);
}

void Server::ProcessEvents() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    // ---------- Events that always happen ----------
    if (e.type == SDL_QUIT) {  // 0x100
      Quit();
    }
    // ---------- Events when input is NOT captured ----------
    else if (not capturing) {
      /* Capturing starts if capture box gains focus (clicking/alt-tabbing).
       * We also respond to clicks if the window is already focussed but not
       * capturing (user pressed ESCAPE). */
      if (e.type == SDL_MOUSEBUTTONUP or
          e.type == SDL_WINDOWEVENT and
              e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
        StartCapturing();
      }
    }

    // ---------- Events when input IS captured ----------
    else {
      if (e.type == SDL_KEYDOWN or e.type == SDL_KEYUP) {  // 0x300 / 0x301
        if (e.key.keysym.sym == SDLK_ESCAPE) {
          StopCapturing();
        } else {
          keyboard_has_updated = true;
        }
      } else if (e.type == SDL_MOUSEMOTION) {  // 0x400
        /* Mouse is constrained to small (200x100) window,
         * so we have to do screen clamping */
        mouse.x = std::clamp(mouse.x + e.motion.xrel, 0, SOURCE_WIDTH);
        mouse.y = std::clamp(mouse.y + e.motion.yrel, 0, SOURCE_HEIGHT);
        mouse_has_updated = true;

      } else if (e.type == SDL_MOUSEBUTTONDOWN) {  // 0x401
        mouse_has_updated = true;

      } else if (e.type == SDL_MOUSEBUTTONUP) {  // 0x402

        mouse_has_updated = true;

      } else if (e.type == SDL_MOUSEWHEEL) {  // 0x403
        mouse.scroll_amount += e.wheel.y;
        mouse_has_updated = true;

      } else {
        // Print events not yet captured
        printf("SDL_Event: 0x%04x\n", e.type);
      }
    }
  }
}
void Server::StartCapturing() {
  assert(not capturing);
  capturing = true;
  std::cout << "START\n";
  SDL_SetRelativeMouseMode(SDL_TRUE);
  SDL_BlitSurface(esc, nullptr, surface, nullptr);
  SDL_UpdateWindowSurface(window);
}

void Server::StopCapturing() {
  assert(capturing);
  capturing = false;
  std::cout << "STOP\n";
  SDL_SetRelativeMouseMode(SDL_FALSE);
  SDL_Rect screen = {0, 0, 200, 100};
  Uint32 white = SDL_MapRGB(surface->format, 255, 255, 255);
  SDL_FillRect(surface, &screen, white);
  SDL_UpdateWindowSurface(window);
}

void Server::CreateSocket() {
  socket_handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (socket_handle == SOCKET_ERROR) {
    printf("Failed to create socket: %d\n", WSAGetLastError());
    ShutdownSockets();
    assert(0);
  }

  unsigned long non_blocking = true;

  int iResult = ioctlsocket(socket_handle, FIONBIO, &non_blocking);
  if (iResult == SOCKET_ERROR) {
    printf("Failed to set non-blocking: %d\n", WSAGetLastError());
    ShutdownSockets();
    assert(0);
  }
}

void Server::BindSocket(const char *port) {
  addrinfo hints, *servinfo;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  int iResult;
  iResult = getaddrinfo(NULL, port, &hints, &servinfo);
  if (iResult) {
    printf("getaddrinfo: %d\n", WSAGetLastError());
    ShutdownSockets();
    assert(0);
  }
  iResult = bind(socket_handle, servinfo->ai_addr, servinfo->ai_addrlen);
  if (iResult == SOCKET_ERROR) {
    printf("Error %d. Failed to send new_cli message.\n", WSAGetLastError());
    ShutdownSockets();
    assert(0);
  }
}
void Server::CheckForMessages() {
  sockaddr_in client;
  int clientlen = sizeof(client);

  char message[8];
  int iResult = SOCKET_ERROR;

  int max_iter = 10000;
  while (max_iter-- > 0) {
    iResult = recvfrom(socket_handle, message, sizeof(message), 0,
                       (sockaddr *)&client, &clientlen);
    if (iResult == SOCKET_ERROR) {
      int code = WSAGetLastError();
      if (code == WSAEWOULDBLOCK) {
        // Normal?
      } else if (code == WSAECONNRESET) {
        // Handle client disconnecting
      } else {
        printf("Error %d. Failed to listen.\n", code);
        ShutdownSockets();
        assert(0);
      }
      return;

      // Otherwise we received a packet
    } else if (strcmp(message, "cnewcli") == 0) {
      std::cout << "New client accepted." << '\n';
      clients.push_back(client);
    } else if (message[0] == 'l' && message[1] == 'a' && message[2] == 't') {
      sendto(socket_handle, message, sizeof(message), 0,
             (const sockaddr *)&client, sizeof(client));
      std::cout << "Latency test requested." << '\n';
    } else {
      std::cout << "Unrecognised message: " << message << '\n';
      ShutdownSockets();
      assert(0);
    }
  };
}
void Server::Send(const uint8_t *buffer, int bufferlen) {
  // TODO - handle errors
  for (const auto &a : clients) {
    sendto(socket_handle, (char *)buffer, bufferlen, 0, (const sockaddr *)&a,
           sizeof(a));
  }
}

void Server::Start() {
  CreateSocket();
  BindSocket();
  while (running) {
    ProcessEvents();

    CheckForMessages();

    if (capturing and mouse_has_updated) {
      mouse.button = SDL_GetMouseState(nullptr, nullptr);
      Send(mouse.data, 8);
      mouse.scroll_amount = 0;
    }
    if (capturing and keyboard_has_updated) {
      UpdateKeyData(keyboard_state, keys);
      Send(keys.data, 32);
    }

    if (mouse_has_updated or keyboard_has_updated) {
      mouse_has_updated = false;
      keyboard_has_updated = false;
      SDL_Delay(delay);
    }
  }
};

int main(int argv, char **args) {
  Server server;
  server.Start();
  return 0;

  return 0;
}

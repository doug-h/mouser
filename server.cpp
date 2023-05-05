#include "server.h"

Server::Server()
    : mouse{}, keys{}, window{nullptr}, surface{nullptr}, esc{nullptr} {
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

  keyboard_state = SDL_GetKeyboardState(nullptr);

  socket = CreateSocket(false, false);
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
          (e.type == SDL_WINDOWEVENT and
           e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)) {
        StartCapturing();
      }
    }

    // ---------- Events when input IS captured ----------
    else {
      if (e.type == SDL_KEYDOWN or e.type == SDL_KEYUP) {  // 0x300 / 0x301
        if (e.key.keysym.sym == SDLK_ESCAPE) {
          StopCapturing();
        } else {
          keyboard_sleep_counter = keyboard_sleep_time;
        }
      } else if (e.type == SDL_MOUSEMOTION) {  // 0x400
        /* Mouse is constrained to small (200x100) window,
         * so we have to do screen clamping */
        mouse.data.x =
            (uint16_t)std::clamp(mouse.data.x + e.motion.xrel, 0, 1920);
        mouse.data.y =
            (uint16_t)std::clamp(mouse.data.y + e.motion.yrel, 0, 1080);
        mouse_has_updated = true;

      } else if (e.type == SDL_MOUSEBUTTONDOWN) {  // 0x401
        mouse_has_updated = true;

      } else if (e.type == SDL_MOUSEBUTTONUP) {  // 0x402

        mouse_has_updated = true;

      } else if (e.type == SDL_MOUSEWHEEL) {  // 0x403
        mouse.data.scroll_amount += (int8_t)e.wheel.y;
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
  SDL_SetWindowKeyboardGrab(window, SDL_TRUE);
  SDL_BlitSurface(esc, nullptr, surface, nullptr);
  SDL_UpdateWindowSurface(window);
}

void Server::StopCapturing() {
  assert(capturing);
  capturing = false;
  std::cout << "STOP\n";
  SDL_SetRelativeMouseMode(SDL_FALSE);
  SDL_SetWindowKeyboardGrab(window, SDL_FALSE);
  SDL_Rect screen = {0, 0, 200, 100};
  Uint32 white = SDL_MapRGB(surface->format, 255, 255, 255);
  SDL_FillRect(surface, &screen, white);
  SDL_UpdateWindowSurface(window);
}

void Server::CheckForMessages() {
  sockaddr_storage client;

  char message[8];

  int max_iter = 10000;
  while (max_iter-- > 0) {
    if (socket->Listen(message, 8, client)) {
      // We received a packet
      if (strcmp(message, "cnewcli") == 0) {
        std::cout << "New client accepted." << '\n';
        clients.push_back(client);
      } else {
        std::cout << "Unrecognised message: " << message << '\n';
      }
    } else {
      continue;
    }
  }
}

void Server::Send(const char *buffer, int bufferlen) {
  // TODO - handle errors
  for (const auto &a : clients) {
    socket->SendTo(a, buffer, bufferlen);
  }
}

void Server::Start() {
  if (!socket->Bind(port)) {
    assert(0);
  }

  while (running) {
    ProcessEvents();

    CheckForMessages();

    if (capturing and mouse_has_updated) {
      mouse.data.button = (uint8_t)SDL_GetMouseState(nullptr, nullptr);
      Send((char *)&mouse, MOUSE_PACKET_SIZE);
      mouse.data.scroll_amount = 0;
    }
    // We continue to send keyboard packets for a short time after all keys
    // have been released to make sure the client doesn't miss it
    if (capturing and keyboard_sleep_counter) {
      CopySDLKeyData(keys.data, keyboard_state);
      Send((char *)&keys, KEYBOARD_PACKET_SIZE);
      --keyboard_sleep_counter;
    }

    if (mouse_has_updated or keyboard_sleep_counter) {
      mouse_has_updated = false;
      SDL_Delay(delay);
    }
  }
};

int main(int argv, char **args) {
  Server server;
  server.Start();
  return 0;
}

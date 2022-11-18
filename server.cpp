#include "server.h"


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

void Server::ProcessEvents() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {

    // ---------- Events that always happen ----------
    if (e.type == SDL_QUIT) { // 0x100
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
      if (e.type == SDL_KEYDOWN) { // 0x300
        if (e.key.keysym.sym == SDLK_ESCAPE) {
          StopCapturing();
        }
        //} else if (e.type == SDL_KEYUP) { // 0x301

      } else if (e.type == SDL_MOUSEMOTION) { // 0x400
        /* Mouse is constrained to small (200x100) window,
         * so we have to do screen clamping */
        mouse.x = std::clamp(mouse.x + e.motion.xrel, 0, SOURCE_WIDTH);
        mouse.y = std::clamp(mouse.y + e.motion.yrel, 0, SOURCE_HEIGHT);
        mouse_has_updated = true;

      } else if (e.type == SDL_MOUSEBUTTONDOWN) { // 0x401
        mouse.b = SDL_GetMouseState(nullptr, nullptr);
        mouse_has_updated = true;

      } else if (e.type == SDL_MOUSEBUTTONUP) { // 0x402
        mouse.b = SDL_GetMouseState(nullptr, nullptr);
        mouse_has_updated = true;

      } else {
        // Print events not yet captured
        printf("SDL_Event: 0x%04x\n", e.type);
      }
    }
  }
}


void Server::Run() {

  while (running) {
    ProcessEvents();

    net.CheckForMessages();

    if (capturing and mouse_has_updated) {
      net.Send(mouse.data, 8);
      mouse_has_updated = false;
      SDL_Delay(delay);
    }
  }
};

int main(int argv, char **args) {
  Server server;
  server.Run();
  return 0;
}

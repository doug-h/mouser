#include "client.h"

#include <cassert>

Client::Client() : mouse{}, keys{} {
  if (SDL_CreateWindow("Mouser", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 0, 0,
                       SDL_WINDOW_HIDDEN) == nullptr) {
    printf("Unable to create window: %s", SDL_GetError());
  }
  if (SDL_Init(SDL_INIT_EVENTS) != 0) {
    printf("Unable to initialize SDL: %s", SDL_GetError());
  }

  socket = CreateSocket(false, false);
}

void Client::ProcessEvents() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      Quit();
    }
  }
}

void Client::Connect(const char *_address) {
  std::string address;
  if (_address != nullptr) {
    address = _address;
  } else {
    std::cout << "0. Add new address" << '\n';
    int i = 1;
    for (auto s : server_book) {
      std::cout << (i++) << ". " << s << '\n';
    }
    std::cin >> i;
    if (i == 0) {
      std::cin >> address;
    } else {
      assert(i > 0 and i - 1 < server_book.size());
      address = server_book[i - 1];
      // TODO - input verification
    }
  }

  socket->Connect(address.c_str(), port);

  // Send "new client" message to server
  const char *msg = "cnewcli";
  socket->Send(msg, sizeof(msg));
}

void Client::Run() {
  while (m_running) {
    ProcessEvents();

    uint8_t msg[32];

    int n_bytes = socket->Listen(msg, 32);
    if (n_bytes) {
      switch (msg[0]) {
        case 'm':  // Mouse data
          assert(n_bytes == 8);
          memcpy(mouse.data, msg, 8);
          std::cout << mouse << '\n';
          SetMouse(mouse);
          break;
        case 'k':  // Keyb data
          assert(n_bytes == 32);
          memcpy(keys.data, msg, 32);
          SetKeys(keys);
          break;
        case 'c':  // Command
          puts("Received command.");
          break;
        default:
          puts("Unrecognised packet.");
          break;
      }

      SDL_Delay(delay);
    }
  }
}

int main(int argv, char **args) {
  Client client;

  client.Connect(args[1]);
  client.Run();

  return 0;
}

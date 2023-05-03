#include "client.h"

Client::Client() : net{}, mouse{} {
  if (SDL_CreateWindow("Mouser", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 0, 0,
                       SDL_WINDOW_HIDDEN) == nullptr) {

    printf("Unable to create window: %s", SDL_GetError());
  }
  if (SDL_Init(SDL_INIT_EVENTS) != 0) {
    printf("Unable to initialize SDL: %s", SDL_GetError());
  }
}

void Client::StartCapturing() {
  assert(not capturing);
  capturing = true;
  std::cout << "START\n";
  SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Client::StopCapturing() {
  assert(capturing);
  capturing = false;
  std::cout << "STOP\n";
  SDL_SetRelativeMouseMode(SDL_FALSE);
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
    }
  }
  net.CreateClient(address.c_str());
}

void Client::Run() {
  while (running) {
    ProcessEvents();
    if (net.Listen(mouse.data, 8)) {
      std::cout << mouse << '\n';
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

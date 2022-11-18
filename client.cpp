#include "client.h"

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

void Client::Connect(const char *_address = nullptr) {
  // Client provides ip, or chooses from server_book
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

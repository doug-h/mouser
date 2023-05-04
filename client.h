

//
#include <SDL2/SDL.h>

#include <memory>
#include <vector>

#include "keyboard.h"
#include "mouse.h"
#include "platform.h"

class Client {
 public:
  Client();

  // Call without arg to connect to saved address
  void Connect(const char* _address = nullptr);
  void Run();

 private:
  void ProcessEvents();

  void Quit() { m_running = false; };

  MouseData mouse;
  KeyData keys;

  // Platform specific
  std::unique_ptr<Socket> socket;

  // List of known ip addresses
  std::vector<std::string> server_book = {"localhost"};

  const int rate = 500;  // Hz
  const int delay = 1000 / rate;
  const char* const port = "34197";

  bool m_running = true;
};

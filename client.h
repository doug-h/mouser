

//
#include <SDL2/SDL.h>

#include <memory>
#include <vector>

#include "network.h"
#include "platform.h"

class Client {
 public:
  Client();

  // Call without arg to connect to saved address
  void Connect(const char *_address = nullptr);
  void Run();

 private:
  void UpdateMouse(MousePacket *);
  void UpdateKeys(KeyPacket *);

  void Quit() { m_running = false; };

  // Storing as packets is easier for sending/receiving
  MousePacket mouse;
  KeyPacket keys;

  // Platform specific
  std::unique_ptr<Socket> socket;

  // List of known ip addresses
  std::vector<std::string> server_book = {"localhost"};

  const int rate = 500;  // Hz
  const int delay = 1000 / rate;
  const char *const port = "34197";

  bool m_running = true;
};

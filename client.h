
#include <SDL2/SDL.h>

#include <memory>
#include <vector>

#include "packet.h"
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

public:
  static constexpr int rate = 500; // Hz
  static constexpr int delay = 1000 / rate;
  static constexpr const char *const port = "34197";
  // ^^ Is that really how you write that?? ^^
  static constexpr int max_simul_keypresses = 16;

private:
  // Storing as packets is easier for sending/receiving
  MousePacket mouse;
  KeyPacket keys;

  // List of known ip addresses
  std::vector<std::string> server_book;

  // Platform specific
  std::unique_ptr<Socket> socket;


  bool m_running;
};

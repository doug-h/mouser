/* Mixing SDL and win32 api bad idea,
 * for now make sure win32 included before SDL. */

#include "network.h"

#include "mouse.h"

class Client {
public:

  Client();

  void Connect(const char* _address = nullptr);
  void Run();


private:
  void ProcessEvents();
  void StartCapturing();
  void StopCapturing();

  void Quit() { running = false; };


  NetMan net;
  MouseData mouse;

  // List of known ip addresses
  std::vector<std::string> server_book = {"localhost"};

  int rate = 50; // Hz
  int delay = 1000 / rate;

  bool running = true;
  bool capturing = false;
  bool mouse_has_updated = true;

};

#include "client.h"

#include <cassert>

Client::Client()
    : mouse{}, keys{}, server_book{{"localhost"}},
      socket(Platform::CreateSocket(false, false)), m_running(true)
{}

void Client::Connect(const char *_address)
{
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
      assert(i > 0 and (size_t)(i - 1) < server_book.size());
      address = server_book[i - 1];
      // TODO - input verification
    }
  }

  socket->Connect(address.c_str(), port);

  // Send "new client" message to server
  const char *msg = "cnewcli";
  socket->Send(msg, sizeof(msg));
}

void Client::UpdateMouse(MousePacket *packet)
{
  mouse.data = packet->data;
  Platform::SetMouse(mouse.data);
  std::cout << mouse.data << '\n';
}

void Client::UpdateKeys(KeyPacket *packet)
{
  // TODO - rework this (KB's of ints just from my poorly thought out design)
  static SDL_Scancode keys_to_press[SDL_NUM_SCANCODES];
  static SDL_Scancode keys_to_release[SDL_NUM_SCANCODES];
  static SDL_Scancode keys_to_repeat[SDL_NUM_SCANCODES];
  int num_to_press = 0;
  int num_to_release = 0;
  int num_to_repeat = 0;

  uint8_t *received = packet->data.packed_scancodes;
  uint8_t *stored = keys.data.packed_scancodes;

  // Unpack key state
  for (size_t byte = 0; byte < N_BYTES; ++byte) {
    uint8_t diff = received[byte] ^ stored[byte];
    int bit = 0;
    while (diff) {
      SDL_Scancode s = (SDL_Scancode)(MIN_SCANCODE + 8 * byte + bit);

      if (diff & 1) {
        // Key state changed
        if (received[byte] & (1 << bit)) {
          keys_to_press[num_to_press++] = s;
        } else {
          keys_to_release[num_to_release++] = s;
        }
      } else {
        // Key state unchanged
        if (received[byte] & (1 << bit)) {
          keys_to_repeat[num_to_repeat++] = s;
        }
      }
      diff >>= 1;
      ++bit;
    }
  }

  Platform::SetKeys(keys_to_press, keys_to_release, keys_to_repeat,
                    num_to_press, num_to_release, num_to_repeat);

  keys.data = packet->data;
}

void Client::Run()
{
  while (m_running) {
    char msg[MAX_PACKET_SIZE];

    int n_bytes = socket->Receive(msg, MAX_PACKET_SIZE);
    if (n_bytes) {
      switch (CheckPacketType(msg)) {
      case MOUSE:
        // puts("Mouse packet");
        assert(n_bytes == MOUSE_PACKET_SIZE);

        UpdateMouse((MousePacket *)msg);
        break;
      case KEYBOARD:
        assert(n_bytes == KEYBOARD_PACKET_SIZE);
        // puts("Keyboard packet");
        UpdateKeys((KeyPacket *)msg);
        break;
      case COMMAND:
        puts("Received command.");
        break;
      case UNKNOWN:
      default:
        puts("Unrecognised packet.");
        break;
      }
    }
    SDL_Delay(delay);
  }
}

int main(int argv, char **args)
{
  Client client;

  client.Connect(args[1]);
  client.Run();

  return 0;
}

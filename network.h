#pragma once

#include "input.h"
#include <cstdint>

struct MousePacket {
  const uint8_t header = 'm';
  MouseData data;
};
struct KeyPacket {
  const uint8_t header = 'k';
  KeyData data;
};

enum PacketType {
  MOUSE,
  KEYBOARD,
  COMMAND,
  UNKNOWN,
};

constexpr size_t HEADER_SIZE = 1;
constexpr size_t MOUSE_PACKET_SIZE = sizeof(MousePacket);
constexpr size_t KEYBOARD_PACKET_SIZE = sizeof(KeyPacket);
constexpr size_t MOUSE_DATA_SIZE = MOUSE_PACKET_SIZE - HEADER_SIZE;
constexpr size_t KEYBOARD_DATA_SIZE = KEYBOARD_PACKET_SIZE - HEADER_SIZE;

constexpr size_t MAX_PACKET_SIZE = 32;

static_assert(MOUSE_PACKET_SIZE <= MAX_PACKET_SIZE);
static_assert(KEYBOARD_PACKET_SIZE <= MAX_PACKET_SIZE);

PacketType CheckPacketType(uint8_t *message) {
  static_assert(HEADER_SIZE == 1);
  switch (message[0]){
  case 'm':
    return MOUSE;
  case 'k':
    return KEYBOARD;
  case 'c':
    return COMMAND;
  default:
    return UNKNOWN;
  }
}

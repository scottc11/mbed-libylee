#include "MCP4461.h"

void MCP4461::init() {
  writeWiper(Wiper0, 0);
  writeWiper(Wiper1, 0);
  writeWiper(Wiper2, 0);
  writeWiper(Wiper3, 0);
}

int MCP4461::readWiper(Wiper wiper) {
  uint8_t command = (wiper << 4) | READ;
  return this->readRegister(command);
}

int MCP4461::writeWiper(Wiper wiper, int value) {
  if (value > 257) return 0;
  uint8_t command = ((( wiper << 4)) | WRITE);
  command = command | (value >> 8); // taking the 2 MSB from 10 bit value and appending it to the 2 LSB of command
  uint8_t data = value & 0xFF;      // shaving off the 2 MSB from 10 bit value
  this->writeRegister(command, data);
  return 0;
}
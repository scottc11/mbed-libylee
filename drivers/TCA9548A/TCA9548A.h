#ifndef __TCA9548A_H
#define __TCA9548A_H

#include "I2C.h"

#define TCA9548A_DEFAULT_ADDR     0x70 // 1110000
class TCA9548A {
public:
  
  enum Channel {
    CH0 = 0b00000001,
    CH1 = 0b00000010,
    CH2 = 0b00000100,
    CH3 = 0b00001000,
    CH4 = 0b00010000,
    CH5 = 0b00100000,
    CH6 = 0b01000000,
    CH7 = 0b10000000,
  };

  TCA9548A(I2C *_i2c, uint8_t _address = TCA9548A_DEFAULT_ADDR) {
    address = _address << 1;
    i2c = _i2c;
  };

  uint8_t address;
  int channel;
  I2C * i2c;
  uint8_t read_buffer[1];
  uint8_t write_buffer[1];

  void enableChan(Channel channel) {
    write_buffer[0] = channel;
    i2c->write(address, write_buffer, 1);
  }

  char currentChan() {
    i2c->read(address, read_buffer, 1);
    return read_buffer[0];
  }
};



#endif
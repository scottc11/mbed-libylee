#ifndef __MCP4461_H
#define __MCP4461_H

/**
 * NOTE: All registers mentioned in the document are little endian.
*/ 

#include <mbed.h>

#define MCP4461_I2C_ADDRESS     0x58 // 8 bit address

class MCP4461 {

  public:

    enum Wiper {
      Wiper0 = 0x00,
      Wiper1 = 0x01,
      Wiper2 = 0x06,
      Wiper3 = 0x07,
    };

    MCP4461(I2C *_i2c, int _address = MCP4461_I2C_ADDRESS) {
      i2c = _i2c;
      address = _address;
    }

    I2C *i2c;
    int address;
    
    void init();
    int writeWiper(Wiper wiper, int val);
    int readWiper(Wiper wiper);

  private:
    
    void writeRegister(char reg, char data) {    
      char buffer[2];
      buffer[0] = reg;
      buffer[1] = data;
      i2c->start();
      i2c->write(address, buffer, 2);
      i2c->stop();
    }

    char readRegister(char reg) {
      char command[1];
      char buffer[1];
      command[0] = reg;
      i2c->write(address, command, 1, true);
      i2c->read(address, buffer, 1);
      return buffer[0];
    }

    enum Registers {
      STATUS = 0x05,
      READ = 0xC,       // 11 = Read Data
      WRITE = 0x00,     // 00 = Write Data
      INCREMENT = 0x04, // 01 = Increment
      DECREMENT = 0x08, // 10 = Decrement
      V_WIPER_0 = 0x00,
      V_WIPER_1 = 0x01,
      V_WIPER_2 = 0x06,
      V_WIPER_3 = 0x07,
      NV_WIPER_0 = 0x02, // via EEPROM?
      NV_WIPER_1 = 0x03,
      NV_WIPER_2 = 0x08,
      NV_WIPER_3 = 0x09,
      TCON_0 = 0x04,
      TCON_1 = 0x0A,
    };
};

#endif
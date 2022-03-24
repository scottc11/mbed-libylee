#ifndef __CAP1208_H
#define __CAP1208_H

#include "I2C.h"
#include "TCA9548A.h"

#define CAP1208_I2C_ADDR     0x50
#define CAP1208_PROD_ID      0x6B

// supports I2C speeds to 400kHz

class CAP1208 {
  public:

  CAP1208(I2C *i2c_ptr) {
    useMux = false;
    i2c = i2c_ptr;
  }

  CAP1208(I2C *i2c_ptr, TCA9548A *mux_ptr, TCA9548A::Channel _muxChannel) {
    useMux = true;
    i2c = i2c_ptr;
    mux = mux_ptr;
    muxChannel = _muxChannel;
  }

  I2C * i2c;
  TCA9548A * mux;
  TCA9548A::Channel muxChannel;
  bool useMux;
  bool connected;
  char data_read[1];
  char data_write[2];
  uint8_t currTouched;  // not yet implemented
  uint8_t prevTouched;  // not yet implemented

  void init();
  void disableInterupts();
  bool isConnected();
  void read(uint8_t reg);
  uint8_t getControlStatus();
  uint8_t getGeneralStatus();
  void calibrate();
  void clearInterrupt();
  uint8_t touched();
  bool padIsTouched(int pad, int currTouched);
  bool padWasTouched(int pad, int currTouched, int prevTouched);

private:

  void i2cWrite(char reg, char data) {
    
    if (useMux) { mux->enableChan(muxChannel); }

    uint8_t buffer[2];
    buffer[0] = reg;
    buffer[1] = data;
    i2c->write(CAP1208_I2C_ADDR, buffer, 2);
  }

  char i2cRead(char reg) {
    
    if (useMux) { mux->enableChan(muxChannel); }

    uint8_t command[1];
    uint8_t buffer[1];
    command[0] = reg;
    i2c->write(CAP1208_I2C_ADDR, command, 1, true);
    i2c->read(CAP1208_I2C_ADDR, buffer, 1);
    return buffer[0];
  }

  enum Registers {
    MAIN_CTRL_REG = 0x00,
    GENERAL_STATUS_REG = 0x02,     // read general status of CAP1208
    SENSITIVITY = 0x1F,            // The Sensitivity Control register controls the sensitivity of a touch detection
    INPUT_STATUS_REG = 0x03,
    PRODUCT_ID_REG = 0xFD,
    AVR_SMPL_CONF_REG = 0x24,      // default: 0b00111001
    CALIBRATE_REG = 0x26,          // CALIBRATION ACTIVATE AND STATUS
    MULT_TOUCH_CONF_REG = 0x2A,
    INT_ENABLE_REG = 0x27,
    REPEAT_RATE_ENABLE_REG = 0x28,
    CONF_2_REG = 0x44,             // default: 0x40
  };
};


#endif
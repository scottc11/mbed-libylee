#ifndef __QT2120_H
#define __QT2120_H

#include <mbed.h>

#define QT2120_I2C_ADDRESS      0x1C << 1
#define QT2120_CHIP_ID          0x3E

class QT2120 {
  public:
    QT2120(I2C *_i2c, int _address = QT2120_I2C_ADDRESS) {
      i2c = _i2c;
      address = _address;
    }

    I2C *i2c;
    int address;
    bool connected;
    uint16_t touched;

    void init();
    void setDetectThreshold(uint8_t value);
    void setChargeTime(uint8_t value);
    void setDetectionIntegrator(uint8_t value);
    void calibrate();
    uint16_t getTouched();
    bool keyIsTouched(int key);
  
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

    uint16_t readRegister16(char reg) {
      char command[1];
      char buffer[2];
      command[0] = reg;
      i2c->write(address, command, 1, true);
      i2c->read(address, buffer, 2);
      return (buffer[1] << 8) | buffer[0];
    }

    enum Registers {
      CHIP_ID = 0,
      DETECTION_STATUS = 2,
      KEY_STATUS_0_7 = 3,
      KEY_STATUS_8_11 = 4,
      CALIBRATE = 6,
      RESET = 7,
      LP_MODE = 8,
      DI = 11,
      CHARGE_TIME = 15,
      DTHR_KEY0 = 16,
      DTHR_KEY11 = 27,
      KEY1_SIGNAL = 54,
      KEY2_SIGNAL = 56,
      KEY3_SIGNAL = 58,
      KEY4_SIGNAL = 60,
      KEY5_SIGNAL = 62,
      KEY6_SIGNAL = 64,
      KEY7_SIGNAL = 66,
      KEY8_SIGNAL = 68,
      KEY9_SIGNAL = 70,
      KEY10_SIGNAL = 72,
      KEY11_SIGNAL = 74,   
      KEY0_REFERENCE = 76,
      KEY1_REFERENCE = 78,
      KEY2_REFERENCE = 80,
      KEY3_REFERENCE = 82,
      KEY4_REFERENCE = 84,
      KEY5_REFERENCE = 86,
      KEY6_REFERENCE = 88,
      KEY7_REFERENCE = 90,
      KEY8_REFERENCE = 92,
      KEY9_REFERENCE = 94,
      KEY10_REFERENCE = 96,
      KEY11_REFERENCE = 98,
    };
};

#endif
#ifndef __IS31FL3739_H
#define __IS31FL3739_H

// I2C Pull-Ups: 4.7kΩ @ 400kHz, 2kΩ @ 1MHz

#include <mbed.h>
#include <OK_I2C.h>

#define IS31FL3739_ADDR 0x68 // 8-bit addr

const static int IS31FL3739_PWM_MAP_4x16[64] = {
    0x73, 0x63, 0x53, 0x43, 0x33, 0x23, 0x13, 0x03, 0x77, 0x67, 0x57, 0x47, 0x37, 0x27, 0x17, 0x07,
    0x74, 0x64, 0x54, 0x44, 0x34, 0x24, 0x14, 0x04, 0x7E, 0x6E, 0x5E, 0x4E, 0x3E, 0x2E, 0x1E, 0x0E,
    0x75, 0x65, 0x55, 0x45, 0x35, 0x25, 0x15, 0x05, 0x7F, 0x6F, 0x5F, 0x4F, 0x3F, 0x2F, 0x1F, 0x0F,
    0x76, 0x66, 0x56, 0x46, 0x36, 0x26, 0x16, 0x06, 0x80, 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x10
};

class IS31FL3739 {

public:
    IS31FL3739(I2C *i2c_ptr, uint8_t addr = IS31FL3739_ADDR)
    {
        address = addr;
        i2c = i2c_ptr;
    };

    uint8_t address;
    I2C *i2c;

    void init();
    void config(uint8_t data);
    void setGlobalCurrent(uint8_t val);
    void setPWM(uint8_t led, uint8_t pwm);
    void setScale(uint8_t val);

private:
    enum Registers
    {
        CONFIG_REG = 0xA0,
        CURR_CTRL_REG = 0xA1,
        PWM_REG = 0x03,
        SCALING_REG = 0x92,
        PWM_FREQ_REG = 0xB2,
        RESET_REG = 0xCF
    };

    void writeRegister(char reg, char _data1)
    {
        char commands[2];
        commands[0] = reg;
        commands[1] = _data1;

        i2c->write(address, commands, 2);
    }

    char readRegister(char reg)
    {
        char buffer[2];
        buffer[0] = reg;
        i2c->write(address, buffer, 1);
        i2c->read(address, buffer, 1);
        return buffer[0];
    }
};

#endif
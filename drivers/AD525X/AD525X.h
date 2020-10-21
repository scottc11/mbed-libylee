#ifndef __AD525X_H
#define __AD525X_H

#include <mbed.h>

#define AD525X_I2C_ADDR  0x2C // 7-bit

class AD525X
{

public:
    enum Channels
    {
        CHAN_A = 0x0,
        CHAN_B = 0x1,
        CHAN_C = 0x2,
        CHAN_D = 0x3,
    };

    AD525X(I2C *i2c_ptr, int _addr = AD525X_I2C_ADDR)
    {
        i2c = i2c_ptr;
        addr = _addr << 1; // now 8-bit address for mbed
    }

    void setWiper(AD525X::Channels wiper, uint8_t value)
    {
        this->i2cWrite(wiper, value);
    }

    I2C *i2c;
    int addr;

private:
    // AD1, AD0 = I2C device address bits, must match with the logic states at Pins AD1, AD0 
    // R / W = read enable bit at logic high; write enable bit at logic low
    // CMD / REG = command enable bit at logic high; register access bit at logic low
    // EE / RDAC = EEMEM register at logic high; RDAC register at logic low
    // A4, A3, A2, A1, A0 = RDAC / EEMEM register addresses

    void i2cWrite(char reg, char data)
    {
        char buffer[2];
        buffer[0] = reg;
        buffer[1] = data;
        i2c->write(addr, buffer, 2);
    }
};

#endif
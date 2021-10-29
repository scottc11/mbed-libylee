#ifndef __OK_I2C_H
#define __OK_I2C_H

#include "common.h"
#include "I2C.h"

class OK_I2C {
public:
    
    OK_I2C(){};

    char address; // Note: The Arm Mbed API uses 8 bit addresses, so make sure to left-shift 7 bit addresses by 1 bit before passing them.
    I2C *i2c;     // use an #ifdef macro here to determin which underlying framework I2C class should be used

    /** Write to a register
     *  Note: most writes are only valid in stop mode
     *  @param reg - The register to be written
     *  @param data - The data to be written
     */
    void writeRegister(char reg, char _data1, char _data2)
    {
        uint8_t commands[3];
        commands[0] = reg;
        commands[1] = _data1;
        commands[2] = _data2;

        i2c->write(address, commands, 3);
    }

    void writeRegister(char reg, char _data1)
    {
        uint8_t commands[2];
        commands[0] = reg;
        commands[1] = _data1;

        i2c->write(address, commands, 2);
    }

    /** Read from a register
     *  @param reg - The register to read from
     *  @return The register contents
     */
    char readRegister(char reg)
    {
        uint8_t buffer[2];
        buffer[0] = reg;
        i2c->write(address, buffer, 1);
        i2c->read(address, buffer, 1);
        return buffer[0];
    }

    uint16_t readRegister16(char reg) {
        uint8_t buffer[2];
        buffer[0] = reg;
        i2c->write(address, buffer, 1, true);
        i2c->read(address, buffer, 2);
        return two8sTo16(buffer[0], buffer[1]);
    }
};

#endif
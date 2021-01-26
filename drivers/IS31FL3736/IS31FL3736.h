#ifndef __IS31FL3736_H
#define __IS31FL3736_H

#include <mbed.h>

#define IS31FL3736_ADDR 0xA0

class IS31FL3736 {
public:
    IS31FL3736(){};
    void init(uint8_t addr = IS31FL3736_ADDR);
};

#endif
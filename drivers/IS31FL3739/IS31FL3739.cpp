#include "IS31FL3739.h"

void IS31FL3739::init()
{
    for (int i = PWM_REG; i < 0x8F; i++)
    {
        writeRegister(i, 0);
    }

    for (int i = SCALING_REG; i < 0xA0; i++) // hex address 0x92..0x9F
    {
        writeRegister(i, 40);
    }

    this->setGlobalCurrent(127);
    this->config(0b00010001); // 8x8 grid
}

/**
 * @brief takes an LED number between 0 and 63 and sets its PWM value
 * 
 * Each dot has a byte to modulate the PWM duty in 256 steps. 
 * The PWM clock frequency is set by the PWM Frequency Register (B2h). 
 * The value of the PWM Registers decides the average current of each LED noted
*/
void IS31FL3739::setPWM(uint8_t led, uint8_t pwm)
{
    led = led > 63 ? 0 : led;
    writeRegister(IS31FL3739_PWM_MAP_4x16[led], pwm);
}

/**
 * Set the global current
*/
void IS31FL3739::setGlobalCurrent(uint8_t val)
{
    writeRegister(CURR_CTRL_REG, val);
}

/**
 * Scaling register control the DC output current of each CSy. Each CSy has a byte to modulate the scaling in 256 steps.
 * 
 * The value of the Scaling Register decides the peak current of each LED
*/
void IS31FL3739::setScale(uint8_t val)
{

}

/**
 * The Configuration Register sets operating mode of IS31FL3739.

    Bit ------ D7:D4 | D3  | D2:D1 | D0
    Name -----  SWS  | LGC |  OSDE |  SSD
    Default --  0001 |  0  |   00  |   0
*/
void IS31FL3739::config(uint8_t data)
{
    writeRegister(CONFIG_REG, data);
}
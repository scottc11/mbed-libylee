#include "MCP23008.h"

void MCP23008::init(void)
{
    this->setConfig(0b00100000); // disable register auto incremenet
    this->setInterupt(0); // disable interupts
}


/**
 * The IOCON register contains several bits for configuring the device:
    • TheSequentialOperation(SEQOP)controlsthe incrementing function of the Address Pointer. If the Address Pointer is disabled, the Address Pointer does not automatically increment after each byte is clocked during a serial transfer. This feature is useful when it is desired to continuously poll (read) or modify (write) a register.
    • TheSlewRate(DISSLW)bitcontrolstheslew rate function on the SDA pin. If enabled, the SDA slew rate will be controlled when driving from a high to a low.
    • TheHardwareAddressEnable(HAEN)controlbit enables/disables the hardware address pins (A1, A0) on the MCP23S08. This bit is not used on the MCP23008. The address pins are always enabled on the MCP23008.
    • TheOpen-Drain(ODR)controlbit enables/disables the INT pin for open-drain configuration.
    • TheInterruptPolarity(INTPOL)controlbitsets the polarity of the INT pin. This bit is functional only when the ODR bit is cleared, configuring the INT pin as active push-pull.

  * | - | — | SEQOP | DISSLW | HAEN | ODR | INTPOL | — |
*/
void MCP23008::setConfig(char value)
{
    writeRegister(IOCON_REG, value);
}


/**
 * Controls the direction of the data I/O.
 * When a bit is set, the corresponding pin becomes an input. When a bit is clear, the corresponding pin becomes an output
*/
void MCP23008::setDirection(char value)
{
    writeRegister(IODIR_REG, value);
}


/**
 * The GPPU register controls the pull-up resistors for the PORT pins.
 * If a bit is set and the corresponding pin is configured as an input, the corresponding PORT pin is internally pulled up with a 100 k resistor.
*/
void MCP23008::setPullUp(char value)
{
    writeRegister(GPPU_REG, value);
}


/**
 * The IPOL register allows the user to configure the polarity on the corresponding GPIO port bits.
 * If a bit is set, the corresponding GPIO register bit will reflect the inverted value on the pin.
*/
void MCP23008::setInputPolarity(char value)
{
    writeRegister(IPOL_REG, value);
}


/**
 * The GPINTEN register controls the inter- rupt-on-change feature for each pin.
 * If a bit is set, the corresponding pin is enabled for interrupt-on-change. 
 * The DEFVAL and INTCON registers must also be configured if any pins are enabled for interrupt-on-change.
**/
void MCP23008::setInterupt(char value)
{
    writeRegister(GPINTEN_REG, value);
}


/**
 * This register allows the user to configure the polarity on the corresponding GPIO port bits.
 * If a bit is set, the corresponding GPIO register bit will reflect the inverted value on the pin.
 * for some reason, I think the default is inverted...
**/
void MCP23008::writePins(char value)
{
    writeRegister(GPIO_REG, value);
}

/**
 * @return 8-bit value representing all 8 pins
*/ 
char MCP23008::readPins()
{
    return readRegister(GPIO_REG);
}


int MCP23008::getBitStatus(int byte, int bit)
{
    return (byte & (1 << bit));
}

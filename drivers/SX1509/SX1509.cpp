
#include "SX1509.h"

void SX1509::init() {

  this->reset();

  this->i2cWrite(REG_CLOCK, 0b01010000);
  this->i2cWrite(REG_MISC, 0b01110000); // very important for LED Driver mode that this config gets set
}

void SX1509::pinMode(int pin, Mode mode) {
  if (mode == INPUT) {

  } else if (mode == OUTPUT) {

  } else if (mode == ANALOG_OUTPUT) {
    ledConfig(pin);
  }
}

// software reset
void SX1509::reset() {
  this->i2cWrite(REG_RESET, 0x12);
  this->i2cWrite(REG_RESET, 0x34);
}

/** 
 * LED Driver Configuration

Please note that in this configuration the IO must be programmed as open drain output (RegOpenDrain)
with no pull-up (RegPullUp) and input buffer must be disabled (RegInputBufferDisable).

Below are the steps required to use the LED driver with the typical LED connection described §6.2:

- Disable input buffer (RegInputDisable)
- Disable pull-up (RegPullUp)
- Enable open drain (RegOpenDrain)
- Set direction to output (RegDir) – by default RegData is set high => LED OFF
- Enable oscillator (RegClock)
- Configure LED driver clock and mode if relevant (RegMisc)
- Enable LED driver operation (RegLEDDriverEnable)
- Configure LED driver parameters (RegTOn, RegIOn, RegOff, RegTRise, RegTFall)
- Set RegData bit low => LED driver started 
*/

void SX1509::ledConfig(int pin)
{
  int hotConfig; // the current value of the register
  int reg;       // register to read/write
  int bank = (pin < 8) ? 1 : 0; // for bank A increment all commands by 1, else don't increment
  int pinNum = (pin < 8) ? pin : pin - 8;

  reg = REG_INPUT_DISABLE_B + bank;
  hotConfig = this->i2cRead(reg);
  this->i2cWrite(reg, bitWrite(hotConfig, pinNum, 1)); // Disable the input buffer (HIGH)

  reg = REG_PULL_UP_B + bank;
  hotConfig = this->i2cRead(reg);
  this->i2cWrite(reg, bitWrite(hotConfig, pinNum, 0)); // remove internal pull-up (LOW)
  
  reg = REG_OPEN_DRAIN_B + bank;
  hotConfig = this->i2cRead(reg);
  this->i2cWrite(reg, bitWrite(hotConfig, pinNum, 1)); // set IO to open drain (HIGH)

  reg = REG_DIR_B + bank;
  hotConfig = this->i2cRead(reg);
  this->i2cWrite(reg, bitWrite(hotConfig, pinNum, 1)); // Set IO direction to output (LOW)

  reg = REG_LED_DRIVER_ENABLE_B + bank;
  hotConfig = this->i2cRead(reg);
  this->i2cWrite(reg, bitWrite(hotConfig, pinNum, 1));

  reg = REG_DATA_B + bank;
  hotConfig = this->i2cRead(reg);
  this->i2cWrite(reg, bitWrite(hotConfig, pinNum, 0)); // setting data LOW means LED Driver started
}

void SX1509::analogWrite(int value) {

  if (value == 1) {
    this->i2cWrite(REG_DATA_B, 0x00); 
  } else {
    this->i2cWrite(REG_DATA_B, 0xFF);
  }
}

void SX1509::setDirection(int dir) {
  if (dir == INPUT) {
    this->i2cWrite(REG_DIR_B, 0xFF);
  } else {
    this->i2cWrite(REG_DIR_B, 0x00);
  }
}

void SX1509::setPWM(int pin, int value)
{
  this->i2cWrite(REG_I_ON[pin], value); // sets the PWM / brightness
}


// configure Clock

/**

Bit 7 = unused

Oscillator frequency (fOSC) source (bits 6:5)
00 : OFF. LED driver, keypad engine and debounce features are disabled.
01 : External clock input (OSCIN)
10 : Internal 2MHz oscillator
11 : Reserved

OSCIO pin function (bit 4)
0 : OSCIO is an input (OSCIN)
1 : OSCIO is an output (OSCOUT)

Frequency of the signal output on OSCOUT pin (bits 3:0):
0x0 : 0Hz, permanent "0" logical level (GPO)
0xF : 0Hz, permanent "1" logical level (GPO)
Else : fOSCOUT = fOSC/(2^(RegClock[3:0]-1))

*/

/* Input Debouncer

Each input can be individually debounced by setting corresponding bits in RegDebounce register. At power up the debounce function is disabled. After enabling the debouncer, the change of the input value is accepted only if the input value is identical at two consecutive sampling times.
The debounce time common to all IOs can be set in RegDebounceConfig register from 0.5 to 64ms (fOSC = 2MHz)
*/
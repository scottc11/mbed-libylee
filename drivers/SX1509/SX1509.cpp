
#include "SX1509.h"

void SX1509::init() {
  // this->i2cWrite();
}

void SX1509::configurePinIO(char _port, int direction) {
  if (direction == INPUT) {

  }
}


/**
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

void SX1509::ledConfig() {
  this->i2cRead(REG_DATA_B);
  this->i2cWrite(REG_INPUT_DISABLE_B, 0xFF);
  this->i2cWrite(REG_PULL_UP_B, 0x00);
  this->i2cWrite(REG_OPEN_DRAIN_B, 0x00);
  this->setDirection(OUTPUT);
  this->i2cWrite(REG_CLOCK, 0b01010000);  // internal OSC, OSCIO Pin output, 
  this->toggleLEDDriver(false);
  this->i2cWrite(REG_DATA_B, 0x00);
}

void SX1509::ledWrite(int value) {

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

void SX1509::toggleLEDDriver(bool state) {
  if (state) {
    this->i2cWrite(REG_LED_DRIVER_ENABLE_B, 0xFF); // currently used to turn on/off leds
  } else {
    this->i2cWrite(REG_LED_DRIVER_ENABLE_B, 0x00); // currently used to turn on/off leds
  }
  
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
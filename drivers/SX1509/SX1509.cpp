
#include "SX1509.h"

void SX1509::init(bool extClock /*false*/) {

  this->reset();
  wait_ms(1);
  int clockConfig = extClock ? 0b00100000 : 0b01010001;  // use either internal or external clock signal
  //  fOSCOUT = fOSC/(2^(RegClock[3:0]-1))
  //  ClkX = fOSC/(2^(RegMisc[6:4]-1))
  this->i2cWrite(REG_CLOCK, clockConfig);
  int regMiscConfig = (DriverMode::LINEAR | ClockSpeed::EXTRA_SLOW);
  regMiscConfig = 0b01110100;
  this->i2cWrite(REG_MISC, regMiscConfig); // very important for LED Driver mode that this config gets set
}

/**
 * implementation not fully complete. 
 */ 
void SX1509::setDriverMode(bool linear)
{
  int hotValue = this->i2cRead(REG_MISC);
  if (linear) {
    hotValue = bitClear(hotValue, 7);
  } else {
    hotValue = bitSet(hotValue, 7);
  }
  this->i2cWrite(REG_MISC, hotValue);
};

void SX1509::pinMode(int pin, Mode mode) {
  if (mode == INPUT) {

  } else if (mode == OUTPUT) {

  } else if (mode == ANALOG_OUTPUT) {
    ledConfig(pin);
  }
}

/** Software Reset
 * Writing consecutively 0x12 and 0x34 to RegReset register will reset all registers to their default values.
*/
void SX1509::reset() {
  this->i2cWrite(REG_RESET, 0x12);
  this->i2cWrite(REG_RESET, 0x34);
}

/** LED Driver Configuration

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

/** LED PWM
 * ON Intensity of IO[X]
 * 
 * Linear mode : IOnX = RegIOnX
 * Logarithmic mode (fading capable IOs only) : IOnX = f(RegIOnX)
*/
void SX1509::setPWM(int pin, int value)
{
  this->i2cWrite(REG_I_ON[pin], value); // sets the PWM / brightness
}

/** LED Driver Blink Mode
 * 
 * Invoked when TOnX != 0 and TOffX != 0. (they both default to 0x00)
 * If the I/O doesn’t support fading the LED intensity will step directly to the IOnX/IOffX value.
 * When RegData(X) is cleared, the LED will complete any current ramp, and then stay at minimum intensity

TOnX:
0 : Infinite (Static mode, TOn directly controlled by RegData, Cf §4.8.2)
1 - 15 : TOnX = 64 * RegTOnX * (255/ClkX)
16 - 31 : TOnX = 512 * RegTOnX * (255/ClkX)

ToffX:


*/
void SX1509::setBlink(int pin, uint8_t onTime, uint8_t offTime, uint8_t onIntensity, uint8_t offIntensity)
{
  this->i2cWrite(REG_T_ON[pin], (onTime > 31) ? 31 : onTime);
  this->setPWM(pin, onIntensity);
  
  uint8_t offValue = offTime << 3; // offTime is 5 bits, from bit 7:3
  offValue |= (offIntensity & 0x07);      // offIntensity is a 3 bit number, from bit 2:0
  this->i2cWrite(REG_OFF[pin], offValue);
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
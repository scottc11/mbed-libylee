#include "TLC59116.h"

void TLC59116::initialize() {
  setMode(0b00000000, 0b00000000);
  
  // IREF affects the voltage at the REXT terminal and indirectly the reference current (Iref) flowing through the external resistor at terminal REXT
  writeRegister(IREF, 0xC0); // set voltage gain to 0.5 (default: 0.992)

  writeRegister(GRPPWM, 0x00);
  writeRegister(GRPFREQ, 0x00);

  setAllOutputsLow();
}

void TLC59116::setLedOutput(int led, LedState state, int pwm /*=0*/) {
  
  switch (led) {
    case 0:
      led0States &= 0b11111100; // set bits 0 and 1 to 0/LOW
      led0States |= state; // apply desired LED output state (ON, OFF, PWM, GROUP)
      writeRegister(LEDOUT0, led0States);
      break;
    case 1:
      led0States &= 0b11110011;
      led0States |= (state << 2);
      writeRegister(LEDOUT0, led0States);
      break;
    case 2:
      led0States &= 0b11001111;
      led0States |= (state << 4);
      writeRegister(LEDOUT0, led0States);
      break;
    case 3:
      led0States &= 0b00111111;
      led0States |= (state << 6);
      writeRegister(LEDOUT0, led0States);
      break;
    case 4:
      led1States &= 0b11111100;
      led1States |= state;
      writeRegister(LEDOUT1, led1States);
      break;
    case 5:
      led1States &= 0b11110011;
      led1States |= (state << 2);
      writeRegister(LEDOUT1, led1States);
      break;
    case 6:
      led1States &= 0b11001111;
      led1States |= (state << 4);
      writeRegister(LEDOUT1, led1States);
      break;
    case 7:
      led1States &= 0b00111111;
      led1States |= (state << 6);
      writeRegister(LEDOUT1, led1States);
      break;
    case 8:
      led2States &= 0b11111100;
      led2States |= state;
      writeRegister(LEDOUT2, led2States);
      break;
    case 9:
      led2States &= 0b11110011;
      led2States |= (state << 2);
      writeRegister(LEDOUT2, led2States);
      break;
    case 10:
      led2States &= 0b11001111;
      led2States |= (state << 4);
      writeRegister(LEDOUT2, led2States);
      break;
    case 11:
      led2States &= 0b00111111;
      led2States |= (state << 6);
      writeRegister(LEDOUT2, led2States);
      break;
    case 12:
      led3States &= 0b11111100;
      led3States |= state;
      writeRegister(LEDOUT3, led3States);
      break;
    case 13:
      led3States &= 0b11110011;
      led3States |= (state << 2);
      writeRegister(LEDOUT3, led3States);
      break;
    case 14:
      led3States &= 0b11001111;
      led3States |= (state << 4);
      writeRegister(LEDOUT3, led3States);
      break;
    case 15:
      led3States &= 0b00111111;
      led3States |= (state << 6);
      writeRegister(LEDOUT3, led3States);
      break;
    default:
      break;
  }
  
  if (state == LedState::PWM) {
    setLedPWM(led, pwm);
  }
}


/**
 * sets all 16 pins to ON or OFF, based on a 16 bit integer
 * ex. 1111 1111 1111 1111 == 65535 == all LEDs ON
*/
void TLC59116::setLedOutput16(uint16_t value) {
}


/**
 * write a function which will get called after (or before) every I2C transfer to determine how many LEDs
 * are going to be turned on, and have it determing the appropriate voltage gain to power those leds.
 * Class initialization will take an additional argument informing the "max power" usage to be used. This
 * value will be used as a "ceiling"
 * 
 * basically the more LEDs lit, the dimmer they become.
*/ 

/**
 * A 97-kHz fixed frequency signal is used for each output.
 * Duty cycle is controlled through 256 linear steps
 * 00h (0% duty cycle = LED output off) to FFh (99.6% duty cycle = LED output at maximum brightness)
 * 
*/ 
void TLC59116::setLedPWM(int led, int value) {
  writeRegister(pwmRegisters[led], value);
}


/**
 * General brightness for the 16 outputs is controlled through 256 linear steps.
 * 00h (0% duty cycle = LED output off) to FFh (99.6% duty cycle = maximum brightness)
*/
void TLC59116::setGroupPWM(int value) {
  writeRegister(GRPPWM, value);
}


/**
 * Group Frequency Register (GRPFREQ)

 * GRPFREQ is used to program the global blinking period when the DMBLNK bit (MODE2 register) is equal to 1. 
 * Value in this register is a Don't care when DMBLNK = 0. 
 * This is applicable to LED output programmed with LDRx = 11 (LEDOUT0, LEDOUT1, LEDOUT2 and LEDOUT3 registers).
 * The blinking period is controlled through 256 linear steps from 00h (41 ms, frequency 24 Hz) to FFh (10.73 s). 
 * Global blinking period (seconds) = (GFRQ[7:0] + 1) / 24
*/ 
void TLC59116::setGroupFREQ(int value) {
  writeRegister(GRPFREQ, value);
}


/** Mode Register 1 and 2
 * 
 * MODE 1 --> | AI2 R | AI1 R | AI0 R | OSC R/W | SUB1 R/W | SUB2 R/W | SUB3 R/W  | ALLCALL R/W |
 * default: 0001 0001
 * 
 * MODE 2 --> | EFCLR | N/A | DMBLNK | N/A | OCH | N/A | N/A |
 * DMBLNK --> group control dimming or blinking
 * default: 0000 0000
*/ 
void TLC59116::setMode(int mode1Value, int mode2Value) {
  writeRegister(MODE1, mode1Value);
  writeRegister(MODE2, mode2Value);
}



/** Output Gain Control Register
 * 
 * IREF affects the voltage at the REXT terminal and indirectly the reference current (Iref) 
 * flowing through the external resistor at terminal REXT
*/
void TLC59116::setVoltageGain(int gain) {
  writeRegister(IREF, 0xC0); // set voltage gain to 0.5 (default: 0.992)
}

void TLC59116::setAllOutputsLow() {
  led0States = 0x00;
  led1States = 0x00;
  led2States = 0x00;
  led3States = 0x00;
  writeRegister(LEDOUT0, 0x00);
  writeRegister(LEDOUT1, 0x00);
  writeRegister(LEDOUT2, 0x00);
  writeRegister(LEDOUT3, 0x00);
}

void TLC59116::setAllOutputsHigh() {
  led0States = 0x11;
  led1States = 0x11;
  led2States = 0x11;
  led3States = 0x11;
  writeRegister(LEDOUT0, 0x11);
  writeRegister(LEDOUT1, 0x11);
  writeRegister(LEDOUT2, 0x11);
  writeRegister(LEDOUT3, 0x11);
}

void TLC59116::enableDimmingMode() {
  int config = readRegister(MODE2);
  config = bitwise_clear_bit(config, 5);
  writeRegister(MODE2, config);  
}

void TLC59116::enableBlinkingMode() {
  int config = readRegister(MODE2);
  config = bitwise_set_bit(config, 5);
  writeRegister(MODE2, config);
}
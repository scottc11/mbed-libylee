#include "TLC59116.h"

void TLC59116::initialize() {
  setMode(0b00000000, 0b00000000);
  
  // IREF affects the voltage at the REXT terminal and indirectly the reference current (Iref) flowing through the external resistor at terminal REXT
  writeRegister(IREF, 0xC0); // set voltage gain to 0.5 (default: 0.992)

  writeRegister(GRPPWM, 0x00);
  writeRegister(GRPFREQ, 0x00);

  writeRegister(LEDOUT0, 0b11111111);
  writeRegister(LEDOUT1, 0b11111111);
  writeRegister(LEDOUT2, 0b11111111);
  writeRegister(LEDOUT3, 0b11111111);

  writeRegister(PWM0, 0x00);
  writeRegister(PWM1, 0x00);
  writeRegister(PWM2, 0x00);
  writeRegister(PWM3, 0x00);
  writeRegister(PWM4, 0x00);
  writeRegister(PWM5, 0x00);
  writeRegister(PWM6, 0x00);
  writeRegister(PWM7, 0x00);

}

void TLC59116::setLedOutput(int led, LedState state, int pwm /*=0*/) {
  
  switch (led) {
    case 0:
      led0States |= state;
      writeRegister(LEDOUT0, led0States);
      break;
    case 1:
      led0States |= (state << 2);
      writeRegister(LEDOUT0, led0States);
      break;
    case 2:
      led0States |= (state << 4);
      writeRegister(LEDOUT0, led0States);
      break;
    case 3:
      led0States |= (state << 6);
      writeRegister(LEDOUT0, led0States);
      break;
    case 4:
      led1States |= state;
      writeRegister(LEDOUT1, led1States);
      break;
    case 5:
      led1States |= (state << 2);
      writeRegister(LEDOUT1, led1States);
      break;
    case 6:
      led1States |= (state << 4);
      writeRegister(LEDOUT1, led1States);
      break;
    case 7:
      led1States |= (state << 6);
      writeRegister(LEDOUT1, led1States);
      break;
    case 8:
      led2States |= state;
      writeRegister(LEDOUT2, led2States);
      break;
    case 9:
      led2States |= (state << 2);
      writeRegister(LEDOUT2, led2States);
      break;
    case 10:
      led2States |= (state << 4);
      writeRegister(LEDOUT2, led2States);
      break;
    case 11:
      led2States |= (state << 6);
      writeRegister(LEDOUT2, led2States);
      break;
    case 12:
      led3States |= state;
      writeRegister(LEDOUT3, led3States);
      break;
    case 13:
      led3States |= (state << 2);
      writeRegister(LEDOUT3, led3States);
      break;
    case 14:
      led3States |= (state << 4);
      writeRegister(LEDOUT3, led3States);
      break;
    case 15:
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
#include "QT2120.h"
#include "BitwiseMethods.h"

void QT2120::init() {
  int id = readRegister(CHIP_ID);

  if (id == QT2120_CHIP_ID) {
    connected = true;
  } else {
    connected = false;
    return;
  }

  // int status = readRegister(DETECTION_STATUS);

  setDetectionIntegrator(2);
  setDetectThreshold(5);
  writeRegister(LP_MODE, 1); // deactivate low popwer mode
  calibrate();
}

/**
 * These bits indicate which keys are in detection, if any. 
 * Touched keys report as 1, untouched or disabled keys report as 0. 
 * A change in these bytes will cause the CHANGE line to trigger.
 * 
 * register Address 3: keys 0 - 7   --> | KEY7 | KEY6 | KEY5 | KEY4 | KEY3  | KEY2  | KEY1 | KEY0 |
 * register Address 4: keys 8 - 11  --> |  --  |  --  |  --  |  --  | KEY11 | KEY10 | KEY9 | KEY8 |
*/ 
uint16_t QT2120::getTouched() {
  int byte1 = readRegister(4);
  int byte2 = readRegister(3);
  touched = (byte1 << 8) | byte2;
  return touched;
}

bool QT2120::keyIsTouched(int key) {
  if (bitRead(touched, key)) {
    return true;
  } else {
    return false;
  }
}

/**
 * Writing any nonzero value into this address triggers the device to start a calibration cycle. 
 * The CALIBRATE flag in the detection status register is set when the calibration begins and clears when the calibration has finished.
*/ 
void QT2120::calibrate() {
  writeRegister(CALIBRATE, 1);
}


/**
 * DI: Allows the DI level to be set for each key.
 * This 8-bit value controls the number of consecutive measurements that must be confirmed as 
 * having passed the key threshold before that key is registered as being in detect. 
 * The minimum value for the DI filter is 1. A settings of 0 for the DI also defaults to 1.
 * DEFAULT: 4 (maximum = 32)
*/ 
void QT2120::setDetectionIntegrator(uint8_t value) {
  writeRegister(DI, value);
}

/**
 * Prolongs the charge-transfer period of signal acquisition by 1 μs per count. 
 * Allows full charge-transfer for keys with heavy Rs / Cx loading.
*/
void QT2120::setChargeTime(uint8_t value) {
  writeRegister(CHARGE_TIME, value);
}


/**
 * SET ALL KEYS DTHR
 * DTHR KEY 0 – DTHR KEY 11: these 8-bit values set the threshold value for each key to register a detection.
 * Default: 10 counts
 * Note: Do not use a setting of 0 as this causes a key to go into detection when its signal is equal to its reference.
*/ 
void QT2120::setDetectThreshold(uint8_t value) {
  for (int reg = DTHR_KEY0; reg < DTHR_KEY11; reg++) {
    writeRegister(reg, value);
  }
}
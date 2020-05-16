#include "CY8CMBR3116.h"

void CY8CMBR3116::init() {
  this->readRegister(SENSOR_EN); // wake device
  volatile int deviceId = this->readRegister16(DEVICE_ID);
  
  while (connected == false) {
    deviceId = this->readRegister16(DEVICE_ID);
    connected = deviceId == 2565 ? true : false;
  }
  
}
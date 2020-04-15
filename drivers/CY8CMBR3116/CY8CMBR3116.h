#ifndef __CY8CMBR3116_H
#define __CY8CMBR3116_H

/**
 * NOTE: All registers mentioned in the document are little endian.
*/ 

#include <mbed.h>

#define CY8CMBR3116_I2C_ADDRESS     0x37 << 1

class CY8CMBR3116 {

  public:
    CY8CMBR3116(I2C *_i2c, int _address = CY8CMBR3116_I2C_ADDRESS) {
      i2c = _i2c;
      address = _address;
    }

    I2C *i2c;
    int address;
    bool connected;
    
    void init();

  private:
    
    void writeRegister(char reg, char data) {    
      char buffer[2];
      buffer[0] = reg;
      buffer[1] = data;
      i2c->start();
      i2c->write(address, buffer, 2);
      i2c->stop();
    }

    char readRegister(char reg) {
      char command[1];
      char buffer[1];
      command[0] = reg;
      i2c->write(address, command, 1, true);
      i2c->read(address, buffer, 1);
      return buffer[0];
    }

    uint16_t readRegister16(char reg) {
      char command[1];
      char buffer[2];
      command[0] = reg;
      i2c->write(address, command, 1, true);
      i2c->read(address, buffer, 2);
      return (buffer[1] << 8) | buffer[0];
    }

    enum Registers {
      SENSOR_EN           = 0x00,   // Capacitive sensor enable/disable configuration
      FSS_EN              = 0x02,
      TOGGLE_EN	          = 0x04,   // GPO toggle enable/disable
      LED_ON_EN	          = 0x06,   // GPO extended LED ON duration enable/disable
      SENSITIVITY0        = 0x08,   // Sensitivities (units: counts/pF) for button sensors 0 - 3
      SENSITIVITY1			  = 0x09,   // ...sensors 4 - 7
      SENSITIVITY2			  = 0x0A,   // ...sensors 8 - 11
      SENSITIVITY3			  = 0x0B,   // ...sensors 12 - 15
      BASE_THRESHOLD0			= 0x0C,   // This threshold is applied when sensor 0 is configured as a button sensor and automatic threshold mode is disabled
      BASE_THRESHOLD1			= 0x0D,
      FINGER_THRESHOLD2		= 0x0E,
      FINGER_THRESHOLD3		= 0x0F,
      FINGER_THRESHOLD4		= 0x10,
      FINGER_THRESHOLD5		= 0x11,
      FINGER_THRESHOLD6		= 0x12,
      FINGER_THRESHOLD7		= 0x13,
      FINGER_THRESHOLD8		= 0x14,
      FINGER_THRESHOLD9		= 0x15,
      FINGER_THRESHOLD10	= 0x16,
      FINGER_THRESHOLD11	= 0x17,
      FINGER_THRESHOLD12	= 0x18,
      FINGER_THRESHOLD13	= 0x19,
      FINGER_THRESHOLD14	= 0x1A,
      FINGER_THRESHOLD15	= 0x1B,
      SENSOR_DEBOUNCE			= 0x1C,
      BUTTON_HYS				  = 0x1D,
      BUTTON_BUT				  = 0x1E,
      BUTTON_LBR				  = 0x1F,
      BUTTON_NNT				  = 0x20,
      BUTTON_NT				    = 0x21,
      PROX_EN					    = 0x26,
      PROX_CFG				    = 0x27,
      PROX_CFG2				    = 0x28,
      PROX_TOUCH_TH0			= 0x2A,
      PROX_TOUCH_TH1			= 0x2C,
      PROX_HYS				    = 0x30,
      PROX_BUT				    = 0x31,
      PROX_LBR				    = 0x32,
      PROX_NNT				    = 0x33,
      PROX_NT					    = 0x34,
      PROX_POSITIVE_TH0		= 0x35,
      PROX_POSITIVE_TH1		= 0x36,
      PROX_NEGATIVE_TH0		= 0x39,
      PROX_NEGATIVE_TH1		= 0x3A,
      LED_ON_TIME				  = 0x3D,
      BUZZER_CFG				  = 0x3E,
      BUZZER_ON_TIME			= 0x3F,
      GPO_CFG					    = 0x40,
      PWM_DUTYCYCLE_CFG0	= 0x41,
      PWM_DUTYCYCLE_CFG1	= 0x42,
      PWM_DUTYCYCLE_CFG2	= 0x43,
      PWM_DUTYCYCLE_CFG3	= 0x44,
      PWM_DUTYCYCLE_CFG4	= 0x45,
      PWM_DUTYCYCLE_CFG5	= 0x46,
      PWM_DUTYCYCLE_CFG6	= 0x47,
      PWM_DUTYCYCLE_CFG7	= 0x48,
      SPO_CFG					    = 0x4C,
      DEVICE_CFG0				  = 0x4D,
      DEVICE_CFG1				  = 0x4E,
      DEVICE_CFG2				  = 0x4F,
      I2C_ADDR				    = 0x51,
      REFRESH_CTRL			  = 0x52,
      STATE_TIMEOUT			  = 0x55,
      SLIDER_CFG				  = 0x5D,
      SLIDER1_CFG				  = 0x61,
      SLIDER1_RESOLUTION	= 0x62,
      SLIDER1_THRESHOLD		= 0x63,
      SLIDER2_CFG				  = 0x67,
      SLIDER2_RESOLUTION	= 0x68,
      SLIDER2_THRESHOLD		= 0x69,
      SLIDER_DEBOUNCE			= 0x6F,
      SLIDER_BUT				  = 0x70,
      SLIDER_LBR				  = 0x71,
      SLIDER_NNT				  = 0x72,
      SLIDER_NT				    = 0x73,
      CONFIG_CRC				  = 0x7E,
      GPO_OUTPUT_STATE		= 0x80,
      SENSOR_ID				    = 0x82,
      CTRL_CMD				    = 0x86,
      DEVICE_ID           = 0x90,   // default: 2565
      BUTTON_STATUS			  = 0xAA,
      PROX_STAT           = 0xAE,
      GPO_DATA            = 0xDA,   // GPO state values that the device is outputting. If GPOs are outputting PWM, these bits reflect the duty cycle selection (LOW or HIGH). Bits corresponding to disabled GPO are 0
    };
};

#endif
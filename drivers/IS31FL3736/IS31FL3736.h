/**
 * IS31FL3736B DESCRIPTION
 * 
 * The IS31FL3736B is a general purpose 12×8 LEDs matrix driver with 1/12 cycle rate.
 * The device can be programmed via an I2C compatible interface. 
 * Each LED can be dimmed individually with 8-bit PWM data which allowing 256 steps of linear dimming.
 * 
 * IS31FL3736B features 3 Auto Breathing Modes which are noted as ABM-1, ABM-2 and ABM-3.
 * For each Auto Breathing Mode, there are 4 timing characters which include current rising / holding / falling / off
   time and 3 loop characters which include Loop-Beginning / Loop-Ending / Loop-Times.
 * Every LED can be configured to be any Auto Breathing Mode or No-Breathing Mode individually.
*/

#ifndef __IS31FL3736_H
#define __IS31FL3736_H

#include <mbed.h>
#include <OK_I2C.h>


#define IS31FL3736_ADDR 0xA0 // 8-bit addr

class IS31FL3736 : public OK_I2C {
public:
    IS31FL3736(I2C *i2c_ptr, uint8_t addr = IS31FL3736_ADDR) {
        address = addr;
        i2c = i2c_ptr;
    };

    uint8_t currPage;

    /**
     * Note: FDh is locked when power up, need to unlock this register before write command to it
     * 
     * The Command Register should be configured first after writing in the slave address to choose the available register.
     * Then write data in the choosing register. Power up default state is “0000 0000”.
     * 
     * For example, when write “0000 0001” in the Command Register (FDh), the data which writing after will be stored in the PWM Register (Page1).
    */
    void init() {
        
        allLEDsOn();
        setPWM(127);
        // setLED(8, 9, true);

        i2cWrite(0xFE, 0xc5); // unlock FDh
        i2cWrite(0xFD, 0x03); // write page 3
        i2cWrite(0x00, 0x01); // normal operation
        i2cWrite(0x01, 0xFF); // set global current
    }

    /**
     * FEh Command Register Write Lock
     * 
     * To select the PG0~PG3, need to unlock this register first, with the purpose to avoid misoperation of this register. 
     * When FEh is written with 0xC5, FDh is allowed to modify once, after the FDh is modified the FEh will reset to be 0x00 at once.
     * 
     * CRWL Command Register WriteLock
     * 0x00 FDh write disable
     * 0xC5 FDh write enable once
    */
    void unlockCommandReg() {
        i2cWrite(CRWL_REG, 0xC5);
    }

    /**
     * The LED On/Off Registers store the on or off state of each LED in the matrix.
     * For example, if 00h=0x01, SW1~CS1 will open, if 01h=0x01, SW1~CS5 will open.
    */
    void setLED(int x, int y, bool state) {
        if (currPage != PAGE_0) {
            unlockCommandReg();
            setPage(PAGE_0);
        }
        i2cWrite(23, 0x55); //open all led
    }

    void allLEDsOn() {
        unlockCommandReg();
        setPage(PAGE_0); // write page 0

        for (uint8_t i = 0; i < 24; i++) {
            i2cWrite(i, 0x55); // open all led
        }
    }

    void allLEDsOff() {
        unlockCommandReg();
        setPage(PAGE_0); // write page0

        for (uint8_t i = 0; i < 24; i++) {
            i2cWrite(i, 0x00); // close all led
        }
    }

    void Select_Sw_Pull(uint8_t data) {
        unlockCommandReg();
        i2cWrite(COMMAND_REG, PAGE_3); // write page 3 (COULD BE WRONG)
        i2cWrite(SW_PULLUP_SEL, data);
    }

    void Select_Cs_Pull(uint8_t data) {
        unlockCommandReg();
        i2cWrite(COMMAND_REG, PAGE_3); // write page 3 (COULD BE WRONG)
        i2cWrite(CS_PULLDOWN_SEL, data);
    }

    void setPWM(uint8_t value) {
        unlockCommandReg();            // unlock FDh
        i2cWrite(COMMAND_REG, PAGE_1); // write page 1

        for (int i = 0; i < 192; i = i + 2)
        {
            i2cWrite(i, value); // write all pwm
        }
    }
    
    void setGlobalCurrent(uint8_t data) {
        unlockCommandReg();
        setPage(PAGE_3);
        i2cWrite(GLB_CURR_CTRL, 0xFF); // set global current
    }

    /**
     * The Configuration Register sets operating mode of IS31FL3736B.
     * 
     * Bit     | D7:D6 | D5:D3 |  D2  |  D1  | D0
     * Name    | SYNC  |  PFS  | OSD  | B_EN | SSD
     * Default | 00    |  000  |  0   |  0   |  0
     * 

     * When SYNC bits are set to “01”, the IS31FL3736B is configured as the master clock source and the SYNC pin will 
       generate a clock signal distributed to the clock slave devices. To be configured as a clock slave deviceand accept
       an external clock input the slave device’s SYNC bits must be set to “10”.
        
        SYNC Synchronize Configuration
            00/11 High Impedance
            01 Master
            10 Slave
     
     * The PFS bit selects a fixed PWM operating frequency for all CSx, when PFS set “000”, the PWM frequency is 8.4kHz,
     * when PFS set to “010”, the PWM frequency is 26.7kHz.
    
        PFS PWM Frequency Setting Bit
            000 8.4kHz (default)
            010 26.7kHz
            001 4.2kHz
            011 2.1kHz 100 1.05kHz

     * When OSD set high, open/short detection will be trigger once, the user could trigger OS detection again by set OSD from 0 to 1.
        
        OSD Open/Short Detection Enable Bit
            0 Disable open/short detection
            1 Enable open/short detection
     
     * When B_EN enable, those dots select working in ABM-x mode will start to run the pre-established timing. If it is disabled, 
     * all dots work in PWM mode

        B_EN Auto Breath Enable
            0 PWM Mode Enable
            1 Auto Breath Mode Enable
     
     * When SSD is “0”, IS31FL3736B works in software shutdown mode and to normal operate the SSD bit should set to “1”.

        SSD Software Shutdown Control
            0 Software shutdown
            1 Normal operation
    */
    void setOperationMode() {

    }

    // must unlock before
    void setPage(int page) {
        currPage = page;
        i2cWrite(COMMAND_REG, page);
    }

    void Brightness_Level(uint8_t data);
    void PWM_ALL_ON(void);
    void PWM_ALL_OFF(void);
    void Reset_Pwm_Value(void);
    void Reset_Resigester(void);
    void ABMS_Control(uint8_t data);
    void ABMS_Reset(void);
    void ABMS_Breath_Time(void);

private:
    enum Registers
    {
        COMMAND_REG = 0xFD,
        CRWL_REG = 0xFE, // Command Register Write Lock
        INT_MASK_REG = 0xF0,
        INT_STATUS_REG = 0xF1,
        LED_ON_OFF_REG = 0x00, // 00h ~ 17h (0 ~ 23)
        LED_OPEN_REG = 0x18,   // 18h ~ 2Fh (24 ~ 47)
        LED_SHORT_REG = 0x30,  // 30h ~ 47h (48 ~ 71)
    };

    enum ConfigRegisters
    {
        CONFIG_MODE = 0x00,          // (Configuration Register) Configure the operation mode

        GLB_CURR_CTRL = 0x01, // (Global Current Control Register )Set the global current
        
        TIME_UPDATE = 0x0E,   // (Time Update Register) Update the setting of 02h ~ 0Dh registers

        SW_PULLUP_SEL = 0x0F, // (SWy Pull-Up Resistor Selection Register) Set the pull-up resistor for SWy

        CS_PULLDOWN_SEL = 0x10, // (CSx Pull-Down Resistor Selection Register) Set the pull-down resistor for CSx

        RESET = 0x11          // (Reset Register) Reset all register to POR state
    };

    enum Pages
    {
        PAGE_0 = 0x00, // LED Control Register
        PAGE_1 = 0x01, // PWM Register
        PAGE_2 = 0x02, // Auto Breath Mode Register
        PAGE_3 = 0x03  // Function Register
    };
};

#endif




// AUTO_BREATH_CTRL1 = 0x02, // (Auto Breath Control Register 1 of ABM-1 )Set fade in and hold time for breath function of ABM-1


// AUTO_BREATH_CTRL2 = 0x03, // (Auto Breath Control Register 2 of ABM-1 )Set the fade out and off time for breath function of ABM-1


// AUTO_BREATH_CTRL3 = 0x04, // (Auto Breath Control Register 3 of ABM-1 )Set loop characters of ABM-1


// AUTO_BREATH_CTRL4 = 0x05, // (Auto Breath Control Register 4 of ABM-1 )Set loop characters of ABM-1


// AUTO_BREATH_CTRL5 = 0x06, // (Auto Breath Control Register 1 of ABM-2 )Set fade in and hold time for breath function of ABM-2


// AUTO_BREATH_CTRL6 = 0x07, // (Auto Breath Control Register 2 of ABM-2 )Set the fade out and off time for breath function of ABM-2


// AUTO_BREATH_CTRL7 = 0x08, // (Auto Breath Control Register 3 of ABM-2 )Set loop characters of ABM-2


// AUTO_BREATH_CTRL8 = 0x09, // (Auto Breath Control Register 4 of ABM-2 )Set loop characters of ABM-2


// AUTO_BREATH_CTRL9 = 0x0A, // (Auto Breath Control Register 1 of ABM-3 )Set fade in and hold time for breath function of ABM-3


// AUTO_BREATH_CTRL10 = 0x0B, // (Auto Breath Control Register 2 of ABM-3 )Set the fade out and off time for breath function of ABM-3


// AUTO_BREATH_CTRL11 = 0x0C, // (Auto Breath Control Register 3 of ABM-3 )Set loop characters of ABM-3


// AUTO_BREATH_CTRL12 = 0x0D, // (Auto Breath Control Register 4 of ABM-3 )Set loop characters of ABM-3



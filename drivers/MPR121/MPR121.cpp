/**
 * @file    MPR121.cpp
 * @brief   Device driver - MPR121 capactiive touch IC
 * @author  sam grove
 * @version 1.0
 * @see     http://cache.freescale.com/files/sensors/doc/data_sheet/MPR121.pdf
 *
 * Copyright (c) 2013
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "MPR121.h"
#include "mbed_debug.h"

/**
 * Clear state variables and initilize the dependant objects
*/
void MPR121::init(void)
{
    // set the i2c speed
    i2c->frequency(400000);
    
    // irq is open-collector and active-low
    irq.mode(PullUp);

    // setup and registers - start with POR values (must be in stop mode)
    MPR121::writeRegister(SRST, 0x63); // REG 0x80

    // Baseline Filtering Control Register (changes response sensitivity)
    MPR121::writeRegister(MHDR, 0x1);  //REG 0x2B
    MPR121::writeRegister(NHDR, 0x1);  //REG 0x2C
    MPR121::writeRegister(NCLR, 0x0);  //REG 0x2D
    MPR121::writeRegister(FDLR, 0x0);  //REG 0x2E
    MPR121::writeRegister(MHDF, 0x1);  //REG 0x2F
    MPR121::writeRegister(NHDF, 0x1);  //REG 0x30
    MPR121::writeRegister(NCLF, 0xFF); //REG 0x31
    MPR121::writeRegister(FDLF, 0x2);  //REG 0x32

    // Touch / Release Threshold
    for (int i = 0; i < (12 * 2); i += 2) // touch
    {
        MPR121::writeRegister(static_cast<MPR121_REGISTER>(E0TTH + i), 0x20); //REG 0x41...0x58 odd
    }
    for (int i = 0; i < (12 * 2); i += 2) // release
    {
        MPR121::writeRegister(static_cast<MPR121_REGISTER>(E0RTH + i), 0x10); //REG 0x41...0x58 even
    }

    // Debounce Register DR=b6...4, DT=b2...0
    MPR121::writeRegister(DT_DR, 0x11); //REG 0x5B

    // Filter and Global CDC CDT Configuration (sample time, charge current)
    MPR121::writeRegister(CDC_CONFIG, 0x10); //REG 0x5C default 10
    MPR121::writeRegister(CDT_CONFIG, 0x20); //REG 0x5D default 24

    // Auto-Configuration Registers
    MPR121::writeRegister(AUTO_CFG0, 0x33); // REG 0x7B
    MPR121::writeRegister(AUTO_CFG1, 0x07); // REG 0x7C
    MPR121::writeRegister(USL, 0xc9);       // REG 0x7D((3.3-.07)/3.3) * 256
    MPR121::writeRegister(LSL, 0x83);       // REG 0x7E((3.3-.07)/3.3) * 256 * 0.65f
    MPR121::writeRegister(TL, 0xb5);        // REG 0x7F((3.3-.07)/3.3) * 256 * 0.9f
    // 255 > USL > TL > LSL > 0

    // Electrode Configuration Register - enable all 12 and start
    MPR121::writeRegister(ECR, 0x8f);

    return;
}

/** 
 * Allow the IC to run and collect user input
*/
void MPR121::enable(void)
{
    _button = 0;
    _button_has_changed = 0;
    // enable the 12 electrodes - allow disable to put device into lower current consumption mode
    writeRegister(ECR, 0x8f);
    irq.fall(callback(this, &MPR121::irq_handler)); // and attach the interrupt handler

    return;
}

/**
 * Stop the IC and put into low power mode 
*/
void MPR121::disable(void)
{
    // detach the interrupt handler
    irq.fall(NULL);
    _button = 0;
    _button_has_changed = 0;
    
    // put the device in low current consumption mode - dont re-init registers
    writeRegister(ECR, 0x0);
    writeRegister(AUTO_CFG0, 0x0); // REG 0x7B
    writeRegister(AUTO_CFG1, 0x0); // REG 0x7C

    return;
}

void MPR121::handleTouch() {
    // Get the currently touched pads
    currTouched = this->getTouched();

    if (currTouched != prevTouched)
    {
        for (uint8_t i = 0; i < 12; i++)
        {
            // it if *is* touched and *wasnt* touched before, alert!
            if (bitRead(currTouched, i) && !bitRead(prevTouched, i))
            {
                if (touchedCallback) touchedCallback(i);
            }

            // if it *was* touched and now *isnt*, alert!
            if (!bitRead(currTouched, i) && bitRead(prevTouched, i))
            {
                if (releasedCallback) releasedCallback(i);
            }
        }

        // reset our state
        prevTouched = currTouched;
    }
}

/** Fetches currently touched pad data from MPR121 then clears class interupt
 * @return 16 bit value containing status of all 12 pads
*/
uint16_t MPR121::getTouched()
{
    uint16_t touched = readRegister(ELE0_7_STAT);
    this->clearInterupt();
    return touched;
}


/** The interrupt handler for the IRQ pin */
void MPR121::irq_handler(void)
{
    interupt = true;
    return;
}

bool MPR121::wasTouched() {
    return interupt;
}

void MPR121::clearInterupt() {
    interupt = false;
    return;
}

void MPR121::attachCallbackTouched(Callback<void(uint8_t pad)> func)
{
    touchedCallback = func;
}

void MPR121::attachCallbackReleased(Callback<void(uint8_t pad)> func)
{
    releasedCallback = func;
}
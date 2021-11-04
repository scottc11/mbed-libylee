#ifndef __DAC8554_H
#define __DAC8554_H

/**
 * Pin 9  --> Select
 * Pin 10 --> SCLK
 * Pin 11 --> MOSI
*/

#include "SPI.h"

#define DAC8554_BUFFER_WRITE  0b00000000
#define DAC8554_SINGLE_WRITE  0b00010000
#define DAC8554_ALL_WRITE     0b00100000
#define DAC8554_BROADCAST     0b00110000

class DAC8554 {

public:
  
  enum Channel {
    CHAN_A = 0x0,
    CHAN_B = 0x1,
    CHAN_C = 0x2,
    CHAN_D = 0x3,
  };
  
  SPI spi;
  int baseline = 485;
  int ceiling = 64741;

  /**
   * NOTE: Texas Instruments requires special SPI formatting, most commonly known as SPI Mode #1
  */
  DAC8554(PinName spiMosi, PinName spiSck, PinName selectPin) : spi(spiMosi, NC, spiSck, 1, selectPin)
  {
    // spi.frequency(25000000); // 25MHz
  }
  
  void init() {
    spi.init();
  };
  
  // 485 and 64741
  void write(DAC8554::Channel chan, uint16_t value, uint8_t mode = DAC8554_SINGLE_WRITE) {
    uint8_t config = mode | (chan << 1);
    this->writeRegister(config, value);
  };

private:
  
  // 8 MSBs are used as control bits and the 16 LSBs are used as data
  // the DAC8554 requires its data with the MSB as the first bit received
  // byte1 --> | A1 | A0 | LD1 | LD0 | X | DAC Select 1 | DAC Select 0 | PD0 |
  // byte2 --> | D15 | D14 | D13 | D12 | D11 | D10 | D9 | D8 |
  // byte3 --> | D7  | D6  | D5  | D4  | D3  | D2  | D1 | D0 |
  void writeRegister(uint8_t config, uint16_t data) {
    
    uint8_t byte1 = config;
    uint8_t byte2 = (data >> 8) & 0xFF;
    uint8_t byte3 = data & 0xFF;
    uint8_t buffer[3] = { byte1, byte2, byte3 };
    spi.write(buffer, 3);
  }
  
  enum Registers {

  };
};

#endif
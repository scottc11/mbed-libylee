#ifndef __BITWISE_METHODS_H
#define __BITWISE_METHODS_H

#include <stdint.h>

int bitSet(int byte, int bit);
int bitClear(int byte, int bit);
int bitClearMany(int byte, int value);
int bitWrite(int byte, int bit, int value);
int bitRead(int byte, int bit);
int bitFlip(int byte, int bit);
uint16_t two8sTo16(int byte1, int byte2);
uint32_t two16sTo32(uint16_t byte1, uint16_t byte2);
void byte32to16(uint16_t *bytes, uint32_t byte32);

uint16_t bitwise_first_16_of_32(uint32_t value);
uint16_t bitwise_last_16_of_32(uint32_t value);

uint16_t convert12to16(int value);

uint32_t bitwise_slice(uint32_t value, uint8_t slice_start, uint8_t slice_length);

#endif
#ifndef __BITWISE_METHODS_H
#define __BITWISE_METHODS_H

#include <stdint.h>

int bitSet(int byte, int bit);
int bitClear(int byte, int bit);
int bitClearMany(int byte, int value);
int bitWrite(int byte, int bit, int value);
int bitRead(int byte, int bit);
int bitFlip(int byte, int bit);
int two8sTo16(int byte1, int byte2);
uint32_t two16sTo32(uint16_t byte1, uint16_t byte2);
void byte32to16(uint16_t *bytes, uint32_t byte32);

#endif
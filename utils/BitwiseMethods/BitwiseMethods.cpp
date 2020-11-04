#include "BitwiseMethods.h"

/**
 * https://www.geeksforgeeks.org/bitwise-operators-in-c-cpp/
 * 
 * The & (bitwise AND) The result of AND is 1 only if both bits are 1.
 * The | (bitwise OR) The result of OR is 1 if any of the two bits is 1.
 * The ^ (bitwise XOR) The result of XOR is 1 if the two bits are different.
 * The << (left shift) in C or C++ takes two numbers, left shifts the bits of the first operand, the second operand decides the number of places to shift.
 * The >> (right shift) in C or C++ takes two numbers, right shifts the bits of the first operand, the second operand decides the number of places to shift.
 * The ~ (bitwise NOT) in C or C++ takes one number and inverts all bits of it 
 * 
 * 
 * settings, clearing, toggling single bits --> https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit
 * 
 * combine two 8 bit values into 16 bit value --> https://stackoverflow.com/questions/11193918/combine-merge-two-bytes-into-one/11193978
**/

int bitSet(int byte, int bit) {
  return (byte |= (1UL << bit));
}

int bitClear(int byte, int bit) {
  return (byte &= ~(1UL << bit));
}

// clears all bits given a byte, 0 means don't clear that bit, 1 means clear that bit
// ex. byte == 0b00001100 will cleat bits 2 and 3
int bitClearMany(int byte, int value) {
  return byte &= ~value;
}

int bitWrite(int byte, int bit, int value) {
  return (value ? bitSet(byte, bit) : bitClear(byte, bit));
}

int bitRead(int byte, int bit) {
  return (byte >> bit) & 0x01;
}

int bitFlip(int byte, int bit) {
  return bitWrite(byte, bit, !bitRead(byte, bit));
}

int two8sTo16(int byte1, int byte2) {
  return (byte2 << 8) | byte1;
}

uint32_t two16sTo32(uint16_t byte1, uint16_t byte2) {
  return (byte2 << 16) | byte1;
}

void byte32to16(uint16_t *bytes, uint32_t byte32) {
  bytes[0] = (byte32 >> 16) & 0xffff;
  bytes[1] = byte32 & 0xffff;
}
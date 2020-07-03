#ifndef __BITWISE_METHODS_H
#define __BITWISE_METHODS_H

int bitSet(int byte, int bit);
int bitClear(int byte, int bit);
int bitClearMany(int byte, int value);
int bitWrite(int byte, int bit, int value);
int bitRead(int byte, int bit);
int bitFlip(int byte, int bit);
int two8sTo16(int byte1, int byte2);

#endif
#ifndef __BITWISE_METHODS_H
#define __BITWISE_METHODS_H

int bitSet(int value, int bit);
int bitClear(int value, int bit);
int bitClearMany(int value, int byte);
int bitWrite(int byte, int bit, int value);
int bitRead(int byte, int bit);

#endif
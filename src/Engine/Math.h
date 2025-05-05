#ifndef MATH_H
#define MATH_H

#include <cstdint>

/// To avoid overflow when adding positive 8-bit integers. Keeps the value between 0 and 127.
int8_t addInt8Positive(int8_t a, int8_t b);

/// To avoid overflow when adding 8-bit unsigned integers. Keeps the value between 0 and 255.
uint8_t addUint8(uint8_t a, short b);

#endif //MATH_H

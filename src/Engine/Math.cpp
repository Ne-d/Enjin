#include "Math.h"


int8_t addInt8Positive(const int8_t a, const int8_t b) {
    if (a + b > 127)
        return 127;

    if (a + b <= 0)
        return 0;

    return static_cast<int8_t>(a + b);
}

uint8_t addUint8(const uint8_t a, const short b) {
    if (a + b > 255)
        return 255;

    if (a + b < 0)
        return 0;

    return static_cast<uint8_t>(a + b);
}


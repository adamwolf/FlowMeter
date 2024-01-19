#include <cinttypes>
#include <cstddef> // gets us NULL

#define INPUT_PULLUP 0xb33f

void pinMode(unsigned char, unsigned int) {} ;

uint32_t millis() {
    return 0;
}
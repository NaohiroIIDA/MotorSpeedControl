#pragma once
#include <stdint.h>

// Counts a quadrature encoder at x4 resolution using interrupts on both edges of A and B
namespace encoder {
void    begin(uint8_t pinA, uint8_t pinB);
int32_t count();
}

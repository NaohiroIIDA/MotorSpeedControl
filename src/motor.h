#pragma once
#include <stdint.h>

// One-direction PWM drive through a low-side FET
namespace motor {
void begin(uint8_t pin);
void setDuty(float duty);  // 0.0 .. 1.0
}

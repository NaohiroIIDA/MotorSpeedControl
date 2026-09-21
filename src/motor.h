#pragma once
#include <stdint.h>

// ローサイド FET による一方向 PWM 駆動
namespace motor {
void begin(uint8_t pin);
void setDuty(float duty);  // 0.0 .. 1.0
}

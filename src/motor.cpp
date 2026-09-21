#include "motor.h"
#include <Arduino.h>
#include "config.h"

namespace motor {
namespace {
uint8_t pin_;
}

void begin(uint8_t pin) {
    pin_ = pin;
    analogWriteFreq(PWM_FREQ_HZ);
    analogWriteRange(PWM_RANGE);
    analogWrite(pin_, 0);
}

void setDuty(float duty) {
    duty = constrain(duty, 0.0f, 1.0f);
    analogWrite(pin_, static_cast<int>(duty * PWM_RANGE + 0.5f));
}
}  // namespace motor

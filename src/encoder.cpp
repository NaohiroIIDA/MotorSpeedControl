#include "encoder.h"
#include <Arduino.h>

namespace encoder {
namespace {
uint8_t          pinA_, pinB_;
volatile int32_t count_ = 0;
volatile uint8_t prevState_ = 0;

// [前状態<<2 | 現状態] -> 増減。不正遷移 (2ステップ飛び) は 0
constexpr int8_t kTransition[16] = {
     0, +1, -1,  0,
    -1,  0,  0, +1,
    +1,  0,  0, -1,
     0, -1, +1,  0,
};

void onEdge() {
    const uint32_t in    = sio_hw->gpio_in;
    const uint8_t  state = (((in >> pinA_) & 1u) << 1) | ((in >> pinB_) & 1u);
    count_ += kTransition[(prevState_ << 2) | state];
    prevState_ = state;
}
}  // namespace

void begin(uint8_t pinA, uint8_t pinB) {
    pinA_ = pinA;
    pinB_ = pinB;
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    prevState_ = (digitalRead(pinA) << 1) | digitalRead(pinB);
    attachInterrupt(digitalPinToInterrupt(pinA), onEdge, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pinB), onEdge, CHANGE);
}

int32_t count() { return count_; }  // 32bit 読み出しはアトミック
}  // namespace encoder

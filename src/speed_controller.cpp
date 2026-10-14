#include "speed_controller.h"

float SpeedController::update(float targetRpm, float measuredRpm, float dt) {
    if (targetRpm <= 0.0f) {
        reset();
        return 0.0f;
    }

    const float error = targetRpm - measuredRpm;
    const float ff    = targetRpm / maxRpm_;  // Approximate duty from the no-load characteristic
    const float unsat = ff + kp_ * error + integral_ + ki_ * error * dt;

    // Anti-windup: don't integrate in the direction that deepens saturation
    const bool saturatedHigh = unsat > 1.0f && error > 0.0f;
    const bool saturatedLow  = unsat < 0.0f && error < 0.0f;
    if (!saturatedHigh && !saturatedLow) {
        integral_ += ki_ * error * dt;
    }

    const float out = ff + kp_ * error + integral_;
    return out < 0.0f ? 0.0f : (out > 1.0f ? 1.0f : out);
}

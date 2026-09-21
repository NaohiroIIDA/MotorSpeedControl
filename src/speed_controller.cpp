#include "speed_controller.h"

float SpeedController::update(float targetRpm, float measuredRpm, float dt) {
    if (targetRpm <= 0.0f) {
        reset();
        return 0.0f;
    }

    const float error = targetRpm - measuredRpm;
    const float ff    = targetRpm / maxRpm_;  // 無負荷特性からの概算 duty
    const float unsat = ff + kp_ * error + integral_ + ki_ * error * dt;

    // アンチワインドアップ: 飽和をさらに深める方向には積分しない
    const bool saturatedHigh = unsat > 1.0f && error > 0.0f;
    const bool saturatedLow  = unsat < 0.0f && error < 0.0f;
    if (!saturatedHigh && !saturatedLow) {
        integral_ += ki_ * error * dt;
    }

    const float out = ff + kp_ * error + integral_;
    return out < 0.0f ? 0.0f : (out > 1.0f ? 1.0f : out);
}

#pragma once

// Feedforward + PI speed control (output is duty 0..1)
class SpeedController {
public:
    SpeedController(float kp, float ki, float maxRpm) : kp_(kp), ki_(ki), maxRpm_(maxRpm) {}

    float update(float targetRpm, float measuredRpm, float dt);
    void  reset() { integral_ = 0.0f; }

    void  setGains(float kp, float ki) { kp_ = kp; ki_ = ki; }
    float getKp() const { return kp_; }
    float getKi() const { return ki_; }

private:
    float kp_, ki_, maxRpm_;
    float integral_ = 0.0f;
};

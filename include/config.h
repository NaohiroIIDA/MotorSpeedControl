#pragma once
#include <stdint.h>

// ---- ピン割り当て (RP2040-Zero) ----
constexpr uint8_t PIN_MOTOR_PWM  = 0;   // FET ゲート (ローサイド N-ch)
constexpr uint8_t PIN_ENC_A      = 2;   // エンコーダ A 相
constexpr uint8_t PIN_ENC_B      = 3;   // エンコーダ B 相

// 回転方向に対してカウントが負になる場合は true (A/B 相の入れ替えと同じ効果)
constexpr bool ENC_REVERSE = true;
constexpr uint8_t PIN_STATUS_LED = 16;  // オンボード WS2812

// ---- PWM ----
constexpr uint32_t PWM_FREQ_HZ = 20000;  // 可聴域外
constexpr uint32_t PWM_RANGE   = 1000;   // 分解能 (duty 0..PWM_RANGE)

// ---- FIT0403 (GB37Y3530-12V-90EN) ----
// エンコーダ: モータ軸 16 パルス/回転, 4逓倍で 64 カウント, 減速比 90:1
constexpr float GEAR_RATIO           = 90.0f;
constexpr float ENC_COUNTS_PER_MOTOR = 16.0f * 4.0f;
constexpr float COUNTS_PER_OUT_REV   = ENC_COUNTS_PER_MOTOR * GEAR_RATIO;  // 5760
constexpr float MAX_RPM              = 122.0f;  // 12V 無負荷時の出力軸回転数

// ---- 制御 ----
constexpr uint32_t CONTROL_PERIOD_US = 10000;  // 100 Hz
constexpr float    SPEED_LPF_ALPHA   = 0.3f;   // 速度推定のローパス (1 でフィルタなし)
constexpr float    DEFAULT_KP        = 0.005f; // [duty / rpm]
constexpr float    DEFAULT_KI        = 0.03f;  // [duty / (rpm*s)]

// ---- テレメトリ ----
constexpr uint32_t TELEMETRY_PERIOD_MS = 100;

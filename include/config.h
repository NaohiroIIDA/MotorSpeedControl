#pragma once
#include <stdint.h>

// ---- Pin assignments (RP2040-Zero) ----
constexpr uint8_t PIN_MOTOR_PWM  = 0;   // FET gate (low-side N-ch)
constexpr uint8_t PIN_ENC_A      = 2;   // Encoder phase A
constexpr uint8_t PIN_ENC_B      = 3;   // Encoder phase B

// Set to true if the count goes negative for the drive direction (same effect as swapping A/B)
constexpr bool ENC_REVERSE = true;
constexpr uint8_t PIN_STATUS_LED = 16;  // On-board WS2812

// ---- PWM ----
constexpr uint32_t PWM_FREQ_HZ = 20000;  // Above the audible range
constexpr uint32_t PWM_RANGE   = 1000;   // Resolution (duty 0..PWM_RANGE)

// ---- FIT0403 (GB37Y3530-12V-90EN) ----
// Encoder: 16 pulses/rev on the motor shaft, 64 counts with x4 decoding, 90:1 gear ratio
constexpr float GEAR_RATIO           = 90.0f;
constexpr float ENC_COUNTS_PER_MOTOR = 16.0f * 4.0f;
constexpr float COUNTS_PER_OUT_REV   = ENC_COUNTS_PER_MOTOR * GEAR_RATIO;  // 5760
constexpr float MAX_RPM              = 122.0f;  // Output shaft speed at 12V, no load

// ---- Control ----
constexpr uint32_t CONTROL_PERIOD_US = 10000;  // 100 Hz
constexpr float    SPEED_LPF_ALPHA   = 0.3f;   // Low-pass for speed estimate (1 = no filtering)
constexpr float    DEFAULT_KP        = 0.005f; // [duty / rpm]
constexpr float    DEFAULT_KI        = 0.03f;  // [duty / (rpm*s)]

// ---- Telemetry ----
constexpr uint32_t TELEMETRY_PERIOD_MS = 100;

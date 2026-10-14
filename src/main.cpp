#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"
#include "encoder.h"
#include "motor.h"
#include "speed_controller.h"

Adafruit_NeoPixel statusLed(1, PIN_STATUS_LED, NEO_GRB + NEO_KHZ800);
SpeedController   controller(DEFAULT_KP, DEFAULT_KI, MAX_RPM);

float targetRpm   = 0.0f;
float measuredRpm = 0.0f;
float duty        = 0.0f;

void printHelp() {
    Serial.println(F("commands:"));
    Serial.println(F("  t <rpm>  target speed [output shaft rpm]"));
    Serial.println(F("  s        stop"));
    Serial.println(F("  kp <v>   proportional gain"));
    Serial.println(F("  ki <v>   integral gain"));
    Serial.println(F("  ?        help / show current gains"));
    Serial.printf("kp=%.5f ki=%.5f\n", controller.getKp(), controller.getKi());
}

void handleCommand(String line) {
    line.trim();
    if (line.length() == 0) return;

    const int   sp  = line.indexOf(' ');
    const String cmd = sp < 0 ? line : line.substring(0, sp);
    const float  val = sp < 0 ? 0.0f : line.substring(sp + 1).toFloat();

    if (cmd == "t") {
        targetRpm = constrain(val, 0.0f, MAX_RPM);
    } else if (cmd == "s") {
        targetRpm = 0.0f;
    } else if (cmd == "kp") {
        controller.setGains(val, controller.getKi());
    } else if (cmd == "ki") {
        controller.setGains(controller.getKp(), val);
    } else {
        printHelp();
    }
}

void pollSerial() {
    static String buf;
    while (Serial.available()) {
        const char c = Serial.read();
        if (c == '\n' || c == '\r') {
            handleCommand(buf);
            buf = "";
        } else {
            buf += c;
        }
    }
}

void controlStep(float dt) {
    static int32_t prevCount = encoder::count();
    const int32_t  count     = encoder::count();
    const int32_t  delta     = (count - prevCount) * (ENC_REVERSE ? -1 : 1);
    prevCount = count;

    const float rawRpm = (delta / COUNTS_PER_OUT_REV) * 60.0f / dt;
    measuredRpm += SPEED_LPF_ALPHA * (rawRpm - measuredRpm);

    duty = controller.update(targetRpm, measuredRpm, dt);
    motor::setDuty(duty);
}

void updateStatusLed() {
    // Stopped: blue / within ±5% of target: green / tracking: yellow
    uint32_t color = statusLed.Color(0, 0, 255);
    if (targetRpm > 0.0f) {
        const bool onTarget = fabsf(targetRpm - measuredRpm) < targetRpm * 0.05f;
        color = onTarget ? statusLed.Color(0, 255, 0) : statusLed.Color(255, 160, 0);
    }
    statusLed.setPixelColor(0, color);
    statusLed.show();
}

void setup() {
    Serial.begin(115200);
    motor::begin(PIN_MOTOR_PWM);
    encoder::begin(PIN_ENC_A, PIN_ENC_B);
    statusLed.begin();
    statusLed.setBrightness(20);
}

void loop() {
    static uint32_t lastControlUs  = micros();
    static uint32_t lastTelemetryMs = millis();

    pollSerial();

    const uint32_t nowUs = micros();
    if (nowUs - lastControlUs >= CONTROL_PERIOD_US) {
        const float dt = (nowUs - lastControlUs) * 1e-6f;
        lastControlUs  = nowUs;
        controlStep(dt);
    }

    const uint32_t nowMs = millis();
    if (nowMs - lastTelemetryMs >= TELEMETRY_PERIOD_MS) {
        lastTelemetryMs = nowMs;
        // key:value format for the serial plotter
        Serial.printf("target:%.1f rpm:%.1f duty:%.1f\n", targetRpm, measuredRpm, duty * 100.0f);
        updateStatusLed();
    }
}

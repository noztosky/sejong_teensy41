// 2025.10.17 nodes - x_beeper
#pragma once
#include <Arduino.h>

class x_beeper {
public:
  void setup(uint8_t pin_ = 33) {
    pin = pin_;
    pinMode(pin, OUTPUT);
    Serial.printf("[x_beeper] setup pin=%d\n", pin);
  }

  // 단일 비프음
  void beep(uint16_t freq = 2000, uint16_t ms = 100) {
    tone(pin, freq);
    delay(ms);
    noTone(pin);
  }

  // 비프음 패턴
  void pattern(uint8_t count = 2, uint16_t freq = 1500, uint16_t ms = 80, uint16_t gap = 60) {
    for (uint8_t i = 0; i < count; i++) {
      tone(pin, freq);
      delay(ms);
      noTone(pin);
      delay(gap);
    }
  }

  // 비동기 루프용 (e.g., 주기적 경고음)
  void loop_async() {
    if (!enabled) return;
    unsigned long now = millis();
    if (now - last_ms >= interval) {
      last_ms = now;
      tone(pin, freq);
      delay(20);
      noTone(pin);
    }
  }

  void enable(uint16_t freq_ = 3000, uint16_t interval_ = 1000) {
    freq = freq_;
    interval = interval_;
    enabled = true;
    last_ms = millis();
  }

  void disable() { enabled = false; }

private:
  uint8_t pin = 33;
  uint16_t freq = 2000;
  uint16_t interval = 1000;
  bool enabled = false;
  unsigned long last_ms = 0;
};

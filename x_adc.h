// 2025.10.18 nodes - x_adc
#pragma once
#include <Arduino.h>

class x_adc {
public:
  void setup(uint8_t pin_ = 40, float vref_ = 3.3f) {
    pin = pin_;
    vref = vref_;
    pinMode(pin, INPUT);
    analogReadResolution(12);      // 0~4095
    
    Serial.printf("[x_adc] setup pin=%d (Vref=%.2fV)\n", pin, vref);
  }

  // 단순 읽기
  uint16_t read_raw() {
    return analogRead(pin);
  }

  // 전압 환산
  float read_voltage() {
    return (float)analogRead(pin) * vref / 4095.0f;
  }

  // 평균 읽기 (노이즈 줄이기용)
  float read_avg(uint8_t n = 8) {
    uint32_t sum = 0;
    for (uint8_t i = 0; i < n; i++) {
      sum += analogRead(pin);
      delayMicroseconds(200);
    }
    return (float)sum / n * vref / 4095.0f;
  }

  void loop_async(uint16_t interval_ms = 500) {
    unsigned long now = millis();
    if (now - last_ms >= interval_ms) {
      last_ms = now;
      float v = read_voltage();
      Serial.printf("[x_adc] %.3f V\n", v);
    }
  }

private:
  uint8_t pin = 40;
  float vref = 3.3f;
  unsigned long last_ms = 0;
};

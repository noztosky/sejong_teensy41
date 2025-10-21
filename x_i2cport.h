// x_i2cport_min_sane.h — 내부 풀업 미사용 + 초슬로 스캔
#pragma once
#include <Arduino.h>
#include <Wire.h>

class x_i2cport {
public:
  void setup() {
    // === Wire (SDA=18, SCL=19) ===
    begin_bus(Wire, 18, 19, "Wire");
    scan_bus(Wire, "Wire");

    // === Wire2 (SDA=25, SCL=24) ===
    begin_bus(Wire2, 25, 24, "Wire2");
    scan_bus(Wire2, "Wire2");
  }

private:
  void begin_bus(TwoWire& w, uint8_t sda, uint8_t scl, const char* name) {
    // GPIO는 모두 입력(풀업 없음)으로 설정 → 외부 풀업만 사용
    pinMode(sda, INPUT);
    pinMode(scl, INPUT);
    delay(2);

    // 버스 리커버리: 오픈드레인 방식 (HIGH를 직접 구동X)
    bus_recover_opendrain(sda, scl);

    // I2C 시작 + 초슬로부터 시도
    w.begin();
    w.setClock(100000); // 100kHz → 필요시 더 내릴 것
    Serial.printf("[%s] begin @100kHz\n", name);
  }

  void bus_recover_opendrain(uint8_t sda, uint8_t scl) {
    // SDA가 Low면 SCL을 토글해 stuck slave 해제
    if (digitalRead(sda) == LOW) {
      // SCL을 출력으로 내렸다 올렸다(LOW/HIZ) → 외부 풀업으로 HIGH
      for (int i = 0; i < 9 && digitalRead(sda) == LOW; ++i) {
        pinMode(scl, OUTPUT);
        digitalWrite(scl, LOW);
        delayMicroseconds(2000);
        pinMode(scl, INPUT); // HIZ → 외부 풀업으로 HIGH
        delayMicroseconds(2000);
      }
      // STOP: SCL=HIZ(High), SDA를 잠깐 LOW→HIZ
      pinMode(scl, INPUT);
      pinMode(sda, OUTPUT);
      digitalWrite(sda, LOW);
      delayMicroseconds(500);
      pinMode(sda, INPUT); // HIZ → 외부 풀업으로 HIGH
      delayMicroseconds(500);
    }
  }

  void scan_bus(TwoWire& w, const char* name) {
    uint8_t found = 0;
    Serial.printf("[x_i2cport] scan %s (STOP probe)\n", name);
    for (uint8_t a = 0x03; a <= 0x77; ++a) {
      w.beginTransmission(a);
      uint8_t e = w.endTransmission(true); // STOP 전송
      if (e == 0) { Serial.printf("  - 0x%02X\n", a); found++; }
      delay(1);
    }
    if (!found) {
      // Repeated-Start 방식으로 한 번 더
      Serial.printf("[x_i2cport] scan %s (RS probe)\n", name);
      for (uint8_t a = 0x03; a <= 0x77; ++a) {
        // reg 0x00 read 시도 (일반적으로 무해)
        if (try_rd_u8(w, a, 0x00) >= 0) {
          Serial.printf("  - 0x%02X (RS)\n", a); found++;
        }
        delay(1);
      }
      if (!found) Serial.printf("  (no device on %s)\n", name);
    }
  }

  int try_rd_u8(TwoWire& w, uint8_t addr, uint8_t reg) {
    w.beginTransmission(addr);
    w.write(reg);
    if (w.endTransmission(false) != 0) return -1;   // repeated start
    if (w.requestFrom((int)addr, 1) != 1) return -1;
    return w.read();
  }

};

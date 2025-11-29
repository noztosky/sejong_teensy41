// 2025.10.17 nodes - Teensy 4.1 + ICM-20948 SPI check

#include <SPI.h>

#define PIN_CS                          38
#define REG_WHO_AM_I                    0x00
#define EXPECTED_ID                     0xEA

class x_icm20948 {
public:
  bool found = false;
  uint8_t who = 0;

  void setup() {
    pinMode(PIN_CS, OUTPUT);
    digitalWrite(PIN_CS, HIGH);

    who = readReg(REG_WHO_AM_I);
    Serial.print("[ICM20948] WHO_AM_I = 0x");
    Serial.println(who, HEX);

    if (who == EXPECTED_ID) {
      found = true;
      Serial.println("[ICM20948] Found and responding ✅");
    } else {
      Serial.println("[ICM20948] Not responding ⚠️");
    }
  }

  void loop() {
    // 센서 초기화 이후 루프 처리 예정
  }

private:
  uint8_t readReg(uint8_t reg) {
    // SPI 읽기: bit7=1 (read), bit6=0 (single)
    uint8_t addr = reg | 0x80;
    digitalWrite(PIN_CS, LOW);
    SPI.transfer(addr);
    uint8_t val = SPI.transfer(0x00);
    digitalWrite(PIN_CS, HIGH);
    return val;
  }
};


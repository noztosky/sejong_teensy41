// 2025.10.17 nodes - Teensy 4.1 + ICM-42688P SPI check

#include <SPI.h>

#define PIN_CS                          37
#define REG_WHO_AM_I                    0x75
#define EXPECTED_ID                     0x47

class x_icm42688p {
public:
  bool found = false;
  uint8_t who = 0;

  void setup() {
    Serial.begin(115200);
    delay(200);

    SPI.begin();
    pinMode(PIN_CS, OUTPUT);
    digitalWrite(PIN_CS, HIGH);

    who = readReg(REG_WHO_AM_I);
    Serial.print("[ICM42688P] WHO_AM_I = 0x");
    Serial.println(who, HEX);

    if (who == EXPECTED_ID) {
      found = true;
      Serial.println("[ICM42688P] Found and responding ✅");
    } else {
      Serial.println("[ICM42688P] Not responding ⚠️");
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

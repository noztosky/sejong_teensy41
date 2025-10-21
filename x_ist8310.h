// 2025.10.17 nodes - Teensy 4.1 + IST8310 I2C check
#define IST8310_ADDR                          0x0C
#define IST8310_WHOAMI                        0x00
#define IST8310_EXPECTED                      0x10

class x_ist8310 {
public:
  bool found = false;
  uint8_t who = 0;

  void setup() {
    who = readReg(IST8310_ADDR, IST8310_WHOAMI);
    Serial.print("[IST8310] WHO_AM_I = 0x");
    Serial.println(who, HEX);

    if (who == IST8310_EXPECTED) {
      found = true;
      Serial.println("[IST8310] Found and responding ✅");
    } else {
      Serial.println("[IST8310] Not responding ⚠️");
    }
  }

  void loop() {
    // 이후 측정 루틴 추가 가능
  }

private:
  uint8_t readReg(uint8_t addr, uint8_t reg) {
    Wire1.beginTransmission(addr);
    Wire1.write(reg);
    if (Wire1.endTransmission(false) != 0) return 0xFF;

    Wire1.requestFrom((int)addr, 1);
    if (Wire1.available() < 1) return 0xFF;
    return Wire1.read();
  }
};

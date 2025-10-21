// 2025.10.17 nodes - Teensy 4.1 + MS5611 detect

class x_ms5611 {
public:
  bool found = false;
  uint8_t i2c_addr = 0x00;
  uint16_t C[7] = {0}; // C1..C6 + CRC word(raw)

  void setup() {
    if (detect(0x76) || detect(0x77)) {
      Serial.print("[MS5611] Found at 0x");
      Serial.print(i2c_addr, HEX);
      Serial.print("  C1..C6: ");
      for (int i = 1; i <= 6; ++i) {
        Serial.print(C[i]); Serial.print(i < 6 ? ", " : "\n");
      }
    } else {
      Serial.println("[MS5611] Not found at 0x76/0x77");
    }
  }

private:
  bool detect(uint8_t addr) {
    // 1) 소프트 리셋 (0x1E)
    if (!writeCmd(addr, 0x1E)) return false;
    delay(3); // 데이터시트 최소 2.8ms

    // 2) PROM 읽기 (A2..AE : C1..C6, A0은 팩토리/버전, AE는 CRC word)
    for (int i = 0; i < 7; ++i) {
      uint8_t cmd = 0xA0 + (i + 1) * 2; // A2,A4,A6,A8,AA,AC,AE
      C[i + 1] = readPROM(addr, cmd);
    }

    // 3) 간이 유효성 체크: 전부 0x0000/0xFFFF가 아니고 C1..C6 중 몇 개라도 non-zero면 존재로 판단
    int nonzero = 0;
    for (int i = 1; i <= 6; ++i) if (C[i] != 0x0000 && C[i] != 0xFFFF) ++nonzero;

    if (nonzero >= 3) { // 느슨한 판정
      found = true;
      i2c_addr = addr;
      return true;
    }
    return false;
  }

  bool writeCmd(uint8_t addr, uint8_t cmd) {
    Wire1.beginTransmission(addr);
    Wire1.write(cmd);
    return Wire1.endTransmission() == 0;
  }

  uint16_t readPROM(uint8_t addr, uint8_t cmd) {
    // write command with repeated start, then read 2 bytes
    Wire1.beginTransmission(addr);
    Wire1.write(cmd);
    if (Wire1.endTransmission(false) != 0) return 0x0000;

    Wire1.requestFrom((int)addr, 2);
    if (Wire1.available() < 2) return 0x0000;
    uint16_t msb = Wire1.read();
    uint16_t lsb = Wire1.read();
    return (msb << 8) | lsb;
  }
};


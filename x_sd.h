// x_sd.h  —  2025.10.17 nodes
#pragma once
#include <Arduino.h>
#include <SdFat.h>

// ===== 설정 =====
// 외장 SPI SD를 쓰고 싶다면 주석 해제하고 CS 핀 지정하세요.
// #define XSD_USE_SPI 1
#ifndef XSD_SPI_CS
#define XSD_SPI_CS 10      // 외장 SPI SD 모듈 사용시 CS 핀
#endif

class x_sd {
public:
  bool   ready = false;
  SdFat  sd;
  FsFile file;            // SdFat의 파일 타입 (SD.h의 File 아님)

  bool setup() {
    Serial.println("[SD] Init...");

#if !defined(XSD_USE_SPI)
    // Teensy 4.1 내장 SD (SDIO)
    // FIFO_SDIO가 가장 속도/안정성 좋음
    if (!sd.begin(SdioConfig(FIFO_SDIO))) {
      Serial.println("[SD] SDIO begin FAIL ❌");
      return false;
    }
#else
    // 외장 SPI SD
    if (!sd.begin(XSD_SPI_CS, SD_SCK_MHZ(25))) {
      Serial.println("[SD] SPI begin FAIL ❌");
      return false;
    }
#endif

    ready = true;
    Serial.println("[SD] OK ✅");
    return true;
  }

  bool open(const char* path) {
    if (!ready) return false;
    // O_WRITE|O_CREAT|O_APPEND = append 모드
    file = sd.open(path, O_WRITE | O_CREAT | O_APPEND);
    if (!file) {
      Serial.print("[SD] Open FAIL: ");
      Serial.println(path);
      return false;
    }
    Serial.print("[SD] Opened: ");
    Serial.println(path);
    return true;
  }

  void println(const String& s) {
    if (file) { file.println(s); file.flush(); }
  }
  void print(const String& s) {
    if (file) { file.print(s); file.flush(); }
  }

  void close() {
    if (file) { file.close(); }
  }

  // 데모: 1초마다 타임스탬프 찍기
  void loop_demo() {
    if (!ready || !file) return;
    static uint32_t t0 = 0;
    if (millis() - t0 >= 1000) {
      t0 = millis();
      file.print("t=");
      file.println(millis());
      file.flush();
    }
  }
};
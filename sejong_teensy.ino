//
// 2025.10.17   nozsoft - xlab
//
// mail:        custompilot1982@gmail.com
// reference:   https://docs.google.com/spreadsheets/d/1F_w5camnlc7cn0YtFFpAKI0pPCTOrmvhNvpB1cN5wfA/edit?gid=0#gid=0
// schematic:   https://pro.easyeda.com/editor#id=a5c96e1751ed435fa6f125d54dbc6e88,tab=13bb2ba02be84a75a610c58930748622@a5c96e1751ed435fa6f125d54dbc6e88|d9fc51fe403a4f2795ce8017a20d3c57@a5c96e1751ed435fa6f125d54dbc6e88|315d43a523ce4fe9989b48b5057603b1@a5c96e1751ed435fa6f125d54dbc6e88|*474c8ef620c74b1f851f36ad0037bf96@a5c96e1751ed435fa6f125d54dbc6e88|db6e5be36b69448e9c5321dd7cf94ad7@a5c96e1751ed435fa6f125d54dbc6e88
// github:      https://github.com/noztosky/sejong_teensy41
//

#include <Arduino.h>
#include <Wire.h>

#include "def.h"

void setup()
{
  Serial.begin(0);
  Wire1.begin();
  Wire1.setClock(400000);       // 400 kHz OK

  // ms5611
  _m.sensor.ms5611.setup();

  // icm42688p
  _m.sensor.icm42688p.setup();

  // icm20948
  _m.sensor.icm20948.setup();

  // ist8310
  _m.sensor.ist8310.setup();

  // serials
  _m.port.serial.setup();

  // i2c port
  _m.port.i2c.setup();

  // voltage adc
  _m.port.adc.setup();

  // sd
  _m.sd.setup();
  _m.sd.open("test.txt");
  _m.sd.println("==== sd test ====");

  // led indicators
  _m.led.setup();

  // peep~
  _m.port.beeper.setup();
}

void loop()
{
  _m.port.serial.loop_async();
  _m.led.loop_async();

  // peep~
  _m.port.beeper.beep();

  // voltage adc
  _m.port.adc.loop_async();     // 0.5초마다 전압 출력

  delay(5000);
}
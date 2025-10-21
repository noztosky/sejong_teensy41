// 2025.10.17   nodes
#if __TEST_PWM

#include "Servo.h"

const uint16_t    pins[]        =   {2, 3, 4, 5, 6, 7, 8, 9, 10, 28, 29};
Servo             servos[11];

void pwm_setup()
{
  sm("test pwms 500 ~ 2700");

  for(uint8_t i = 0; i < 11; i++)
  {
    servos[i].attach(pins[i]);
    servos[i].writeMicroseconds(i * 200 + 500);

    Serial.print(i * 200 + 500);
    Serial.print("\t");
  }
  led_setup();  
}

void pwm_loop()
{
  led_loop();
}

void setup(){
  pwm_setup();
}

void loop(){
  pwm_loop();
}

#endif
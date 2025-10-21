// 2025.10.17   nodes
        
#include "x_serials.h"    
#include "x_ms5611.h"   
#include "x_icm42688p.h"    
#include "x_ist8310.h"    
#include "x_leds.h"   
#include "x_sd.h"
#include "x_i2cport.h"
#include "x_beeper.h"
#include "x_adc.h"
    
struct{   
  struct{   
    x_ms5611                        ms5611;
    x_icm42688p                     icm42688p;
    x_ist8310                       ist8310;
  }sensor;          

  struct{    
    x_serials                       serial;    
    x_i2cport                       i2c;
    x_beeper                        beeper;
    x_adc                           adc;
  }port;

  x_sd                              sd;
  x_leds                            led;
  

}_m;

void bsm(const char* msg)
{
  Serial.begin(1);
  Serial.println(msg);
}

void sm(const char* msg)
{
  Serial.println(msg);
}
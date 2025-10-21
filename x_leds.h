// 2025.10.17   nodes
#define LED_BLUE                  22
#define LED_GREEN                 23
#define LED_ORANGE                32
#define LED_SAFETY                31
    
#define LED_ON                    0
#define LED_OFF                   1
    
const uint16_t leds[] =           {LED_BLUE, LED_GREEN, LED_ORANGE, LED_SAFETY};

class x_leds{
  public:
  void setup()
  {
    _m.current = 0;

    for(int i = 0; i < 4; i ++)
    {
      pinMode(leds[i],            OUTPUT);
      digitalWrite(leds[i],       LED_OFF);
    }
  }     

  void loop_async()
  {
    switch(_m.current++)
    {
      case 0:
        digitalWrite(LED_GREEN,   LED_OFF);
        digitalWrite(LED_ORANGE,  LED_ON); 

        digitalWrite(LED_SAFETY,  LED_ON);
      break;

      case 1:
        digitalWrite(LED_ORANGE,  LED_OFF);
        digitalWrite(LED_BLUE,    LED_ON);
        digitalWrite(LED_SAFETY,  LED_OFF);
      break;

      case 2:
      default:
        digitalWrite(LED_BLUE,    LED_OFF);
        digitalWrite(LED_GREEN,   LED_ON);
        _m.current = 0;
      break;
    }
  }

  protected:
  struct{
    uint8_t current;
  }_m;
};
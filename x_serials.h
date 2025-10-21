// 2025.10.17   nodes
HardwareSerial*   _serial[]       = { &Serial5, &Serial8, &Serial3, &Serial1 };
const char*       _msgs[]         = {"GNSS", "TEL1", "SBUS", "TEL2"};

class x_serials{

public:
  void setup()
  {
    for(uint8_t i = 0; i < 4; i++)
      _serial[i]->begin(115200);
  }

  void loop_async()
  {
    
    for(uint8_t i = 0; i < 4; i++)
    {
      _serial[i]->println(_msgs[i]);
      
      if(_serial[i]->available())
      {
        Serial.print(_msgs[i]);
        Serial.print(": ");
        Serial.println(_serial[i]->readString());
      }
    }
  }
};
#include <Arduino.h>
#ifndef _MYHEADER_H_

class HeaterService
{
private:
  bool isStandby = false;

public:
  void ControlHeaterWithStandBy(int pin, int fanPin, float currentTempC, float minTempC, float maxTempC)
  {
    if (isStandby)
    {
      if (currentTempC < minTempC)
      {
        digitalWrite(pin, HIGH);
        digitalWrite(fanPin, HIGH);
        isStandby = false;
      }
      else
      {
        digitalWrite(pin, LOW);
        digitalWrite(fanPin, LOW);
      }
    }
    else
    {
      if (currentTempC < maxTempC)
      {
        digitalWrite(pin, HIGH);
        digitalWrite(fanPin, HIGH);
      }
      else
      {
        digitalWrite(pin, LOW);
        digitalWrite(fanPin, LOW);
        isStandby = true;
      }
    }
  }
};
#define _MYHEADER_H_
#endif
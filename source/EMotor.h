#include "Arduino.h"

#ifndef LIB_E_MOTOR
#define LIB_E_MOTOR
 
class EMotor
{
private:
  int portEA, portEB;
  byte EPos;
  long count;

  int cSpeed;

  long oldCount;
  unsigned long oldTime;

public:
  EMotor(int portEA, int portEB);

  void setCount(long newCount);
  long getCount();
  void resetCount();
  void update();

  int getSpeed();
};

#endif
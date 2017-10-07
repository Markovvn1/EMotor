#include "Arduino.h"

#ifndef LIB_E_MOTOR
#define LIB_E_MOTOR
 
class EMotor
{
private:
  int portEA, portEB;
  int portInC, portInSh;
  byte EPos;
  long count;

  bool isMotor = false;
  int cSpeed;
  int iSpeed;

  float k = 0.0;
  float dMean = 0;


  unsigned long oldTime;

  void calcSpeed(char count, long time);


public:
  void setSpeedRaw(int speed);
  void setEncoder(int portEA, int portEB);
  void setMotor(int portInC, int portInSh);

  void setCount(long newCount);
  long getCount();
  void resetCount();

  void setSpeed(int newSpeed);
  int getSpeed();

  void update();
};

#endif

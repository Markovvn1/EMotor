#include "EMotor.h"

#define INTER_TIME 100

const char EPosAs[64] = {0, 0, 0, 0, 1, 0, 1, 1,-1,-1, 0,-1, 0,-2, 2, 0, 0,-1,-1,-1, 0, 0, 0, 0, 2, 0, 0,-2, 1, 1, 1, 0, 0, 1, 1, 1,-2, 0, 0, 2, 0, 0, 0, 0,-1,-1,-1, 0, 0, 2,-2, 0,-1, 0,-1,-1, 1, 1, 0, 1, 0, 0, 0, 0};

EMotor::EMotor(int portEA, int portEB)
{
  this->portEA = portEA;
  this->portEB = portEB;
  count = 0;
  cSpeed = 0;

  pinMode(portEA, INPUT);
  pinMode(portEB, INPUT);

  int newEPos = (digitalRead(portEA) << 1) | digitalRead(portEB);
  EPos = (newEPos << 4) | (newEPos << 2) | newEPos;
}

void EMotor::setCount(long newCount)
{
  count = long(newCount * 16 / 9);
}

long EMotor::getCount()
{
  return long(count * 9 / 16);
}

void EMotor::resetCount()
{
  setCount(0);
}

int EMotor::getSpeed()
{
  return cSpeed;
}

void EMotor::update()
{
  unsigned long currentTime = millis();
  if (currentTime - oldTime >= INTER_TIME)
  {
    int newSpeed = (count - oldCount) * 1125 / (int(currentTime) - int(oldTime)) / 2;
    cSpeed = (cSpeed * 2 + newSpeed * 3) / 5;

    oldCount = count;
    oldTime = currentTime;
  }
  
  int newEPos = (digitalRead(portEA) << 1) | digitalRead(portEB);
  
  if (newEPos != (EPos & 3))
  {
    EPos = ((EPos & 15) << 2) | newEPos;       
    count += EPosAs[EPos];
  }
}
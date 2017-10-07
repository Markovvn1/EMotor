#include "EMotor.h"

#define y 0.95
#define pk 0.006
#define dk 0.04

const char EPosAs[64] = {0, 0, 0, 0, 1, 0, 1, 1,-1,-1, 0,-1, 0,-2, 2, 0, 0,-1,-1,-1, 0, 0, 0, 0, 2, 0, 0,-2, 1, 1, 1, 0, 0, 1, 1, 1,-2, 0, 0, 2, 0, 0, 0, 0,-1,-1,-1, 0, 0, 2,-2, 0,-1, 0,-1,-1, 1, 1, 0, 1, 0, 0, 0, 0};

void EMotor::setEncoder(int portEA, int portEB)
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

void EMotor::setMotor(int portInC, int portInSh)
{
  this->portInC = portInC;
  this->portInSh = portInSh;

  isMotor = true;
  iSpeed = 0;

  pinMode(portInC, OUTPUT);
  pinMode(portInSh, OUTPUT);

  setSpeedRaw(0);
}



void EMotor::calcSpeed(char count, long time)
{
  cSpeed = long(count) * 562500 / (time);
}

void EMotor::setSpeedRaw(int speed)
{
  if (speed > 0)
    digitalWrite(portInC, HIGH);
  else
    digitalWrite(portInC, LOW);

  analogWrite(portInSh, speed);
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

void EMotor::setSpeed(int newSpeed)
{
  iSpeed = newSpeed;
}

void EMotor::update()
{
  if (isMotor)
  {
    int pPart = (iSpeed - cSpeed);
    dMean = dMean * y + pPart * (1 - y);
    k = k + pk * pPart + dk * (pPart - dMean);

    int rSpeed = k * iSpeed / 1000.0;

    if (rSpeed > 255)
    {
      k = 255000.0 / iSpeed;
      dMean = 0.0;
      rSpeed = 255;
    }
    setSpeedRaw(rSpeed);
  }

  int newEPos = (digitalRead(portEA) << 1) | digitalRead(portEB);
  
  unsigned long currentTime = micros();
  long time = currentTime - oldTime;
  if (time > 100000) calcSpeed(0, 1);

  if (newEPos != (EPos & 3))
  {
    EPos = ((EPos & 15) << 2) | newEPos;       
    count += EPosAs[EPos];

    Serial.println(int(EPosAs[EPos]));
    calcSpeed(EPosAs[EPos], time);
    oldTime = currentTime;
  }
}

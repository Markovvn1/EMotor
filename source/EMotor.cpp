#include "EMotor.h"

#define y 0.95
#define pk 0.006
#define dk 0.04

const char EPosAs[64] = {0, 0, 0, 0, 1, 0, 1, 1,-1,-1, 0,-1, 0,-2, 2, 0, 0,-1,-1,-1, 0, 0, 0, 0, 2, 0, 0,-2, 1, 1, 1, 0, 0, 1, 1, 1,-2, 0, 0, 2, 0, 0, 0, 0,-1,-1,-1, 0, 0, 2,-2, 0,-1, 0,-1,-1, 1, 1, 0, 1, 0, 0, 0, 0};

void EMotor::setEncoder(int portEA, int portEB)
{
  this->portEA = portEA;
  this->portEB = portEB;

  isEncoder = true;

  pinMode(portEA, INPUT);
  pinMode(portEB, INPUT);

  int newEPos = (digitalRead(portEA) << 1) | digitalRead(portEB);
  EPos = (newEPos << 4) | (newEPos << 2) | newEPos;

  oldTime = micros();
}

void EMotor::setMotor(int portInC, int portInSh)
{
  this->portInC = portInC;
  this->portInSh = portInSh;

  isMotor = true;

  pinMode(portInC, OUTPUT);
  pinMode(portInSh, OUTPUT);

  setRawSpeed(0);
}





void EMotor::setRawSpeedP(int speed)
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
  return aSpeed / 10;
}

void EMotor::setSpeed(int newSpeed)
{
  speedSupport = true;
  iSpeed = newSpeed;
}

void EMotor::setRawSpeed(int newSpeed)
{
  speedSupport = false;
  setRawSpeedP(newSpeed);
}

bool EMotor::update()
{
  bool result = false;

  if (isEncoder)
  {
    int newEPos = (digitalRead(portEA) << 1) | digitalRead(portEB);
    
    unsigned long currentTime = micros();
    long time = currentTime - oldTime;
    if (time > 100000) cSpeed = 0;

    if (newEPos != (EPos & 3))
    {
      EPos = ((EPos & 15) << 2) | newEPos;       
      count += EPosAs[EPos];
      
      cSpeed = long(EPosAs[EPos]) * 562500 / time; 
      oldTime = currentTime;

      result = true;
    }

    aSpeed = (aSpeed * 99 + cSpeed * 10) / 100;


    if (isMotor && speedSupport)
    {
      int pPart = iSpeed - cSpeed;
      dMean = dMean * y + pPart * (1 - y);
      k = k + pk * pPart + dk * (pPart - dMean);

      int rSpeed = k * iSpeed / 1000.0;

      if (rSpeed > 255)
      {
        k = 255000.0 / iSpeed;
        dMean = 0.0;
        rSpeed = 255;
      }

      setRawSpeedP(rSpeed);
    }
    else
    {
      dMean = 0.0;
      k = 0.0;
    }
  }

  return result;
}

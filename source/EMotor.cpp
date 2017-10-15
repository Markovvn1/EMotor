#include "EMotor.h"

// > 100
#define y 0.95
#define pk 0.006
#define dk 0.04

// 40
// #define y 0.5
// #define pk 0.006
// #define dk 0.08

// 10
// #define y 0.0
// #define pk 0.02
// #define dk 0.4

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

  analogWrite(portInSh, speed > 0 ? speed : -speed);
}

void EMotor::setDirection(bool newInvert)
{
  motorType = MotorType::rotate;

  if (invert != newInvert)
  {
    invert = newInvert;
    iSpeed = -iSpeed;
    rSpeed = -rSpeed;
  }
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
  return int(aSpeed / 10);
}

void EMotor::setSpeed(int newSpeed)
{
  supportSpeed = true;

  iSpeed = newSpeed;
  iAbsSpeed = iSpeed > 0 ? iSpeed : -iSpeed;
}

void EMotor::setRawSpeed(int newSpeed)
{
  supportSpeed = false;

  rSpeed = newSpeed > 255 ? 255 : newSpeed < -255 ? -255 : newSpeed;
  // setRawSpeedP(newSpeed);
}

void EMotor::forward()
{
  setDirection(false);
}

void EMotor::backward()
{
  setDirection(true);
}

void EMotor::stop(bool fix)
{
  motorType = fix ? MotorType::stop : MotorType::none;
}

void EMotor::rotateDig(int angle)
{
  motorType = MotorType::rotateDig;
}

bool EMotor::update()
{
  // float ddk = (iSpeed - cSpeed) / 1000.0;
  // dk = dk * y + ddk * (1 - y);
  // k = k + n * ddk + n1 * (ddk - dk);
  // if (k * iSpeed > 255) k = iSpeed / 255.0;
  // setSpeedRaw(k * iSpeed);

  bool result = false;

  if (isEncoder)
  {
    int newEPos = (digitalRead(portEA) << 1) | digitalRead(portEB);
    
    unsigned long currentTime = micros(); // Error! (currentTime < oldTime)
    long time = currentTime - oldTime;
    if (time > 100000) cSpeed = 0;

    if (newEPos != (EPos & 3))
    {
      EPos = ((EPos & 15) << 2) | newEPos;       
      count += EPosAs[EPos];

      // Serial.println(int(EPosAs[EPos]));
      cSpeed = long(EPosAs[EPos]) * 562500 / time; // Error! (time < 0)
      oldTime = currentTime;

      result = true;
    }

    aSpeed = aSpeed * 0.99 + cSpeed * 0.1;


    if (isMotor)
      switch (motorType)
      {
        case MotorType::rotateDig:
          break;



        case MotorType::rotate:
          if (supportSpeed)
          {
            int pPart = iSpeed - cSpeed;
            dMean = dMean * y + pPart * (1 - y);
            k = k + pk * pPart + dk * (pPart - dMean);


            rSpeed = k * iAbsSpeed / 1000.0;

            if (rSpeed > 255)
            {
              k = 255000.0 / iSpeed; // Error
              dMean = pPart;
              rSpeed = k * iAbsSpeed / 1000.0;
            }
          }

          setRawSpeedP(rSpeed);

          break;



        case MotorType::stop:
          setRawSpeedP(0);

          break;



        case MotorType::none:
          setRawSpeedP(0);

          break;
      }
  }

  return result;
}

#include "Arduino.h"

#ifndef LIB_E_MOTOR
#define LIB_E_MOTOR
 
enum MotorType
{
  rotateDig, rotate, stop, none
};

class EMotor
{
private:
  bool isEncoder = false; // Энкодер установлен
  int portEA, portEB; // Порты энкодера
  byte EPos; // Информация о вращении энкодера
  long count = 0; // Текущее положение энкодера (count * 9 / 16 - в градусах)

  bool isMotor = false; // Мотор установлен
  int portInC, portInSh; // Порты мотора
  int cSpeed = 0; // Текущая скорость (мгновенная скорость)
  float aSpeed = 0; // Текущая скорость (средняя)
  int iSpeed = 0; // Скорость, которую нужно поддерживать
  int iAbsSpeed = 0; // Абсолютная скорость, которую нужно поддерживать
  int rSpeed = 0; // Скорость, которая подается на моторы

  float k = 0.0;
  float dMean = 0;


  unsigned long oldTime;


  MotorType motorType = none;
  bool invert = false;
  bool supportSpeed = false;


  void setRawSpeedP(int speed);
  void setDirection(bool newInvert);

public:
  
  void setEncoder(int portEA, int portEB);
  void setMotor(int portInC, int portInSh);

  void setCount(long newCount);
  long getCount();
  void resetCount();

  void setSpeed(int newSpeed);
  void setRawSpeed(int newSpeed);
  int getSpeed();

  void forward();
  void backward();
  void stop(bool fix = true);
  void rotateDig(int angle);

  bool update();
};

#endif

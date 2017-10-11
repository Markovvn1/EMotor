#include "Arduino.h"

#ifndef LIB_E_MOTOR
#define LIB_E_MOTOR
 
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
  long aSpeed = 0; // Текущая скорость (средняя)
  int iSpeed = 0; // Скорость, которую нужно поддерживать

  float k = 0.0;
  float dMean = 0;


  unsigned long oldTime;


  bool speedSupport = false; // Включена поддержка скорости


  void setRawSpeedP(int speed);

public:
  
  void setEncoder(int portEA, int portEB);
  void setMotor(int portInC, int portInSh);

  void setCount(long newCount);
  long getCount();
  void resetCount();

  void setSpeed(int newSpeed);
  void setRawSpeed(int newSpeed);
  int getSpeed();

  bool update();
};

#endif

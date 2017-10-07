#include <EMotor.h> // Подключение библиотеки для работы с енкодерами

EMotor Motor1;

void setup()
{
  Serial.begin(115200);

  Motor1.setEncoder(8, 10); // Установка енкодера
  Motor1.setMotor(12, 9); // Установка мотора 12 - управление направлением, 9 - ШИМ порт
  
  Motor1.setSpeed(60); // Установка скорости (60 градусов/с)
}

void loop()
{
  Motor1.update(); // Вызов обновления

  // Есть запас по времени на
  //   52 умножения 4-битных чисел с плавающей запятой (float)
  //   или ~120 умножений 4-битных целочисленных чисел (long)
}

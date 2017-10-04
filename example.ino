#include <EMotor.h> // Подключение библиотеки для работы с енкодерами

EMotor Motor1(8, 10);

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  Motor1.update(); // Обновление енкодера
  
  Serial.println(Motor1.getSpeed()); // Вывод скорости
  Serial.println(Motor1.getCount()); // Вывод колличества оборотов
}



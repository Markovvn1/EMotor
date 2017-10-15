#include <EMotor.h> // Подключение библиотеки для работы с енкодерами

EMotor Motor1;
EMotor Motor2;

long start_time;

void setup()
{
  Motor1.setEncoder(3, 2); // Установка енкодера
  Motor1.setMotor(12, 10); // Установка мотора (управление направлением, ШИМ порт)
  Motor2.setEncoder(5, 4); // Установка енкодера
  Motor2.setMotor(13, 11); // Установка мотора (управление направлением, ШИМ порт)
  
  Motor1.setSpeed(-200); // Поддерживать скорость в 200 градусов в сек.
  Motor2.setRawSpeed(100); // Не поддерживать скорость (100 - размер ШИМ)
  
  Motor1.forward(); // Вращаться вперед
  Motor2.backward(); // Вращаться назад

  start_time = millis(); // Засикаем время
}

void loop()
{
  // Обновление моторов
  Motor1.update();
  Motor2.update();

  // Если прошло 2 сек
  if (millis() - start_time > 2000)
  {
    Motor1.stop(); // Останавливаем мотор
    Motor2.setSpeed(30); // Меням скорость на 30 градусов в сек (с поддержанием)
  }
}

void loop() {
  rotation.tick();   //Вызываем тикер для работы "SoftServo.h"

#if (WDT_ENABLE == 1)   // Выполняется, если включён WatchDog
  asm("wdr");    // Сброс WatchDog
#endif

  // Ждём нажатия кнопки Вкл. автоповоротов или ручных поворотов с ИК пульта, если получили сигнал, то проверяем какой:
  if (auto_RotatON == true) auto_Rotation();    // Запускаем функцию вкл. автоповоротов Servo
  if (RotatON == true) manual_Turns();          // Иначе работает функция ручных поворотов Servo


  ir_Decoder();   // Проверяем функцию "ir_Decoder()" на наличие приёма команд с ИК пульта
  mode_Timer();   // Проверяем функцию "mode_Timer()" на наличие работающих таймеров
}

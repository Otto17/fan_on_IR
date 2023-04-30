void setup() {
  //Назначаем все РЕЛЕ и LED как ВЫХОД
  for (byte r = 0; r < 4; r++) pinMode(Relay_Array[r], OUTPUT);
  for (byte l = 0; l < 3; l++) pinMode(Led_Array[l], OUTPUT);

  //Выключаем все РЕЛЕ
  for (byte r = 0; r < 4; r++) digitalWrite(Relay_Array[r], !RELAY_TYPE);   // Инвертируем состояние заданное в "RELAY_TYPE"

  //Выключаем все LED
  for (byte l = 0; l < 3; l++) digitalWrite(Led_Array[l], LOW);

  //Получение ИК сигнала через прерывания
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_IR), irIsr, FALLING);

  //Уставливаем в режим таймера (остановится после срабатывания)
  mode1.setTimerMode();   // 6 режимов таймеров для всех 4-х скоростей
  mode2.setTimerMode();
  mode3.setTimerMode();
  mode4.setTimerMode();
  mode5.setTimerMode();
  mode6.setTimerMode();

  blink_LOW.setPeriodMode();    // Установить в режим периода (будет постоянно перезапускаться после срабатывания)

  //Настройки Servo
  //Защита от сильного дребезжания и зависания Servo с большим потреблением тока
  if (servo_val > RIGHT_CORNER || servo_val < LEFT_CORNER) servo_val = (RIGHT_CORNER - LEFT_CORNER) / 2;   // Если считанный из FRAM угол выходит за пределы заданных, то переводим серво в центральное положение от заданных углов
  rotation.attach(SERVOMOTOR);  // Вкл. Servo
  rotation.delayMode();         // переключить в режим задержки (по умолчанию)

  rotation.write(servo_val);     // Записываем положение Servo из FRAM после включения питания
  fram.WriteInt(PAGE_FRAM, SECTOR_FRAM, servo_val);   //Записываем в сектор "SECTOR_FRAM", "PAGE_FRAM" страницы значение "servo_val"

#if (WDT_ENABLE == 1)   // Выполняется, если включён WatchDog
  WTD_on();   // Вкл. WatchDog
#endif
}

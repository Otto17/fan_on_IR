void loop() {
  rotation.tick();  // Вызываем тикер для работы "SoftServo.h"

#if (WDT_ENABLE == 1)  // Выполняется, если включён WatchDog
  asm("wdr");          // Сбрасываем WatchDog
#endif

  ir_Decoder();  // Проверяем функцию "ir_Decoder()" на наличие приёма команд с ИК пульта
  mode_Timer();  // Проверяем функцию "mode_Timer()" на наличие работающих таймеров


  //Вкл. нужный LED из массива пинов (в массиве счёт начинается с нуля, поэтому дописываем -1 (для первых трёх режимов) от pin_Led для удобства восприятия, для четвёртого режима отнимаем другие цифры)
  if (flagLED == true) {                               // Если флаг Вкл./Выкл. LED поднят
    switch (regim_LED) {                               // Проверяем какой из режимов LED поступил из функции "WriteRelLed()"
      case 1:                                          // GREEN
        digitalWrite(Led_Array[regim_LED - 1], HIGH);  // Вкл. зелёный LED (получаем от "WriteRelLed()" цифру 1 и отнимаем 1, получаем 0, в массиве под номером ноль - LED_GREAN)
        break;

      case 2:                                          // BLUE
        digitalWrite(Led_Array[regim_LED - 1], HIGH);  // Вкл. синий LED (получаем от "WriteRelLed()" цифру 2 и отнимаем 1, получаем 1, в массиве под номером один - LED_BLUE)
        break;

      case 3:                                          // RED
        digitalWrite(Led_Array[regim_LED - 1], HIGH);  // Вкл. красный LED (получаем от "WriteRelLed()" цифру 3 и отнимаем 1, получаем 2, в массиве под номером два - LED_RED)
        break;

      case 4:                                          // GREEN + RED
        digitalWrite(Led_Array[regim_LED - 4], HIGH);  // Вкл. зелёный LED (получаем от "WriteRelLed()" цифру 4 и отнимаем 4, получаем 0, в массиве под номером ноль - LED_GREAN)
        digitalWrite(Led_Array[regim_LED - 2], HIGH);  // Вкл. красный LED (получаем от "WriteRelLed()" цифру 4 и отнимаем 2, получаем 2, в массиве под номером два - LED_RED)
        break;
    }
  } else {  // Иначе
    //Выключаем все LED. Используем управление конкретными пинами на порту "B" через регистры портов для уменьшения размера кода (что бы уместить код в ATmega48)
    PORTB &= ~(1 << 2);  // Записываем в порт PB2 (10 в Arduino IDE) логический ноль (Выкл.). Аналогично "digitalWrite(LED_GREAN, LOW);"
    PORTB &= ~(1 << 3);  // Записываем в порт PB3 (11 в Arduino IDE) логический ноль (Выкл.). Аналогично "digitalWrite(LED_BLUE, LOW);"
    PORTB &= ~(1 << 4);  // Записываем в порт PB4 (12 в Arduino IDE) логический ноль (Выкл.). Аналогично "digitalWrite(LED_RED, LOW);"
  }


  //Ждём нажатия кнопки Вкл. автоповоротов или ручных поворотов с ИК пульта, если получили сигнал, то проверяем какой:
  if (auto_RotatON == true) auto_Rotation();  // Если флаг автоповоротов поднят, то запускаем функцию Вкл. автоповоротов Servo
  if (RotatON == true) manual_Turns();        // Если флаг ручных поворотов поднят, то запускаем функцию Вкл. ручных поворотов Servo


  //Записываем положения Servo в EEPROM при поднятом флаге
  if (flagWR == true) {       // Если Флаг для записи поднят, то
    EEPROM_Write(servo_val);  // Записываем наши данные (положение Servo)
    flagWR = false;           // Опускаем флаг
  }


  //Отложенная запись в EEPROM (защита от частой перезаписи при долгом удержании кнопок: < Влево или Вправо >)
  if (ee_flag == true) {                       // Если флаг для отложеной записи в EEPROM поднят, тогда
    if (millis() - ee_tmr >= DELAY_EE_TIME) {  // Проверяем таймер, если время вышло, то...
      ee_tmr = millis();                       // Сохраняем новое время в "ee_tmr"
      flagRW_UP();                             // Разрешаем запись в EEPROM
      ee_flag = false;                         // Опускаем флаг для отложеной записи в EEPROM
    }
  }
}

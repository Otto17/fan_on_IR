// Функция для обработки прерывания
void irIsr() {
  ir.tick();    // Вызывать при ОТРИЦАТЕЛЬНОМ (FALLING) фронте на пине ИК приемника в прерывании

}


#if (WDT_ENABLE == 1)   // Выполняется, если включён WatchDog
//Функция Вкл. WatchDog
void WTD_on(void) {
  asm("wdr");    // Сброс WatchDog
  WDTCR |= (1 << WDCE) | (1 << WDE);    // Разрешить изменение WatchDog
  WDTCR = (1 << WDE) | (1 << WDP2) | (1 << WDP1) | (1 << WDP0);   // Вкл. WatchDog примерно на 2.2 сек (максимальное время для Atmega8)
}

/*
  //Функция Выкл. WatchDog
  void WTD_off(void) {
  asm("wdr");    // Сброс WatchDog
  WDTCR |= (1 << WDCE) | (1 << WDE);    // Разрешить изменение WatchDog
  WDTCR = 0x00;   // Выкл. WatchDog
  }
*/
#endif

//Если вызвать функцию "WriteRelLed(pin_Rel, pin_Led)"с аргументами, то переключимся на выбранное РЕЛЕ и LED
//Функция упрощения Вкл./Выкл. РЕЛЕ и LED и уменьшения кода при многократных обращениях к пинам
void WriteRelLed(byte pin_Rel, byte pin_Led) {    // номер пина РЕЛЕ - "pin_Rel", номер пина LED - "pin_Led", которые будут Вкл.
  //Выключаем все РЕЛЕ
  digitalWrite(RELAY1, !RELAY_TYPE);    // Инвертируем состояние заданное в "RELAY_TYPE"
  digitalWrite(RELAY2, !RELAY_TYPE);
  digitalWrite(RELAY3, !RELAY_TYPE);
  digitalWrite(RELAY4, !RELAY_TYPE);

  //Останавливаем все таймеры
  mode1.stop();
  mode2.stop();
  mode3.stop();
  mode4.stop();
  mode5.stop();
  mode6.stop();
  blink_LOW.stop();   // Останавливаем таймер для мигающего LED в режиме LOW
  blink_ON = false;   // Опускаем флаг для мигающего LED в режиме LOW

  if (repeat[0] == 1) {     // Если флаг "repeat[0]" поднят, то включаем задержку
    delay(DELAY_REL_OFF);   // Ждём, пока контакты реле разъединятся после отключения
    repeat[0] = 0;          // Опускаем флаг
  }

  //Вкл. нужное РЕЛЕ из массива (в массиве счёт начинается с нуля, поэтому дописываем -1 от pin_Rel)
  digitalWrite(Relay_Array[pin_Rel - 1], RELAY_TYPE); // Вкл. нужное реле при состоянии "RELAY_TYPE"

  //Выключаем все LED
  digitalWrite(LED_GREAN, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_RED, LOW);
  //Вкл. нужный LED из массива (в массиве счёт начинается с нуля, поэтому дописываем -1 от pin_Led)
  digitalWrite(Led_Array[pin_Led - 1], HIGH);   // Вкл. нужный LED
}



//Если вызвать функцию "WriteRelLed()" без аргументов, то выключатся все РЕЛЕ и LED
void WriteRelLed() {    // Выключаем ВСЁ
  //Выключаем все РЕЛЕ
  digitalWrite(RELAY1, !RELAY_TYPE);    // Инвертируем состояние заданное в "RELAY_TYPE"
  digitalWrite(RELAY2, !RELAY_TYPE);
  digitalWrite(RELAY3, !RELAY_TYPE);
  digitalWrite(RELAY4, !RELAY_TYPE);

  //Выключаем все LED
  digitalWrite(LED_GREAN, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_RED, LOW);

  //Останавливаем все таймеры
  mode1.stop();
  mode2.stop();
  mode3.stop();
  mode4.stop();
  mode5.stop();
  mode6.stop();
  blink_LOW.stop();   // Останавливаем таймер для мигающего LED в режиме LOW
  blink_ON = false;   // Опускаем флаг для мигающего LED в режиме LOW

  //Блок защиты от быстрого откл./вкл. (перещёлкивания) реле при повторном приёме ИК сигнала и в пределах одного режима
  for (byte i = 0; i < sizeof(repeat); i++) {   // Вычисляем размер массива
    repeat[i] = 0;    // Присваиваем всему массиву ноль (false), тем самым делаем доступный любой режим для включения
  }
}

//Функция с поднятием одного заданного флага и опусканием всех остальных флагов (для защиты и одновременного включения двух релюшек)
void flag_Array(byte flag_Read) {
  for (byte f = 0; f < sizeof(repeat); f++) {   // Вычисляем размер массива и повторяем столько же раз, какой и размер массива
    for (byte i = 0; i < flag_Read; i++) {      // Подставляем число полученное в функцию
      repeat[f] = 0;                            // Опускаем все флаги
      repeat[flag_Read] = 1;                    // И поднимаем нужный (заданный) флаг
    }
  }
}

// Функция для обработки прерывания
void irIsr() {
  ir.tick();  // Вызывать при ОТРИЦАТЕЛЬНОМ (FALLING) фронте на пине ИК приемника в прерывании
}


//WatchDog
#if (WDT_ENABLE == 1)  // Выполняется, если включён WatchDog
void WTD_on(void) {    // Функция Вкл. WatchDog
  asm("wdr");          // Сбрасываем WatchDog

#if (WTD_CPU == 1)
  // Выполняется, если выбран камень ATmega8
  WDTCR |= (1 << WDCE) | (1 << WDE);                             // Разрешить изменение WatchDog
  WDTCR = (1 << WDE) | (1 << WDP2) | (1 << WDP1) | (1 << WDP0);  // Вкл. WatchDog примерно на 2.2 сек (максимальное время для ATmega8)
#else
  // Иначе выполняем код ниже, если выбран камень ATmega48
  WDTCSR |= (1 << WDCE) | (1 << WDE);                             // Разрешить изменение WatchDog
  WDTCSR = (1 << WDE) | (1 << WDP2) | (1 << WDP1) | (1 << WDP0);  // Вкл. WatchDog на 2 сек для ATmega48
#endif
}


//Функция Выкл. WatchDog (оставил тут закоментированный на НЗ)
// void WTD_off(void) {
//   asm("wdr");  // Сбрасываем WatchDog
// #if (WTD_CPU == 1)
//   // Выполняется, если выбран камень ATmega8
//   WDTCR |= (1 << WDCE) | (1 << WDE);  // Разрешить изменение WatchDog
//   WDTCR = 0x00;                       // Выкл. WatchDog
// #else
//   // Иначе выполняем код ниже, если выбран камень ATmega48
//   WDTCSR |= (1 << WDCE) | (1 << WDE);  // Разрешить изменение WatchDog
//   WDTCSR = 0x00;                       // Выкл. WatchDog
// #endif
// }

#endif



//Если вызвать функцию "WriteRelLed(pin_Rel, pin_Led)"с аргументами, то переключимся на выбранное РЕЛЕ и LED
//Функция упрощения Вкл./Выкл. РЕЛЕ и LED и уменьшения кода при многократных обращениях к пинам
void WriteRelLed(byte pin_Rel, byte pin_Led) {  // номер пина РЕЛЕ - "pin_Rel", номер пина LED - "pin_Led", которые будут Вкл.
  //Выключаем все РЕЛЕ
  digitalWrite(RELAY1, !RELAY_TYPE);  // Инвертируем состояние заданное в "RELAY_TYPE"
  digitalWrite(RELAY2, !RELAY_TYPE);
  digitalWrite(RELAY3, !RELAY_TYPE);
  digitalWrite(RELAY4, !RELAY_TYPE);

  //Выключаем все LED. Используем управление конкретными пинами на порту "B" через регистры портов для уменьшения размера кода (что бы уместить код в ATmega48)
  PORTB &= ~(1 << 2);  // Записываем в порт PB2 (10 в Arduino IDE) логический ноль (Выкл.). Аналогично "digitalWrite(LED_GREAN, LOW);"
  PORTB &= ~(1 << 3);  // Записываем в порт PB3 (11 в Arduino IDE) логический ноль (Выкл.). Аналогично "digitalWrite(LED_BLUE, LOW);"
  PORTB &= ~(1 << 4);  // Записываем в порт PB4 (12 в Arduino IDE) логический ноль (Выкл.). Аналогично "digitalWrite(LED_RED, LOW);"


  //Останавливаем оба таймера
  mode1.stop();
  mode2.stop();

  if (repeat[0] == 1) {    // Если флаг "repeat[0]" поднят, то включаем задержку
    delay(DELAY_REL_OFF);  // Ждём, пока контакты реле разъединятся после отключения
    repeat[0] = 0;         // Опускаем флаг
  }

  //Вкл. нужное РЕЛЕ из массива (в массиве счёт начинается с нуля, поэтому дописываем -1 от pin_Rel)
  digitalWrite(Relay_Array[pin_Rel - 1], RELAY_TYPE);  // Вкл. нужное реле при состоянии "RELAY_TYPE"

  regim_LED = pin_Led;  // Копируем значение пина для LED из "pin_Led" в глобальную "regim_LED"
}



//Если вызвать функцию "WriteRelLed()" без аргументов, то выключатся все РЕЛЕ и LED
void WriteRelLed() {  // Выключаем ВСЁ
  //Выключаем все РЕЛЕ
  digitalWrite(RELAY1, !RELAY_TYPE);  // Инвертируем состояние заданное в "RELAY_TYPE"
  digitalWrite(RELAY2, !RELAY_TYPE);
  digitalWrite(RELAY3, !RELAY_TYPE);
  digitalWrite(RELAY4, !RELAY_TYPE);

  //Выключаем все LED. Используем управление конкретными пинами на порту "B" через регистры портов для уменьшения размера кода (что бы уместить код в ATmega48)
  PORTB &= ~(1 << 2);  // Записываем в порт PB2 (10 в Arduino IDE) логический ноль (Выкл.). Аналогично "digitalWrite(LED_GREAN, LOW);"
  PORTB &= ~(1 << 3);  // Записываем в порт PB3 (11 в Arduino IDE) логический ноль (Выкл.). Аналогично "digitalWrite(LED_BLUE, LOW);"
  PORTB &= ~(1 << 4);  // Записываем в порт PB4 (12 в Arduino IDE) логический ноль (Выкл.). Аналогично "digitalWrite(LED_RED, LOW);"


  //Останавливаем оба таймера
  mode1.stop();
  mode2.stop();


  //Блок защиты от быстрого откл./вкл. (перещёлкивания) реле при повторном приёме ИК сигнала и в пределах одного режима
  for (byte i = 0; i < sizeof(repeat); i++) {  // Вычисляем размер массива
    repeat[i] = 0;                             // Присваиваем всему массиву ноль (false), тем самым делаем доступный любой режим для включения
  }
}



//Функция с поднятием одного заданного флага и опусканием всех остальных флагов (для защиты от одновременного включения двух релюшек)
void flag_Array(byte flag_Read) {
  for (byte f = 0; f < sizeof(repeat); f++) {  // Вычисляем размер массива и повторяем столько же раз, какой и размер массива
    for (byte i = 0; i < flag_Read; i++) {     // Подставляем число полученное в функцию
      repeat[f] = 0;                           // Опускаем все флаги
      repeat[flag_Read] = 1;                   // И поднимаем нужный (заданный) флаг
    }
  }
}



//Функция для чтения из EEPROM маркера и данных
void EEPROM_Read() {
  bool flagRD = true;                                      // Если "false", то выполняем чтение из EEPROM
  for (int i = 0; i < EEPROM.length(); i++) {              // Вычисляем объём EEPROM
    if (EEPROM.read(i) == EEPROM_MARKER) {                 // Сравниваем ячейку с номером маркера, если нашли, то...
      addr = (i - 1 + EEPROM.length()) % EEPROM.length();  // Производим рассчёт для ячейки адреса данных
      flagRD = false;                                      // Опускаем флаг
      break;                                               // Принудительно выходим из цикла for
    }
  }
  if (flagRD == true) {  // Срабатывает только 1 раз, когда EEPROM полностью чиста
    eePos = 0;           // Если не нашли маркер, то обнуляем переменную с данными
  } else {
    eePos = EEPROM.read(addr);  // Иначе, если маркер совпал, то считываем значение в переменную с данными
  }
}


//Функция для записи данных в EEPROM
void EEPROM_Write(byte dataWrite) {
  static int maddr;                      // Адрес для записи маркера
  eePos = dataWrite;                     // Передаём наше число для записи в переменную "eePos"
  maddr = (addr + 1) % EEPROM.length();  // Присваемваем "maddr" данные следующей ячейке после считанной из EEPROM
  EEPROM.write(maddr, EEPROM_MARKER);    // Записываем в EEPROM значение Маркера
  EEPROM.update(addr, eePos);            // Записываем наши данные в EEPROM (если они отличаются отдействующих)
  addr++;                                // Прибалвяем +1 к "addr"
  if (addr >= EEPROM.length()) {         // Если "addr" >= размеру EEPROM
    addr = 0;                            // Переходим обратно в первую ячейку и по кругу...
  }
}


//Функция для поднятия флага и перехода к другой ячейки. Разрешаем запись в EEPROM при вызове данной функции
void flagRW_UP() {
  flagWR = true;      // Поднимаем флаг для записи
  eePos = eePos + 1;  // Прибавляем +1
}
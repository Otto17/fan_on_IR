//Функция приёма данных с ИК пульта
void ir_Decoder() {
  if (ir.available()) {          // Если пакет успешно принят, то...
    switch (ir.readCommand()) {  // Читаем команду
      case IR_OFF:
        //Кнопка OFF: Выключить ВСЁ
        WriteRelLed();         // Выключаем всё
        RotatON = false;       // Опускаем флаг для ручных поворотов
        auto_RotatON = false;  // Опускаем флаг для Автоповоротов
        regim_LED = 0;         // Меняем режим LED на нулевой (все LED Выкл.)
        flagRW_UP();           // Разрешаем запись в EEPROM
        break;

      case IR_OK:
        // Кнопка OK: Выключить LED (что бы ночью не мешали)
        flagLED = !flagLED;  // Инвертируем флаг
        break;

      case IR_UP:
        //Кнопка вверх: ↑ Авто - Вращения Вкл.
        RotatON = false;      // Опускаем флаг для ручных поворотов
        auto_RotatON = true;  // Поднимаем флаг для Вкл. Автоповоротов
        break;

      case IR_DOWN:
        //Кнопка вниз:  ↓ Авто - Вращения Выкл.
        RotatON = false;       // Опускаем флаг для ручных поворотов
        auto_RotatON = false;  // Опускаем флаг для Автоповоротов
        flagRW_UP();           // Разрешаем запись в EEPROM
        //delay(2100); //ДЛЯ ТЕСТА WatchDog
        break;

      case IR_LEFT:
        //Кнопка <: « Влево
        auto_RotatON = false;  // Опускаем флаг для Автоповоротов
        RotatON = true;        // Поднимаем флаг для ручных поворотов
        Left_Turns = true;     // Поднимаем флаг при нажатии и удержании кнопки "« Влево"
        ee_flag = true;        // Поднимаем флаг для отложеной записи в EEPROM (защита от частой перезаписи при долгом удержании кнопки < Влево)
        break;

      case IR_RIGHT:
        //Кнопка >: Вправо »
        auto_RotatON = false;  // Опускаем флаг для Автоповоротов
        RotatON = true;        // Поднимаем флаг для ручных поворотов
        Right_Turns = true;    // Поднимаем флаг при нажатии и удержании кнопки "Вправо »"
        ee_flag = true;        // Поднимаем флаг для отложеной записи в EEPROM (защита от частой перезаписи при долгом удержании кнопки Вправо >)
        break;


        //1 Режим
      case IR_1:
        //Кнопка 1: 1 режим (Timer1)
        if (repeat[1] == 0) repeat[0] = 1;  // Если у элемента массива "repeat[1]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(1, 1);                  // Вкл. "RELAY1" и "LED_GREAN"
        mode1.start();                      // Запускаем таймер 1 для режима 1 (для всех 4-х скоростей)
        flag_Array(1);                      // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_4:
        //Кнопка 4: 1 режим (Timer2)
        if (repeat[1] == 0) repeat[0] = 1;  // Если у элемента массива "repeat[1]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(1, 1);                  // Вкл. "RELAY1" и "LED_GREAN"
        mode2.start();                      // Запускаем таймер 2 для режима 1 (для всех 4-х скоростей)
        flag_Array(1);                      // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;


        //2 Режим
      case IR_2:
        //Кнопка 2: 2 режим (Timer1)
        if (repeat[2] == 0) repeat[0] = 1;  // Если у элемента массива "repeat[2]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(2, 2);                  // Вкл. "RELAY2" и "LED_BLUE"
        mode1.start();                      // Запускаем таймер 1 для режима 2 (для всех 4-х скоростей)
        flag_Array(2);                      // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_5:
        //Кнопка 5: 2 режим (Timer2)
        if (repeat[2] == 0) repeat[0] = 1;  // Если у элемента массива "repeat[2]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(2, 2);                  // Вкл. "RELAY2" и "LED_BLUE"
        mode2.start();                      // Запускаем таймер 2 для режима 2 (для всех 4-х скоростей)
        flag_Array(2);                      // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;


        //3 Режим
      case IR_3:
        //Кнопка 3: 3 режим (Timer1)
        if (repeat[3] == 0) repeat[0] = 1;  // Если у элемента массива "repeat[3]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(3, 3);                  // Вкл. "RELAY3" и "LED_RED" без таймера
        mode1.start();                      // Запускаем таймер 1 для режима 3 (для всех 4-х скоростей)
        flag_Array(3);                      // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_6:
        //Кнопк  6: 3 режим (Timer2)
        if (repeat[3] == 0) repeat[0] = 1;  // Если у элемента массива "repeat[3]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(3, 3);                  // Вкл. "RELAY3" и "LED_RED"
        mode2.start();                      // Запускаем таймер 2 для режима 3 (для всех 4-х скоростей)
        flag_Array(3);                      // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;


        //4 Режим
      case IR_STEP:
        // Кнопка STEP: 4 режим (Timer1)
        if (repeat[4] == 0) repeat[0] = 1;  // Если у элемента массива "repeat[4]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(4, 4);                  // Вкл. "RELAY4" и "LED_GREAN + LED_RED"
        regim_LED = 4;                      // Меняем режим LED на четвёрный (зелёный + красный LED)
        mode1.start();                      // Запускаем таймер 1 для режима 4 (для всех 4-х скоростей)
        flag_Array(4);                      // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_STOP:
        //Кнопка STOP: 4 режим (Timer2)
        if (repeat[4] == 0) repeat[0] = 1;  // Если у элемента массива "repeat[4]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(4, 4);                  // Вкл. "RELAY4" и "LED_GREAN + LED_RED"
        regim_LED = 4;                      // Меняем режим LED на четвёрный (зелёный + красный LED)
        mode2.start();                      // Запускаем таймер 2 для режима 4 (для всех 4-х скоростей)
        flag_Array(4);                      // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;
    }
  }
}

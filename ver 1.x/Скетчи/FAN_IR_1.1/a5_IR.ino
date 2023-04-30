//Функция приёма данных с ИК пульта
void ir_Decoder() {
  if (ir.available()) {   // Если пакет успешно принят, то...
    switch (ir.readCommand()) {   // Читаем команду
      case IR_OK:
        //Кнопка OK: Off (Выключить ВСЁ)
        WriteRelLed();   // Выключаем всё
        RotatON = false;        // Опускаем флаг для ручных поворотов
        auto_RotatON = false;   // Опускаем флаг для Автоповоротов
        fram.WriteInt(PAGE_FRAM, SECTOR_FRAM, servo_val);   //Записываем в сектор "SECTOR_FRAM", "PAGE_FRAM" страницы значение "servo_val"
        break;

      case IR_UP:
        //Кнопка вверх: ↑ Авто - Вращения Вкл.
        RotatON = false;        // Опускаем флаг для ручных поворотов
        auto_RotatON = true;    // Поднимаем флаг для Вкл. Автоповоротов
        break;

      case IR_DOWN:
        //Кнопка вниз:  ↓ Авто - Вращения Выкл.
        RotatON = false;        // Опускаем флаг для ручных поворотов
        auto_RotatON = false;   // Опускаем флаг для Автоповоротов
        fram.WriteInt(PAGE_FRAM, SECTOR_FRAM, servo_val);   //Записываем в сектор "SECTOR_FRAM", "PAGE_FRAM" страницы значение "servo_val"
        break;

      case IR_LEFT:
        //Кнопка <: « Влево
        auto_RotatON = false;   // Опускаем флаг для Автоповоротов
        RotatON = true;         // Поднимаем флаг для ручных поворотов
        Left_Turns = true;      // Поднимаем флаг при нажатии и удержании кнопки "« Влево"
        fram.WriteInt(PAGE_FRAM, SECTOR_FRAM, servo_val);   //Записываем в сектор "SECTOR_FRAM", "PAGE_FRAM" страницы значение "servo_val"
        break;

      case IR_RIGHT:
        //Кнопка >: Вправо »
        auto_RotatON = false;  // Опускаем флаг для Автоповоротов
        RotatON = true;        // Поднимаем флаг для ручных поворотов
        Right_Turns  = true;   // Поднимаем флаг при нажатии и удержании кнопки "Вправо »"
        fram.WriteInt(PAGE_FRAM, SECTOR_FRAM, servo_val);   //Записываем в сектор "SECTOR_FRAM", "PAGE_FRAM" страницы значение "servo_val"
        break;

      case IR_1:
        //Кнопка 1: 1 режим (On) (без таймера)
        if (repeat[1] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[1]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(1, 1);    // Вкл. "RELAY1" и "LED_GREAN" без таймера
        flag_Array(1);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_2:
        //Кнопка 2: 2 режим (On) (без таймера)
        if (repeat[2] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[2]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(2, 2);    // Вкл. "RELAY2" и "LED_BLUE" без таймера
        flag_Array(2);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_3:
        //Кнопка 3: 3 режим (On) (без таймера)
        if (repeat[3] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[3]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(3, 3);    // Вкл. "RELAY3" и "LED_RED" без таймера
        flag_Array(3);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_4:
        //Кнопка 4: 1 режим (Timer1)
        if (repeat[1] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[1]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(1, 1);    // Вкл. "RELAY1" и "LED_GREAN"
        mode1.start();        // Запускаем таймер для режима 1 (для всех 4-х скоростей)
        flag_Array(1);        // Поднимаем заданный флаг в скобках, а все остальные опускаем

        break;

      case IR_5:
        //Кнопка 5: 2 режим (Timer1)
        if (repeat[2] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[2]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(2, 2);    // Вкл. "RELAY2" и "LED_BLUE"
        mode1.start();        // Запускаем таймер для режима 1 (для всех 4-х скоростей)
        flag_Array(2);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_6:
        //Кнопк  6: 3 режим (Timer1)
        if (repeat[3] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[3]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(3, 3);    // Вкл. "RELAY3" и "LED_RED"
        mode1.start();        // Запускаем таймер для режима 1 (для всех 4-х скоростей)
        flag_Array(3);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_7:
        //Кнопка 7: 1 режим (Timer2)
        if (repeat[1] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[1]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(1, 1);    // Вкл. "RELAY1" и "LED_GREAN"
        mode2.start();        // Запускаем таймер для режима 2 (для всех 4-х скоростей)
        flag_Array(1);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_8:
        //Кнопка 8: 2 режим (Timer2)
        if (repeat[2] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[2]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(2, 2);    // Вкл. "RELAY2" и "LED_BLUE"
        mode2.start();        // Запускаем таймер для режима 2 (для всех 4-х скоростей)
        flag_Array(2);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_9:
        //Кнопка 9: 3 режим (Timer2)
        if (repeat[3] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[3]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(3, 3);    // Вкл. "RELAY3" и "LED_RED"
        mode2.start();        // Запускаем таймер для режима 2 (для всех 4-х скоростей)
        flag_Array(3);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_10:
        //Кнопка +10: 1 режим (Timer3)
        if (repeat[1] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[1]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(1, 1);    // Вкл. "RELAY1" и "LED_GREAN"
        mode3.start();        // Запускаем таймер для режима 3 (для всех 4-х скоростей)
        flag_Array(1);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_0:
        //Кнопка 0: 2 режим (Timer3)
        if (repeat[2] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[2]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(2, 2);    // Вкл. "RELAY2" и "LED_BLUE"
        mode3.start();        // Запускаем таймер для режима 3 (для всех 4-х скоростей)
        flag_Array(2);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_T_SEARCH:
        //Кнопка T-SEARCH: 3 режим (Timer3)
        if (repeat[3] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[3]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(3, 3);    // Вкл. "RELAY3" и "LED_RED"
        mode3.start();        // Запускаем таймер для режима 3 (для всех 4-х скоростей)
        flag_Array(3);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_STEP:
        if (repeat[1] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[1]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(1, 1);    // Вкл. "RELAY1" и "LED_GREAN"
        mode4.start();        // Запускаем таймер для режима 4 (для всех 4-х скоростей)
        flag_Array(1);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_PLAY_PAUSE:
        //Кнопка PLAY/PAUSE: 2 режим (Timer4)
        if (repeat[2] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[2]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(2, 2);    // Вкл. "RELAY2" и "LED_BLUE"
        mode4.start();        // Запускаем таймер для режима 4 (для всех 4-х скоростей)
        flag_Array(2);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_STOP:
        //Кнопка STOP: 3 режим (Timer4)
        if (repeat[3] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[3]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(3, 3);    // Вкл. "RELAY3" и "LED_RED"
        mode4.start();        // Запускаем таймер для режима 4 (для всех 4-х скоростей)
        flag_Array(3);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_SUBTITLE:
        //Кнопка SUBTITLE: LOW (On)
        if (repeat[4] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[4]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(4, 1);    // Вкл. "RELAY4" и "LED_GREAN"
        blink_LOW.start();    // Запускаем таймер для мигающего LED в режиме LOW
        blink_ON = true;      // Поднимаем флаг для мигающего LED в режиме LOW
        flag_Array(4);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_ANGLE:
        //Кнопка ANGLE: LOW (Timer1)
        if (repeat[4] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[4]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(4, 1);    // Вкл. "RELAY4" и "LED_GREAN"
        mode1.start();        // Запускаем таймер для режима 1 (для LOW скорости)
        blink_LOW.start();    // Запускаем таймер для мигающего LED в режиме LOW
        blink_ON = true;      // Поднимаем флаг для мигающего LED в режиме LOW
        flag_Array(4);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_AUDIO:
        //Кнопка AUDIO: LOW (Timer2)
        if (repeat[4] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[4]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(4, 1);    // Вкл. "RELAY4" и "LED_GREAN"
        mode2.start();        // Запускаем таймер для режима 2 (для LOW скорости)
        blink_LOW.start();    // Запускаем таймер для мигающего LED в режиме LOW
        blink_ON = true;      // Поднимаем флаг для мигающего LED в режиме LOW
        flag_Array(4);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_ZOOM:
        //Кнопка ZOOM: LOW (Timer3)
        if (repeat[4] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[4]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(4, 1);    // Вкл. "RELAY4" и "LED_GREAN"
        mode3.start();        // Запускаем таймер для режима 3 (для LOW скорости)
        blink_LOW.start();    // Запускаем таймер для мигающего LED в режиме LOW
        blink_ON = true;      // Поднимаем флаг для мигающего LED в режиме LOW
        flag_Array(4);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_RANDOM:
        //Кнопка RANDOM: LOW (Timer4)
        if (repeat[4] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[4]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(4, 1);    // Вкл. "RELAY4" и "LED_GREAN"
        mode4.start();        // Запускаем таймер для режима 4 (для LOW скорости)
        blink_LOW.start();    // Запускаем таймер для мигающего LED в режиме LOW
        blink_ON = true;      // Поднимаем флаг для мигающего LED в режиме LOW
        flag_Array(4);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_PROG:
        //Кнопка PROG: LOW (Timer5)
        if (repeat[4] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[4]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(4, 1);    // Вкл. "RELAY4" и "LED_GREAN"
        mode5.start();        // Запускаем таймер для режима 5 (для LOW скорости)
        blink_LOW.start();    // Запускаем таймер для мигающего LED в режиме LOW
        blink_ON = true;      // Поднимаем флаг для мигающего LED в режиме LOW
        flag_Array(4);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;

      case IR_KARAOKE:
        //Кнопка KARAOKE: LOW (Timer6)
        if (repeat[4] == 0) repeat[0] = 1;    // Если у элемента массива "repeat[4]" флаг опущен, то поднимаем флаг первому элементу "repeat[0]" для задержки "DELAY_REL_OFF"
        WriteRelLed(4, 1);    // Вкл. "RELAY4" и "LED_GREAN"
        mode6.start();        // Запускаем таймер для режима 6 (для LOW скорости)
        blink_LOW.start();    // Запускаем таймер для мигающего LED в режиме LOW
        blink_ON = true;      // Поднимаем флаг для мигающего LED в режиме LOW
        flag_Array(4);        // Поднимаем заданный флаг в скобках, а все остальные опускаем
        break;
    }
  }
}

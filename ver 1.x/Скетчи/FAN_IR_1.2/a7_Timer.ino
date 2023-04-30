//Функция отключения всех РЕЛЕ, LED и Автоповоротов по истечению работы таймера
void mode_Timer() {
  if (mode1.tick() == true) {         // Если таймер режима 1 запущен/перезапущен (пришла команда с ИК пульта), то...
    if (mode1.active() == false) {    // Проверяем работает ли таймер (start/resume), если НЕ работает, то...
      WriteRelLed();          // Выключаем всё
      mode1.stop();           // Останавливаем таймер
      auto_RotatON = false;   // Опускаем флаг для Автоповоротов
      fram.WriteInt(PAGE_FRAM, SECTOR_FRAM, servo_val);   //Записываем в сектор "SECTOR_FRAM", "PAGE_FRAM" страницы значение "servo_val"
    }
  }

  if (mode2.tick() == true) {         // Если таймер режима 2 запущен/перезапущен (пришла команда с ИК пульта), то...
    if (mode2.active() == false) {    // Проверяем работает ли таймер (start/resume), если НЕ работает, то...
      WriteRelLed();          // Выключаем всё
      mode2.stop();           // Останавливаем таймер
      auto_RotatON = false;   // Опускаем флаг для Автоповоротов
      fram.WriteInt(PAGE_FRAM, SECTOR_FRAM, servo_val);   //Записываем в сектор "SECTOR_FRAM", "PAGE_FRAM" страницы значение "servo_val"
    }
  }

  if (mode3.tick() == true) {         // Если таймер режима 3 запущен/перезапущен (пришла команда с ИК пульта), то...
    if (mode3.active() == false) {    // Проверяем работает ли таймер (start/resume), если НЕ работает, то...
      WriteRelLed();          // Выключаем всё
      mode3.stop();           // Останавливаем таймер
      auto_RotatON = false;   // Опускаем флаг для Автоповоротов
      fram.WriteInt(PAGE_FRAM, SECTOR_FRAM, servo_val);   //Записываем в сектор "SECTOR_FRAM", "PAGE_FRAM" страницы значение "servo_val"
    }
  }

  if (mode4.tick() == true) {         // Если таймер режима 4 запущен/перезапущен (пришла команда с ИК пульта), то...
    if (mode4.active() == false) {    // Проверяем работает ли таймер (start/resume), если НЕ работает, то...
      WriteRelLed();          // Выключаем всё
      mode4.stop();           // Останавливаем таймер
      auto_RotatON = false;   // Опускаем флаг для Автоповоротов
      fram.WriteInt(PAGE_FRAM, SECTOR_FRAM, servo_val);   //Записываем в сектор "SECTOR_FRAM", "PAGE_FRAM" страницы значение "servo_val"
    }
  }


  if (mode5.tick() == true) {         // Если таймер режима 5 запущен/перезапущен (пришла команда с ИК пульта), то...
    if (mode5.active() == false) {    // Проверяем работает ли таймер (start/resume), если НЕ работает, то...
      WriteRelLed();          // Выключаем всё
      mode5.stop();           // Останавливаем таймер
      auto_RotatON = false;   // Опускаем флаг для Автоповоротов
      fram.WriteInt(PAGE_FRAM, SECTOR_FRAM, servo_val);   //Записываем в сектор "SECTOR_FRAM", "PAGE_FRAM" страницы значение "servo_val"
    }
  }


  if (mode6.tick() == true) {         // Если таймер режима 6 запущен/перезапущен (пришла команда с ИК пульта), то...
    if (mode6.active() == false) {    // Проверяем работает ли таймер (start/resume), если НЕ работает, то...
      WriteRelLed();          // Выключаем всё
      mode6.stop();           // Останавливаем таймер
      auto_RotatON = false;   // Опускаем флаг для Автоповоротов
      fram.WriteInt(PAGE_FRAM, SECTOR_FRAM, servo_val);   //Записываем в сектор "SECTOR_FRAM", "PAGE_FRAM" страницы значение "servo_val"
    }
  }
}

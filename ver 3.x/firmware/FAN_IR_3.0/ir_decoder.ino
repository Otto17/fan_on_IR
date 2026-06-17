// Copyright (c) 2026 Otto
// Лицензия: MIT (см. LICENSE)

// ============================================================
// Обработка команд ИК пульта
// ============================================================

// ir_Decoder — читает команду с ИК пульта и выполняет соответствующее действие
void ir_Decoder() {
  if (!ir.available()) return;

  noInterrupts();
  uint8_t cmd = ir.readCommand();
  interrupts();

  switch (cmd) {

    case IR_OFF:
      WriteRelLed();
      RotatON = false;
      auto_RotatON = false;
      flagRW_UP();
      break;

    case IR_OK:
      flagLED = !flagLED;
      forceUpdateLED();  // Форсирование обновления LED при смене flagLED
      break;

    case IR_UP:
      RotatON = false;
      auto_RotatON = true;
      break;

    case IR_DOWN:
      RotatON = false;
      auto_RotatON = false;
      flagRW_UP();
      break;

    case IR_LEFT:
      auto_RotatON = false;
      RotatON = true;
      Right_Turns = true;
      Left_Turns = false;
      manual_last_ir_ms = millis();
      break;

    case IR_RIGHT:
      auto_RotatON = false;
      RotatON = true;
      Left_Turns = true;
      Right_Turns = false;
      manual_last_ir_ms = millis();
      break;

    case IR_1:
      if (!repeat[1]) repeat[0] = true;
      WriteRelLed(1, 1);
      mode1.startTimer();
      flag_Array(1);
      break;

    case IR_4:
      if (!repeat[1]) repeat[0] = true;
      WriteRelLed(1, 1);
      mode2.startTimer();
      flag_Array(1);
      break;

    case IR_2:
      if (!repeat[2]) repeat[0] = true;
      WriteRelLed(2, 2);
      mode1.startTimer();
      flag_Array(2);
      break;

    case IR_5:
      if (!repeat[2]) repeat[0] = true;
      WriteRelLed(2, 2);
      mode2.startTimer();
      flag_Array(2);
      break;

    case IR_3:
      if (!repeat[3]) repeat[0] = true;
      WriteRelLed(3, 3);
      mode1.startTimer();
      flag_Array(3);
      break;

    case IR_6:
      if (!repeat[3]) repeat[0] = true;
      WriteRelLed(3, 3);
      mode2.startTimer();
      flag_Array(3);
      break;

    case IR_PLAY_PAUSE:
      if (!repeat[4]) repeat[0] = true;
      WriteRelLed(4, 4);
      regim_LED = 4;
      mode1.startTimer();
      flag_Array(4);
      break;

    case IR_STOP:
      if (!repeat[4]) repeat[0] = true;
      WriteRelLed(4, 4);
      regim_LED = 4;
      mode2.startTimer();
      flag_Array(4);
      break;

    default:
      break;
  }
}
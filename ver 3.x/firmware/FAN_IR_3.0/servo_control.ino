// Copyright (c) 2026 Otto
// Лицензия: MIT (см. LICENSE)

// ============================================================
//  Программный сервопривод
//  Период:  SERVO_PERIOD мс (20 мс = 50 Гц)
//  Импульс: SERVO_MIN_US..SERVO_MAX_US мкс
// ============================================================

// Максимально допустимая длительность импульса с запасом (мкс)
// SERVO_MAX_US = 2400, берёт x2 запас на случай сбоя таймера
#define SERVO_PULSE_TIMEOUT (SERVO_MAX_US * 2UL)

// Интервал принудительного обновления положения серво (мс)
// Если серво потеряло контакт и восстановило — через это время само вернётся в правильное положение без перезагрузки МК
#define SERVO_REFRESH_INTERVAL 1000UL

// Таймер последнего обновления servoWrite() для принудительного обновления
static uint32_t servo_refresh_tmr = 0;

// servoWrite — переводит угол (градусы) в длительность импульса и сохраняет в servo_us
void servoWrite(int angle) {
  angle = constrain(angle, 0, 180);
  servo_us = (uint16_t)map(angle, 0, 180, SERVO_MIN_US, SERVO_MAX_US);
  servo_refresh_tmr = millis();  // сбрасываем таймер при каждой записи
}

// servoTick — тикер серво: генерирует ШИМ импульсы и восстанавливает серво после сбоя
void servoTick() {
  uint32_t now_us = micros();
  uint32_t now_ms = millis();

  if (servo_pulse_active) {
    // Нормальное завершение импульса
    if (now_us - servo_tmr_us >= (uint32_t)servo_us) {
      digitalWrite(SERVOMOTOR, LOW);
      servo_pulse_active = false;
    }
    // Аварийное завершение: импульс завис дольше допустимого
    // Защита от зависания при сбое контакта или таймера
    else if (now_us - servo_tmr_us >= SERVO_PULSE_TIMEOUT) {
      digitalWrite(SERVOMOTOR, LOW);
      servo_pulse_active = false;
      servo_tmr_ms = now_ms;  // Сброс таймера периода
    }
  } else {
    // Принудительное обновление положения серво раз в SERVO_REFRESH_INTERVAL
    // Восстанавливает серво после потери контакта без перезагрузки МК
    if (now_ms - servo_refresh_tmr >= SERVO_REFRESH_INTERVAL) {
      servo_refresh_tmr = now_ms;
      servoWrite(servo_val);  // Переотправка текущего положения
    }

    // Штатная генерация импульса каждые SERVO_PERIOD мс
    if (now_ms - servo_tmr_ms >= SERVO_PERIOD) {
      servo_tmr_ms = now_ms;
      digitalWrite(SERVOMOTOR, HIGH);
      servo_tmr_us = micros();
      servo_pulse_active = true;
    }
  }
}

// ============================================================
//  Автоповороты
// ============================================================

// auto_Rotation — двигает серво между LEFT_CORNER и RIGHT_CORNER с заданным интервалом
void auto_Rotation() {
  if (millis() - tmr_auto < AUTO_DELAY_TIME_SERVO) return;
  tmr_auto = millis();

  servo_val += dir;

  if (servo_val >= RIGHT_CORNER) {
    servo_val = RIGHT_CORNER;
    dir = -AUTO_DIR_ITERATION;
  } else if (servo_val <= LEFT_CORNER) {
    servo_val = LEFT_CORNER;
    dir = AUTO_DIR_ITERATION;
  }

  servoWrite(servo_val);
}

// ============================================================
//  Ручные повороты
// ============================================================
#define MANUAL_IR_TIMEOUT 200

// manual_Turns — плавно поворачивает серво по командам с пульта, сохраняет положение при отпускании
void manual_Turns() {
  uint32_t now = millis();

  if (now - manual_last_ir_ms > MANUAL_IR_TIMEOUT) {
    if (RotatON) {
      ee_flag = true;
      ee_tmr = now;
    }
    Right_Turns = false;
    Left_Turns = false;
    RotatON = false;
    return;
  }

  if (now - tmr_manual < MANUAL_DELAY_TIME_SERVO) return;
  tmr_manual = now;

  if (Right_Turns) servo_val += COUNT_DIR_ITERATION;
  if (Left_Turns) servo_val -= COUNT_DIR_ITERATION;

  servo_val = constrain(servo_val, LEFT_CORNER, RIGHT_CORNER);
  servoWrite(servo_val);
}
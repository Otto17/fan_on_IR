// Copyright (c) 2026 Otto
// Лицензия: MIT (см. LICENSE)

// ============================================================
//  Setup & Loop
// ============================================================

// setup — инициализация пинов, EEPROM, серво, прерывания, таймеров и WatchDog
void setup() {
  for (uint8_t r = 0; r < 4; r++) pinMode(Relay_Array[r], OUTPUT);
  for (uint8_t l = 0; l < 3; l++) pinMode(Led_Array[l], OUTPUT);

  for (uint8_t r = 0; r < 4; r++) digitalWrite(Relay_Array[r], !RELAY_TYPE);
  for (uint8_t l = 0; l < 3; l++) digitalWrite(Led_Array[l], LOW);

  pinMode(SERVOMOTOR, OUTPUT);
  digitalWrite(SERVOMOTOR, LOW);

  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_IR), irIsr, FALLING);

  mode1.begin(TIMER_MODE1);
  mode2.begin(TIMER_MODE2);

  EEPROM_Read();
  servo_val = eePos;

  if (servo_val > RIGHT_CORNER || servo_val < LEFT_CORNER) {
    servo_val = (RIGHT_CORNER + LEFT_CORNER) / 2;
  }

  servoWrite(servo_val);
  servo_tmr_ms = millis();

#if (WDT_ENABLE == 1)
  WTD_on();
#endif
}

// loop — главный цикл: тикер серво, ИК, таймеры, LED, повороты, запись в EEPROM
void loop() {
  servoTick();

#if (WDT_ENABLE == 1)
  asm("wdr");
#endif

  ir_Decoder();
  mode_Timer();
  updateLED();

  if (auto_RotatON) auto_Rotation();
  if (RotatON) manual_Turns();

  if (flagWR) {
#if (WDT_ENABLE == 1)
    asm("wdr");  // Сброс WDT перед записью в EEPROM
#endif
    EEPROM_Write(servo_val);
    flagWR = false;
  }

  if (ee_flag && (millis() - ee_tmr >= DELAY_EE_TIME)) {
    ee_tmr = millis();
    flagRW_UP();
    ee_flag = false;
  }
}

// ============================================================
// Включение аппаратного WatchDog (перезагрузка МК при зависании)
// ============================================================
#if (WDT_ENABLE == 1)

// WTD_on — включает аппаратный WatchDog
void WTD_on() {
  asm("wdr");

#if (WTD_CPU == 1)
  // ATmega8
  WDTCR |= (1 << WDCE) | (1 << WDE);
  WDTCR = (1 << WDE) | (1 << WDP2) | (1 << WDP1) | (1 << WDP0);
#else
  // ATmega48
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDE) | (1 << WDP2) | (1 << WDP1) | (1 << WDP0);
#endif
}

#endif
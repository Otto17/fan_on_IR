// Copyright (c) 2026 Otto
// Лицензия: MIT (см. LICENSE)

// ============================================================
// Управление реле и LED индикацией
// ============================================================

// Флаг принудительного обновления LED
static bool led_force_update = true;

// forceUpdateLED — принудительно форсирует следующее обновление LED независимо от кэша
void forceUpdateLED() {
  led_force_update = true;
}

// WriteRelLed — выключает все реле, включает нужное, запоминает режим LED
void WriteRelLed(uint8_t pin_Rel, uint8_t pin_Led) {
  for (uint8_t r = 0; r < 4; r++) {
    digitalWrite(Relay_Array[r], !RELAY_TYPE);
  }

  mode1.stopTimer();
  mode2.stopTimer();

  if (repeat[0]) {
#if (WDT_ENABLE == 1)
    asm("wdr");
#endif
    delay(DELAY_REL_OFF);
    repeat[0] = false;
  }

  digitalWrite(Relay_Array[pin_Rel - 1], RELAY_TYPE);
  regim_LED = pin_Led;
  forceUpdateLED();
}

// WriteRelLed — выключает все реле, сбрасывает таймеры и флаги (полное отключение)
void WriteRelLed() {
  for (uint8_t r = 0; r < 4; r++) {
    digitalWrite(Relay_Array[r], !RELAY_TYPE);
  }

  mode1.stopTimer();
  mode2.stopTimer();

  for (uint8_t i = 0; i < sizeof(repeat); i++) {
    repeat[i] = false;
  }

  regim_LED = 0;
  forceUpdateLED();
}

// flag_Array — поднимает флаг нужного реле, все остальные флаги опускает
void flag_Array(uint8_t flag_idx) {
  for (uint8_t i = 0; i < sizeof(repeat); i++) {
    repeat[i] = false;
  }
  if (flag_idx < sizeof(repeat)) {
    repeat[flag_idx] = true;
  }
}

// updateLED — обновляет состояние LED индикаторов, срабатывает только при изменении состояния
void updateLED() {
  static bool prev_flagLED = true;
  static uint8_t prev_regim_LED = 255;

  // Обновление только при изменении состояния или по принудительному флагу
  if (!led_force_update && flagLED == prev_flagLED && regim_LED == prev_regim_LED) return;

  led_force_update = false;
  prev_flagLED = flagLED;
  prev_regim_LED = regim_LED;

  PORTB &= ~((1 << 2) | (1 << 3) | (1 << 4));

  if (!flagLED) return;

  switch (regim_LED) {
    case 1: digitalWrite(Led_Array[0], HIGH); break;  // GREEN
    case 2: digitalWrite(Led_Array[1], HIGH); break;  // BLUE
    case 3: digitalWrite(Led_Array[2], HIGH); break;  // RED
    case 4:
      digitalWrite(Led_Array[0], HIGH);  // GREEN + RED
      digitalWrite(Led_Array[2], HIGH);
      break;
    default: break;
  }
}


// mode_Timer — проверяет таймеры автовыключения и отключает вентилятор по истечении
void mode_Timer() {
  if (mode1.tick()) {
    WriteRelLed();
    auto_RotatON = false;
    flagRW_UP();
  }

  if (mode2.tick()) {
    WriteRelLed();
    auto_RotatON = false;
    flagRW_UP();
  }
}
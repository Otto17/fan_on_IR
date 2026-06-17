// Copyright (c) 2026 Otto
// Лицензия: MIT (см. LICENSE)

// ============================================================
// Работа с EEPROM (кольцевой буфер)
// ============================================================

// EEPROM_Read — читает положение серво из кольцевого буфера EEPROM при старте
void EEPROM_Read() {
  bool found = false;
  for (uint16_t i = 0; i < EEPROM.length(); i++) {
    if (EEPROM.read(i) == EEPROM_MARKER) {
      uint16_t data_addr = (i == 0) ? EEPROM.length() - 1 : i - 1;
      eePos = EEPROM.read(data_addr);
      addr = data_addr;  // Следующая запись начнётся отсюда
      found = true;
      break;
    }
  }
  if (found) {
    if (eePos > RIGHT_CORNER || eePos < LEFT_CORNER) {
      eePos = (RIGHT_CORNER + LEFT_CORNER) / 2;
    }
  } else {
    eePos = (RIGHT_CORNER + LEFT_CORNER) / 2;
    addr = 0;
  }
}

// EEPROM_Write — записывает положение серво в следующую пару ячеек кольцевого буфера
void EEPROM_Write(uint8_t dataWrite) {
  uint16_t data_addr = addr;
  uint16_t marker_addr = (addr + 1) % EEPROM.length();

  EEPROM.update(data_addr, dataWrite);       // Данные
  EEPROM.write(marker_addr, EEPROM_MARKER);  // Маркер после данных

  addr = (addr + 2) % EEPROM.length();  // Шаг = 2 ячейки
}

// flagRW_UP — поднимает флаг разрешения записи в EEPROM
void flagRW_UP() {
  flagWR = true;
}
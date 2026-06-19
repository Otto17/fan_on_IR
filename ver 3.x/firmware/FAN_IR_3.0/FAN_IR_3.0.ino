// Copyright (c) 2026 Otto
// Лицензия: MIT (см. LICENSE)

// ============================================================
//  FAN_IR — Управление напольным вентилятором через ИК пульт
//  Протокол: NEC | МК: ATmega48 / ATmega8
//  Реле  — переключение скоростей вентилятора (4 режима)
//  Серво — автоповороты и ручное управление направлением
// ============================================================

#include "config.h"
#include "globals.h"

#include <NecDecoder.h>
#include <EEPROM.h>

#if (WDT_ENABLE == 1)
#include <avr/wdt.h>
#endif

// ============================================================
//  Объекты
// ============================================================
NecDecoder ir;

// ============================================================
//  Определения глобальных переменных
// ============================================================
TimerOnce mode1;
TimerOnce mode2;

bool flagLED = true;
uint8_t regim_LED = 0;

const uint8_t Relay_Array[4] = { RELAY1, RELAY2, RELAY3, RELAY4 };
const uint8_t Led_Array[3] = { LED_GREAN, LED_BLUE, LED_RED };

bool flagWR = false;
uint16_t addr = 0;
uint8_t eePos = 0;

uint32_t ee_tmr = 0;
bool ee_flag = false;

uint16_t servo_us = SERVO_MIN_US;
uint32_t servo_tmr_ms = 0;
uint32_t servo_tmr_us = 0;
bool servo_pulse_active = false;

uint8_t servo_val = 0;
int dir = AUTO_DIR_ITERATION;
bool auto_RotatON = false;
uint32_t tmr_auto = 0;

bool RotatON = false;
bool Right_Turns = false;
bool Left_Turns = false;
uint32_t manual_last_ir_ms = 0;
uint32_t tmr_manual = 0;

bool repeat[5] = { false, false, false, false, false };

// ============================================================
//  Прерывание ИК приёмника
// ============================================================

// irIsr — обработчик прерывания ИК приёмника, вызывается по FALLING фронту
void irIsr() {
  ir.tick();
}
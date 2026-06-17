/*
  *
  * Приём команд с пульта.
  * Приёмник подключать на прерывание по FALLING (pin D2).
  * 
  * Загружаем скетч в любую Ардуину с TTL конвертером, подключаем IR приёмник (+, -, D2),
  * открываем Serial и тыкаем кнопку на пульте, копируем из Serial 8-ми битный короткий код
  * и вставляем вместо моих кодов в скетче на вкладке "a0_Settings".
  *
*/

#include <NecDecoder.h>  // Подключаем библиотеку для протокола NEC
NecDecoder ir;

void setup() {
  Serial.begin(9600);                  // Инициализируем Serial
  attachInterrupt(0, irIsr, FALLING);  // Подключил на D2, прерывание 0
}

//В прерывании вызываем tick()
void irIsr() {
  ir.tick();
}

void loop() {
  if (ir.available()) {  // Если пакет успешно принят

    Serial.print("0x");
    Serial.println(ir.readCommand(), HEX);  // Выводим только только команду (8 бит)
  }
}
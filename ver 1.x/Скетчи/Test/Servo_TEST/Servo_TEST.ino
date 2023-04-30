//Servo (pin)
#define SERVOMOTOR 9  // pin для подключения Servo

//Прерывания (pin)
#define INTERRUPT_PIN_IR 2    // Пин с прерыванием к которому подключён ИК приёмник

//Настройки для ручных и автоповоротов Servo
#define RIGHT_CORNER 130            // Задаём максимальный поворот на ПРАВЫЙ угол
#define LEFT_CORNER 12              // Задаём максимальный поворот на ЛЕВЫЙ угол

#define MAX_VALUE 20000              // Максимальное значение для задания скорости движения Servo при ручных поворотах
#define MIN_VALUE 0                 // Минимальное значение для задания скорости движения Servo при ручных поворотах

#define AUTO_DELAY_TIME_SERVO 50    // Время повторений итераций(в миллисекундах) для автоповоротов. Чем больше время, тем медленее будут автоповороты
#define TURNS_DELAY_TIME_SERVO 100  // Время (в миллисекундах) после истечения которого опускаем флаги (если не поступил новый сигнал) при ручных поворотах Servo

#include <SoftServo.h>    // Библиотека для программного управления Servo (на базе millis/micros)
#include <NecDecoder.h>   // Лёгкая библиотека для декодирования ИК протокола NEC

//Коды кнопок пульта в HEX
#define IR_UP         0x1     // Кнопка вверх: ↑ Авто - Вращения Вкл.
#define IR_DOWN       0x81    // Кнопка вниз:  ↓ Авто - Вращения Выкл.
#define IR_LEFT       0x8A    // Кнопка <: « Влево
#define IR_RIGHT      0xB2    // Кнопка >: Вправо »


SoftServo rotation;       // Создаём Объект для Sertvo
NecDecoder ir;            // Создаём Объект для ИК приёмника

//Автоповороты Servo
int auto_val = 0;
int dir = 1;       // Градус поворота для каждой итерации функции
bool auto_RotatON  = false;     // Если "true", то включаем автоповороты Servo
uint32_t tmr_auto;                // Последнее обновление времени (для автоповоротов Servo)


//Ручные повороты Servo
int transformation = 0;     // Переменная для преобразования ручных поворотов через map()
int counter = 1;               // Увеличиваем перемещение на каждом шаге для ручных поворотов
bool RotatON  = false;      // Если "true", то включаем повороты Servo(влево - вправо)
bool Right_Turns = false;   // Если "true", то поворачиваем вручную Servo вправо (при зажатии кнопки вправо)
bool Left_Turns  = false;   // Если "true", то поворачиваем вручную Servo влево (при зажатии кнопки влево)
uint32_t tmr_turn = 0;      // Последнее обновление времени (для ручных поворотов Servo)


void setup() {
  //Получение ИК сигнала через прерывания
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_IR), irIsr, FALLING);

  if (auto_val > RIGHT_CORNER || auto_val < LEFT_CORNER) auto_val = (RIGHT_CORNER - LEFT_CORNER) / 2;   // Если считанный из EEPROM угол выходит за пределы заданных, то переводим серво в центральное положение от заданных углов
  rotation.attach(SERVOMOTOR);  // Вкл. Servo
  rotation.delayMode();         // переключить в режим задержки (по умолчанию)
  rotation.write(auto_val);     // Записываем положение Servo из FRAM после включения питания
}


void loop() {
  rotation.tick();   //Вызываем тикер для работы "SoftServo.h"

  if (auto_RotatON == true) auto_Rotation();   // Запускаем функцию вкл. автоповоротов Servo
  if (RotatON == true) manual_Turns();         // Иначе работает функция ручных поворотов Servo
  ir_Decoder();   // Проверяем функцию "ir_Decoder()" на наличие приёма команд с ИК пульта
}


void irIsr() {
  ir.tick();    // Вызывать при ОТРИЦАТЕЛЬНОМ (FALLING) фронте на пине ИК приемника в прерывании

  //При удержании любой кнопки срабатывает прерывание и перехватываем пустой ИК сигнал, считаем его за код кнопки. При отпускании кнопки считаем, что код перестал приниматься
  tmr_turn = millis();    // Записываем текущее время (для ручных поворотов Servo)
}


void ir_Decoder() {
  //Получаем ИК коды
  if (ir.available()) {             // Если пакет успешно принят, то...
    switch (ir.readCommand()) {   // Читаем команду
      case IR_UP:
        //Кнопка вверх: ↑ Авто - Вращения Вкл.
        RotatON = false;          // Опускаем флаг для ручных поворотов
        auto_RotatON = true;      // Поднимаем флаг для Вкл. Автоповоротов
        break;

      case IR_DOWN:
        //Кнопка вниз:  ↓ Авто - Вращения Выкл.
        RotatON = false;          // Опускаем флаг для ручных поворотов
        auto_RotatON = false;     // Опускаем флаг для Автоповоротов
        break;

      case IR_LEFT:
        //Кнопка <: « Влево
        auto_RotatON = false;     // Опускаем флаг для Автоповоротов
        RotatON = true;           // Поднимаем флаг для ручных поворотов
        Right_Turns = true;       // Поднимаем флаг при нажатии и удержании кнопки "« Влево"
        break;

      case IR_RIGHT:
        //Кнопка >: Вправо »
        auto_RotatON = false;    // Опускаем флаг для Автоповоротов
        RotatON = true;          // Поднимаем флаг для ручных поворотов
        Left_Turns  = true;      // Поднимаем флаг при нажатии и удержании кнопки "Вправо »"
        break;
    }
  }
}


//Функция для Автоповоротов Servo
void auto_Rotation() {
  if (millis() - tmr_auto >= AUTO_DELAY_TIME_SERVO) {    // Если время больше или равно "AUTO_DELAY_TIME_SERVO", тогда...
    tmr_auto = millis();                                    // записываем время в "tmr_auto"
    auto_val += dir;                                         // Прибавляем "auto_val" значение градуса "dir"
    if (auto_val >= RIGHT_CORNER || auto_val <= LEFT_CORNER) dir = -dir;    // Разворачиваем в обратную сторону

    rotation.write(auto_val);  // Записываем угол в Servo
  }
}


//Функция для ручных поворотов Servo (влево - вправо при удержании кнопки)
void manual_Turns() {
  if (millis() - tmr_turn > TURNS_DELAY_TIME_SERVO) (Right_Turns = false), (Left_Turns = false);    // Если вермя больше "TURNS_DELAY_TIME_SERVO", тогда опускам флаги

  if (Right_Turns ==  true && transformation <= MAX_VALUE) transformation += counter;    // Если правый флаг поднят и значение "transformation" меньше "MAX_VALUE", то производим инкремент счётчика
  if (Left_Turns  ==  true && transformation >= MIN_VALUE) transformation -= counter;    // Если левый флаг поднят и значение "transformation" больше "MIN_VALUE", то производим декремент счётчика

  auto_val = constrain(auto_val, LEFT_CORNER, RIGHT_CORNER);                             // Ограничим диапазон для безопасности от "LEFT_CORNER" до "RIGHT_CORNER"
  auto_val = map(transformation, MIN_VALUE, MAX_VALUE, LEFT_CORNER, RIGHT_CORNER);       // Задаём диапазон скорости и поворота Servo (углы поворота от "LEFT_CORNER" до "RIGHT_CORNER" градусов)

  rotation.write(auto_val);    // Записываем положение в Servo
}

/*** БИБЛИОТЕКИ ***/
#include <TimerMs.h>      // Многофункциональный программный таймер на системном таймере millis() для Arduino
#include <SoftServo.h>    // Библиотека для программного управления Servo (на базе millis/micros)
#include <FRAM.h>         // Библиотека для работы с FRAM памятью (записываем положение Servo)
#include <microWire.h>    // Подключаем лёгкую библиотека со стандартным набором инструментов для работы с аппаратным I2C

#include <IRremote.h>     // ВДля Atmega8 нехватает памяти, поэтому в библиотеке "IRremote.h"/src открываем файл "IRremote.hpp"
//и в пункте "PROTOCOLS" комментируем всё, кроме одного нужного протокола. К примеру оставил только "#define DECODE_NEC", остальные дефайны комментирую и сохраняю файл


IRrecv irrecv(INTERRUPT_PIN_IR);
decode_results results;

#if (WDT_ENABLE == 1)
#include <avr/wdt.h>
#endif

/*** ОБЪЕКТЫ ***/
SoftServo rotation;       // Создаём Объект для Sertvo
//NecDecoder ir;            // Создаём Объект для ИК приёмника
FRAM fram(ADDRR_FRAM);    // Задаём адрес i2c для FRAM памяти


// (период, мс), (0 не запущен / 1 запущен), (режим: 0 период / 1 таймер)
TimerMs mode1 (TIMER_MODE1, 0, 1);         // Режим 1 на все 4 скорости (Timer1)
TimerMs mode2 (TIMER_MODE2, 0, 1);         // Режим 2 на все 4 скорости (Timer2)
TimerMs mode3 (TIMER_MODE3, 0, 1);         // Режим 3 на все 4 скорости (Timer3)
TimerMs mode4 (TIMER_MODE4, 0, 1);         // Режим 4 на все 4 скорости (Timer4)
TimerMs mode5 (TIMER_MODE5, 0, 1);         // Режим 5 только на пониженную скорость (Timer5)
TimerMs mode6 (TIMER_MODE6, 0, 1);         // Режим 6 только на пониженную скорость (Timer6)
TimerMs blink_LOW (LED_BLINK_LOW, 0, 1);   // Таймер для мигания "LED_GREAN" в LOW режимах


/*** ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ***/
//Управление пинами МК
const byte Relay_Array[4] = {RELAY1, RELAY2, RELAY3, RELAY4};   // Массив реле
const byte Led_Array[3] = {LED_GREAN, LED_BLUE, LED_RED};       // Массив LED

int readResults;

//Автоповороты Servo
int servo_val = fram.ReadInt(PAGE_FRAM, SECTOR_FRAM);    // Считываем в переменную "servo_val" значения с сектора "SECTOR_FRAM", страницы "PAGE_FRAM"
int dir = AUTO_DIR_ITERATION;   // Градус поворота для каждой итерации функции
bool auto_RotatON  = false;     // Если "true", то включаем автоповороты Servo
uint32_t tmr_auto = 0;          // Последнее обновление времени (для автоповоротов Servo)

//Ручные повороты Servo
int counter = COUNT_DIR_ITERATION;    // Увеличиваем или уменьшаем перемещение на каждом шаге для ручных поворотов
bool RotatON  = false;                // Если "true", то включаем повороты Servo(влево - вправо)
bool Right_Turns = false;             // Если "true", то поворачиваем вручную Servo вправо (при зажатии кнопки вправо)
bool Left_Turns  = false;             // Если "true", то поворачиваем вручную Servo влево (при зажатии кнопки влево)


//Для режима LOW (мигание "LED_GREAN")
bool blink_ON = false;      // Если "true", то включаем мигание LED в режиме LOW


//Массив флагов для защиты от быстрого откл./вкл. (перещёлкивания) реле при повторном приёме ИК сигнала и в пределах одного режима
//Первый элемент массива "нулевой" используется для флага задержки "DELAY_REL_OFF" между переключениями реле, остальные флаги для переключения смены режимов
bool repeat[] = {0, 0, 0, 0, 0};    // Объявляем массив без явного задания размера

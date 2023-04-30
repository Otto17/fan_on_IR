/*** НАСТРОЙКИ ***/
//Пины Реле (pin)
#define RELAY1 4    // Режим 1 - Низкая скорость
#define RELAY2 7    // Режим 1 - Средняя скорость
#define RELAY3 8    // Режим 2 - Высокая скорость
#define RELAY4 3    // Режим 3 - ПОНИЖЕННАЯ скорость первого режима через конденсатор

//Настройки Реле (ВКЛЮЧЕНИЕ по плюсу или минусу на управляющие контакты)
#define RELAY_TYPE LOW      // Выбор типа реле LOW или HIGH. (LOW - ВКЛЮЧЕНИЕ реле по МИНУСУ, HIGH - ВКЛЮЧЕНИЕ реле по ПЛЮСУ)
#define DELAY_REL_OFF 50    // Ожидание "delay()" (в миллисекундах), пока контакты реле разъединятся после отключения реле, что бы исключить одновременную работу двух и более реле при переключении

// - - - - - - - - - - - - - - //

//Led индикация (pin)
#define LED_GREAN 10   // Зелёный светодиод для индикации режима 1
#define LED_BLUE  11   // Синий светодиод для индикации режима 2
#define LED_RED   12   // Красный светодиод для индикации режима 3

//Настройки LED
#define LED_BLINK_LOW 1500   // Частота мигания индикатора "LED_GREAN" (в миллисекундах) для режима ПОНИЖЕННОЙ скорости

// - - - - - - - - - - - - - - //

//WatchDog для Atmega8. Примерное время срабатывания 2.2 сек (MAX время для Atmega8)
#define WDT_ENABLE 1          // 1 - включить, 0 - отключить WatchDog

//Прерывания (pin)
#define INTERRUPT_PIN_IR 2    // Пин с прерыванием к которому подключён ИК приёмник

//Servo (pin)
#define SERVOMOTOR 9  // pin для подключения Servo

// - - - - - - - - - - - - - - //

//FRAM
#define ADDRR_FRAM 0x56   // Задаём адрес i2c для FRAM памяти в HEX формате
#define PAGE_FRAM   0     // Страница на FRAM памяти, куда будем писать данные (0 - первая страница, 1 - вторая страница)
#define SECTOR_FRAM 73    // Сектор в FRAM памяти, куда будем писать данные

// - - - - - - - - - - - - - - //

//Настройки таймера отключения вентилятора (в миллисекундах)
#define TIMER_MODE1 1800000    // Задаём время для таймеров на отключение вентилятора (30 мин)
#define TIMER_MODE2 3600000    // (60 мин)
#define TIMER_MODE3 5400000    // (1 час 30 мин)
#define TIMER_MODE4 7200000    // (2 часа)
#define TIMER_MODE5 10800000   // (3 часа)
#define TIMER_MODE6 14400000   // (4 часа)

// - - - - - - - - - - - - - - //

//Настройки для ручных и автоповоротов Servo
#define RIGHT_CORNER 140             // Задаём максимальный поворот на ПРАВЫЙ угол
#define LEFT_CORNER 10               // Задаём максимальный поворот на ЛЕВЫЙ угол

#define AUTO_DELAY_TIME_SERVO 40    // Время повторений итераций(в миллисекундах) для автоповоротов. Чем больше время, тем медленее будут автоповороты
#define AUTO_DIR_ITERATION 1        // Градус поворота для каждой итерации функции автоповоротов
#define COUNT_DIR_ITERATION 3       // Градус поворота для каждой итерации функции ручных поворотов

// - - - - - - - - - - - - - - //

//Коды кнопок пульта в HEX (8 бит)
#define IR_OK         0x48    // Кнопка OK: Off (Выключить ВСЁ)
#define IR_UP         0x1     // Кнопка вверх: ↑ Авто - Вращения Вкл.
#define IR_DOWN       0x81    // Кнопка вниз:  ↓ Авто - Вращения Выкл.
#define IR_LEFT       0x8A    // Кнопка <: « Влево
#define IR_RIGHT      0xB2    // Кнопка >: Вправо »
#define IR_1          0x80    // Кнопка 1: 1 режим (On)
#define IR_2          0x40    // Кнопка 2: 2 режим (On)
#define IR_3          0xC0    // Кнопка 3: 3 режим (On)
#define IR_4          0x20    // Кнопка 4: 1 режим (Timer1)
#define IR_5          0xA0    // Кнопка 5: 1 режим (Timer1)
#define IR_6          0x60    // Кнопк  6: 1 режим (Timer1)
#define IR_7          0xE0    // Кнопка 7: 2 режим (Timer2)
#define IR_8          0x10    // Кнопка 8: 2 режим (Timer2)
#define IR_9          0x90    // Кнопка 9: 2 режим (Timer2)
#define IR_10         0xA4    // Кнопка +10: 3 режим (Timer3)
#define IR_0          0x50    // Кнопка 0: 3 режим (Timer3)
#define IR_T_SEARCH   0x64    // Кнопка T-SEARCH: 3 режим (Timer3)
#define IR_STEP       0x0     // Кнопка STEP: 4 режим (Timer4)
#define IR_PLAY_PAUSE 0xA8    // Кнопка PLAY/PAUSE: 4 режим (Timer4)
#define IR_STOP       0x28    // Кнопка STOP: 4 режим (Timer4)
#define IR_SUBTITLE   0x14    // Кнопка SUBTITLE: LOW (On)
#define IR_ANGLE      0x94    // Кнопка ANGLE: LOW 1 режим (Timer1)
#define IR_AUDIO      0xE4    // Кнопка AUDIO: LOW 2 режим (Timer2)
#define IR_ZOOM       0x2     // Кнопка ZOOM: LOW 3 режим (Timer3)
#define IR_RANDOM     0x74    // Кнопка RANDOM: LOW 4 режим (Timer4)
#define IR_PROG       0x54    // Кнопка PROG: LOW 5 режим (Timer5)
#define IR_KARAOKE    0xA2    // Кнопка KARAOKE: LOW 6 режим (Timer6)

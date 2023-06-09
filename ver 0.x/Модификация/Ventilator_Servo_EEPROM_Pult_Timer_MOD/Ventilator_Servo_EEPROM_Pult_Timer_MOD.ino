/*
  Напольный вентилятор с управлением через ИК пульт
  Стандартные 3 режима скорости по таймеру и без него + возможность управлять поворотами потора через Servo в ручном режиме и авто режим.
  При откл. питания Servo сохраняет свою позиция угла, и при последующим Вкл. не поворачивается в нулевой градус, а остаётся на месте.
  Так же используется кольцевой буфер для EEPROM, что бы выроянять износ памяти.

  Изменения в версиях кода:
  v0.1 - Создал скетч с Вкл./Выкл. через ИК пульт одного из 3-х режимов вентилятора + 3-х цветная индикация каждого режима RGB светодиодами.  Создан: 11 Августа 2016 года.
  v0.2 - Добавил на каждый из 3-х режимов таймер на отключение через 15, 30 или 45 минут через ИК пульт.  Создан: 16 Сентября 2016 года.
  v0.3 - Переделал вентилятор и добавил возможность управления поворотами Влево-Вправо через ИК пульт благодаря Servo-Мотору.   Создан: 01 Января 2017 года.
  v0.4 - Добавил возможность Откл. Автоповоротов Servo при срабатывании таймера на Откл. вентилятора.   Создан: 02 Января 2017 года.
  v0.5 - Добавил библиотеку EEPROM для сохранения последней позиции Servo при Откл. питания.   Создан: 03 Января 2017 года.
  v0.6 - Исправил проблему с задержкой включения Реле по таймерам 1-го режима при включенных Автоповоротах.   Создан: 09 Января 2017 года.
  v0.7 - Добавил последовательную запись по ячейкам EEPROM при Ручных поваротах для сокращения износа памяти. При Откл. и Вкл. питания запись начинается заново с 0 ячейки.  Создан: 10 Января 2017 года.
  v0.8 - Изменил принцип записи/чтения из EEPROM. Теперь при Откл. и Вкл. питания чтение и дальшейшая запись будет продолжаться с последней ячейки памяти до конца и потом оаять сначала, тем самым увеличив срок службы EEPROM!  Создан: 10 Января 2017 года.
  v0.9 - В этой версии Mod'a просто добавил дополнительные коды для ИК-приёмника для управления с ТЕЛЕФОНА и ИК пульта. Создан: 19 Мая 2018 года.

  Автор: Otto
  Версия изменения: 0.9 stable MOD
  Создан: 19 Мая 2018 года.
*/

#include <EEPROM.h>         // Библиотека для  энергонезависимой памяти 
#include <Servo.h>          // Библиотека для Servo-Мотора
#include <IRremote.h>       // Библиотека для ИК приёмника

#define EEPROM_SIZE 512     // Размер EEPROM равный 512 байтам (для Atmega168)
#define EEPROM_EMPTY 0xFF   // Начальный адрес в EEPROM (0xFF - В шестнадцатеричной системе == 255)
#define EEPROM_MARKER 0xFE  // Маркер для EEPROM (0xFE - В шестнадцатеричной системе == 254)

#define REJ1 2   // Режим  1 (реле подкл. к pin 2)
#define REJ2 3   // Режим  2 (реле подкл. к pin 3)
#define REJ3 4   // Режим  3 (реле подкл. к pin 4)

#define GREAN 5  // Зелёный светодиод к режиму 1 (светодиод подкл. к pin 5)
#define BLUE 6   // Синий   светодиод к режиму 2 (светодиод подкл. к pin 6)
#define RED 7    // Красный светодиод к режиму 3 (светодиод подкл. к pin 7)

#define SERVOMOTOR 9  // Servo подключён на pin 9
#define RECV_PIN 11       // Вход ИК приемника подключён к (pin 11)

IRrecv irrecv(RECV_PIN);  // Объект ИК
decode_results results;   // Lекодируем данные в results

Servo ServoPovoroti;   //даём имя Servo

// переменные переключения режимов, при нажатии кнопок они принимают значения +1
bool a = 0; // Режим 1, кнопка на пульте 1 (БЕЗ ТАЙМЕРА)
bool b = 0; // Режим 2, кнопка на пульте 2 (БЕЗ ТАЙМЕРА)
bool c = 0; // Режим 3, кнопка на пульте 3 (БЕЗ ТАЙМЕРА)
bool d = 0; // Для выключения ВСЕХ режимов!

// переменные 1 режима для включения по таймеру:
bool e = 0; // Режим 1, кнопка на пульте 4 (ТАЙМЕР НА 15 МИНУТ)
bool f = 0; // Режим 1, кнопка на пульте 7 (ТАЙМЕР НА 30 МИНУТ)
bool g = 0; // Режим 1, кнопка на пульте * (ТАЙМЕР НА 45 МИНУТ)

// переменные 2 режима для включения по таймеру:
bool h = 0; // Режим 2, кнопка на пульте 5 (ТАЙМЕР НА 15 МИНУТ)
bool i = 0; // Режим 2, кнопка на пульте 8 (ТАЙМЕР НА 30 МИНУТ)
bool j = 0; // Режим 2, кнопка на пульте 0 (ТАЙМЕР НА 45 МИНУТ)

// переменные 3 режима для включения по таймеру:
bool k = 0; // Режим 3, кнопка на пульте 6 (ТАЙМЕР НА 15 МИНУТ)
bool l = 0; // Режим 3, кнопка на пульте 9 (ТАЙМЕР НА 30 МИНУТ)
bool m = 0; // Режим 3, кнопка на пульте # (ТАЙМЕР НА 45 МИНУТ)

// переменные для переключений переменных в режимах с таймером
bool f_e = 0;
bool f_f = 0;
bool f_g = 0;

bool f_h = 0;
bool f_i = 0;
bool f_j = 0;

bool f_k = 0;
bool f_l = 0;
bool f_m = 0;


// переменные для контроля за процессом поворотов Servo:
bool Right_Pin_1   = false;   //для ручного поворота вправо при зажатии кнопки
bool Left_Pin_1    = false;   //для ручного поворота влево  при зажатии кнопки
bool Auto_Povorot  = false;   //для Откл. Автоповорота  Влево-Вправо при кратковременном нажатии


// переменные для Ручных поворотов Servo через Функцию Povorot.
byte E_1 = 0;        // переменная для экономии ресурсов ОЗУ EEPROM
int addr = 0;        // Начальный адрес памяти EEPROM
int L;               // переменная для преобразования ручных поворотов через map()
int pos;             // текущее положение сервы
int increment = 1;   // увеличиваем перемещение на каждом шаге для Ручных поворотов
unsigned long lastUpdate = 0;  // последнее обновление положения


// переменные для Автоповоротов в Функции AutoServoPovoroti.
byte cPos;         // текущая позиция
byte zPos;         // нужная позиция, куда будем двигаться
byte tDelay = 50;  // задержка между движениями, даёт появление плавного движения


//Далее идёт код для таймера...
unsigned long millis_old[9];                      //массив для хранения последних данных о времени срабатывания таймера.Количество элементов соответствует количеству таймеров
bool my_timer(unsigned long time, byte nomer)     //функция таймер, time-время задержки, nomer-номер таймера
{
  unsigned long current_millis;                   //текущее значение миллисекунды
  current_millis = millis();
  if (current_millis - millis_old[nomer] > time)  //если время срабатывания таймера наступило
  {
    millis_old[nomer] = current_millis;           //перезаписываем время последнего срабатывания таймера
    return true;                                  //возвращаем true
  } else return false;                            //возвращаем false
}


void setup() {
  irrecv.enableIRIn();                // включить ИК приёмник
  ServoPovoroti.attach(SERVOMOTOR);   // Servo подключён на цифровой вход (pin 9)

  // назначаем все необходимые pin'ы как ВЫХОДЫ
  pinMode(REJ1, OUTPUT);  // режим 1 ВЫХОД
  pinMode(REJ2, OUTPUT);  // режим 2 ВЫХОД
  pinMode(REJ3, OUTPUT);  // режим 3 ВЫХОД

  pinMode(GREAN, OUTPUT); // Зелёный цвет к режиму 1
  pinMode(BLUE, OUTPUT);  // Синий  цвет  к режиму 2
  pinMode(RED, OUTPUT);   // Красный свет к Режиму 3

  // При включении Arduino на всех пинах LOW.
  // ВНИМАНИЕ!!! ИНВЕРСИЯ питания для ЭЛЕКТРОМАГНИТНЫХ Реле!!! (HIGH - выключено реле, LOW - включено).
  digitalWrite(REJ1, HIGH); // режим 1 Off
  digitalWrite(REJ2, HIGH); // режим 2 Off
  digitalWrite(REJ3, HIGH); // режим 3 Off

  digitalWrite(GREAN, LOW); // Зелёный цвет Off
  digitalWrite(BLUE, LOW);  // Синий  цвет  Off
  digitalWrite(RED, LOW);   // Красный цвет Off

  /* После подключения питания или перезагрузки, читаем значение из последней сохранённой ячейки ОЗУ
    умножаем это значение на 118 и записываем его в переменную L
    делаем это в void setup то бишь только один раз в момент подключения питания*/
  bool init = true;   //создаём булево значение и делаем её ИСТИНА
  for (int i = 0; i < EEPROM_SIZE; i++)   //запускаем счётчик, если i < объёма EEPROM, тогда +1
  {
    if (EEPROM.read(i) == EEPROM_MARKER)   //считаем переменную i и сравниваем с Маркером, если ровна...
    {
      addr = (i - 1 + EEPROM_SIZE) % EEPROM_SIZE;  //отнимаем от i единицу и прибавляем EEPROM_SIZE(512) и остаток деления от EEPROM_SIZE
      init = false;   // меняем значение на ЛОЖ
      break;          // принудительно выходим из цикла for
    }
  }
  if (init) L = 0;    // если init ИСТИНА, присваиваем L ноль
  else  L = EEPROM.read(addr) * 118;  // иначе, читаем addr и умножаем значение из памяти на 118
}


void AutoServoPovoroti()  //Функция Автоповоротов Servo
{
  cPos = ServoPovoroti.read();   //считываем текущую позицию с Servo и записываем в cPos
  if (cPos == 12) zPos = 130;     //если позиция равно левому положению, тогда нужной позиции присваиваем заданный угол
  if (cPos == 130) zPos = 12;     //декрементируем в обратную сторону
  if (cPos != zPos && millis() >= lastUpdate) {  //если текущая позиция неравна нужной позиции и время больше или равно lastUpdate, тогда...
    if (cPos < zPos) ServoPovoroti.write(cPos + 1);    // перемещаем Servo вправо на +1
    if (cPos > zPos) ServoPovoroti.write(cPos - 1);    // перемещаем Servo влево  на -1
    lastUpdate = millis() + tDelay;        //таймер + задержка
  }
}


void Povorot()//Функция Ручных поворотов Servo при зажатии кнопки
{  
  /* Делаем, что бы система работала в режиме удержания кнопки для Серво.
    После приёма определённого кода кнопки,сразу перехватывать идущий за ним (кодом), (пустой инфракрасный сигнал)
    и считать его - (пустой инфракрасный сигнал), за код кнопки.
    После пропадания (пустого инфракрасного сигнала) - будем считать, что код перестал слаться.*/

  /* 1. После приёма определённого кода кнопки, перехватывать (пустой инфракрасный сигнал).
     2. Конвертировать пульсирующий (-) с (ИК) преемника сигнал в постоянный.
    После появления и пропадания сигнала, состояние сразу не меняется! а ждет  100 миллисекунд.
    Если 100 миллисекунд не прошло, и поступил еще один ИК сигнал с пульта,
    тогда таймер заново запускается на 100 миллисекунд.*/

  if (digitalRead(RECV_PIN) == LOW)
    lastUpdate = millis();
  if (millis() - lastUpdate > 100) (Right_Pin_1 = false), (Left_Pin_1 = false);

  /* Если Right_Pin_1) ==, выставляем ограничение для переменной(L) до (&& L<20000) и начинаем увеличивать
    значения переменной(L), на значение переменной increment,
    и также выставляем ограничение при убывании (&& L>0).*/
  if (Right_Pin_1 ==  true && L < 20000  ) L = L + increment;
  if (Left_Pin_1  ==  true && L > 0  ) L = L - increment;

  //читаем значение (L) и конвертируем его в значение pos, то бишь в значение поворота в градусов доступных для servo.
  pos = L;
  pos = map(pos, 0, 20000, 12, 130);       // Задаём диапазон скорости и поворота Серво (углы поворота от 12 до 130 градусов)
  ServoPovoroti.write(pos);               // Положение Серво в соответствии с pos.

  /* эта конструкция отвечает за запись положения градусов Servo в ОЗУ EEPROM.
    Можно было бы сделать так if(Right_Pin_1 == HIGH || Left_Pin_1 == HIGH)EEPROM.update(addr, L/118);.
    В документации на ATMEGA168 указано, количество записи и чтения в ячейку ОЗУ гарантировано не более 100.000 раз,
    поэтому будем экономить ОЗУ, то бишь записывать не постоянно в (момент движения) изменения градусов Servo,
    а только тогда, когда Servo остановилась.
    Когда кнопка №1 на ИК пульте нажата if(Right_Pin_1 == HIGH || Left_Pin_1 == HIGH)
    фиксируем это событие в переменную E_1 и записываем в неё =1; (E_1=1;)
    когда кнопку №1 отпустили, то бишь Servo остановилась //(Right_Pin_1 == LOW && Left_Pin_1 == LOW//
    и && E_1==1 то бишь перед тем как Servo остановилась она была включена, записываем в ячейку № addr EEPROM значение переменной L.
    Так как ячейка EEPROM может хранить значения от (0 до 255), а диапазон значений переменной L от (1 до 30.000),
    то перед записью в ячейку № addr, делим значение переменной L на 118  //(addr, L/118)//
    и меняем значение переменной E_1 на 0 // ,E_1=0;//  */

  if (Right_Pin_1 ==  true || Left_Pin_1 ==  true) E_1 = 1;
  if (Right_Pin_1 == false &&  Left_Pin_1 == false && E_1 == 1) {
    int maddr = (addr + 1) % EEPROM_SIZE; //записываем в переменную maddr(МаркерАдрес) прибавленый addr с остатком деления от размера EEPROM(512)
    EEPROM.write(maddr, EEPROM_MARKER);   //Записываем в EEPROM значения из Маркера
    EEPROM.update(addr, L / 118);         //записываем обновлённые значения в EEPROM в ячейку addr
    addr++;                     //переходим на следующую ячейку для записи
    if (addr >= EEPROM_SIZE)    //если addr >= размеру EEPROM
    {
      addr = 0;                 //переходим обратно в первую ячейку и по кругу...
    }
    E_1 = 0;    //меняем значение переменной E_1 на 0
  }
}


void off()         // Функция выключения всех режимов и светодиодов + отсчёт времени
{
  // выключаем все реле
  digitalWrite(REJ1, HIGH); // режим 1 Off
  digitalWrite(REJ2, HIGH); // режим 2 Off
  digitalWrite(REJ3, HIGH); // режим 3 Off
  // выключаем все светодиоды
  digitalWrite(GREAN, LOW); // Зелёный цвет Off
  digitalWrite(BLUE, LOW);  // Синий  цвет  Off
  digitalWrite(RED, LOW);   // Красный цвет Off
  millis_old[0] = millis();
  millis_old[1] = millis();
  millis_old[2] = millis();
  millis_old[3] = millis();
  millis_old[4] = millis();
  millis_old[5] = millis();
  millis_old[6] = millis();
  millis_old[7] = millis();
  millis_old[8] = millis();
}


void NoTimer()     // Функция действия вентилятора БЕЗ ТАЙМЕРА
{
  // действия БЕЗ ТАЙМЕРА:
  if (a == 1) {   // Если "a" равер 1, то...
    //выключаем все остальные режимы и светодиоды
    digitalWrite(REJ2, HIGH);   // режим 2 Off
    digitalWrite(BLUE, LOW);    // Синий цвет Off
    digitalWrite(REJ3, HIGH);   // режим 3 Off
    digitalWrite(RED, LOW);     // Красный цвет Off
    // включаем 1 режим
    digitalWrite(REJ1, LOW);    // режим 1 On
    digitalWrite(GREAN, HIGH);  // Зелёный цвет On
  } else {      // Иначе...
    // выключаем 1 режим
    digitalWrite(REJ1, HIGH);   // режим 1 Off
    digitalWrite(GREAN, LOW);   // Зелёный цвет Off
    a = 0; //возвращаем переменную "a" в исходное состояние
  }

  if (b == 1) {   // Если "b" равер 1, то...
    //выключаем все остальные режимы и светодиоды
    digitalWrite(REJ1, HIGH);   // режим 1 Off
    digitalWrite(GREAN, LOW);   // Зелёный цвет Off
    digitalWrite(REJ3, HIGH);   // режим 3 Off
    digitalWrite(RED, LOW);     // Красный цвет Off
    // включаем 2 режим
    digitalWrite(REJ2, LOW);    // режим 2 On
    digitalWrite(BLUE, HIGH);   // Синий цвет On
  } else {      // Иначе...
    // выключаем 2 режим
    digitalWrite(REJ2, HIGH);   // режим 2 Off
    digitalWrite(BLUE, LOW);    // Синий цвет Off
    b = 0; //возвращаем переменную "b" в исходное состояние
  }

  if (c == 1) {   // Если "c" равер 1, то...
    //выключаем все остальные режимы и светодиоды
    digitalWrite(REJ1, HIGH);   // режим 1 Off
    digitalWrite(GREAN, LOW);   // Зелёный цвет Off
    digitalWrite(REJ2, HIGH);   // режим 2 Off
    digitalWrite(BLUE, LOW);    // Синий цвет Off
    // включаем 3 режим
    digitalWrite(REJ3, LOW);    // режим 3 On
    digitalWrite(RED, HIGH);    // Красный цвет On
  } else {      // Иначе...
    // выключаем 3 режим
    digitalWrite(REJ3, HIGH);   // режим 3 Off
    digitalWrite(RED, LOW);     // Красный цвет Off
    c = 0; //возвращаем переменную "c" в исходное состояние
  }

  /////////////////////////////////////////////////////////////////////////


  // Внопка Выключения любого режима
  if (d == 1) {   // Если "d" равер 1, то - ВЫКЛЮЧАЕМ ВСЁ!!!
    off();
    f_e = 0;
    f_f = 0;
    f_g = 0;
    f_h = 0;
    f_i = 0;
    f_j = 0;
    f_k = 0;
    f_l = 0;
    f_m = 0;

    a = 0;
    b = 0;
    c = 0;
    e = 0;
    f = 0;
    g = 0;
    h = 0;
    i = 0;
    j = 0;
    k = 0;
    l = 0;
    m = 0;
    d = 0;  //возвращаем переменную "d" в исходное состояние
  }
}


void YesTimer()    // Функция действия вентилятора ПО ТАЙМЕРУ
{
  // ТАЙМЕРЫ ДЛЯ РЕЖИМА 1
  if (e == 1) {       // Если "e" равер 1, то...
    if (f_e == 0)  //Выполняется 1 раз при e=1
    {
      off();  //Отключаем ВСЕ
      //Отключаем остальные режимы и готовим их на случай Включеня
      f_e = 1;
      f_f = 0;
      f_g = 0;
      f_h = 0;
      f_i = 0;
      f_j = 0;
      f_k = 0;
      f_l = 0;
      f_m = 0;

      a = 0;
      b = 0;
      c = 0;
      e = 1;
      f = 0;
      g = 0;
      h = 0;
      i = 0;
      j = 0;
      k = 0;
      l = 0;
      m = 0;
    }

    // включаем 1 режим на 15 минут
    digitalWrite(REJ1, LOW);    // режим 1 On
    digitalWrite(GREAN, HIGH);  // Зелёный цвет On

    if (my_timer(900000, 0)) {    // Имя_Таймера(Время_В_Миллисекундах, Номер_Таймера)
      off();    //Отключаем ВСЕ
      e = 0;  //возвращаем переменную "e" в исходное состояние
      Auto_Povorot  = false; //отключаем Автоповороты, если были включены.
    }
  }


  if (f == 1) {       // Если "f" равер 1, то...
    if (f_f == 0)  //Выполняется 1 раз при f=1
    {
      off();  //Отключаем ВСЕ
      //Отключаем остальные режимы и готовим их на случай Включеня
      f_e = 0;
      f_f = 1;
      f_g = 0;
      f_h = 0;
      f_i = 0;
      f_j = 0;
      f_k = 0;
      f_l = 0;
      f_m = 0;

      a = 0;
      b = 0;
      c = 0;
      e = 0;
      f = 1;
      g = 0;
      h = 0;
      i = 0;
      j = 0;
      k = 0;
      l = 0;
      m = 0;
    }

    // включаем 1 режим на 30 минут
    digitalWrite(REJ1, LOW);    // режим 1 On
    digitalWrite(GREAN, HIGH);  // Зелёный цвет On

    if (my_timer(1800000, 1)) {    // Имя_Таймера(Время_В_Миллисекундах, Номер_Таймера)
      off();    //Отключаем ВСЕ
      f = 0;  //возвращаем переменную "f" в исходное состояние
      Auto_Povorot  = false; //отключаем Автоповороты, если были включены.
    }
  }


  if (g == 1) {       // Если "g" равер 1, то...
    if (f_g == 0)  //Выполняется 1 раз при g=1
    {
      off();  //Отключаем ВСЕ
      //Отключаем остальные режимы и готовим их на случай Включеня
      f_e = 0;
      f_f = 0;
      f_g = 1;
      f_h = 0;
      f_i = 0;
      f_j = 0;
      f_k = 0;
      f_l = 0;
      f_m = 0;

      a = 0;
      b = 0;
      c = 0;
      e = 0;
      f = 0;
      g = 1;
      h = 0;
      i = 0;
      j = 0;
      k = 0;
      l = 0;
      m = 0;
    }

    // включаем 1 режим на 45 минут
    digitalWrite(REJ1, LOW);    // режим 3 On
    digitalWrite(GREAN, HIGH);  // Зелёный цвет On

    if (my_timer(2700000, 2)) {    // Имя_Таймера(Время_В_Миллисекундах, Номер_Таймера)
      off();    //Отключаем ВСЕ
      g = 0;  //возвращаем переменную "g" в исходное состояние
      Auto_Povorot  = false; //отключаем Автоповороты, если были включены.
    }
  }


  /////////////////////////////////////////////////////////////////////////


  // ТАЙМЕРЫ ДЛЯ РЕЖИМА 2
  if (h == 1) {       // Если "h" равер 1, то...
    if (f_h == 0)  //Выполняется 1 раз при h=1
    {
      off();  //Отключаем ВСЕ
      //Отключаем остальные режимы и готовим их на случай Включеня
      f_e = 0;
      f_f = 0;
      f_g = 0;
      f_h = 1;
      f_i = 0;
      f_j = 0;
      f_k = 0;
      f_l = 0;
      f_m = 0;

      a = 0;
      b = 0;
      c = 0;
      e = 0;
      f = 0;
      g = 0;
      h = 1;
      i = 0;
      j = 0;
      k = 0;
      l = 0;
      m = 0;
    }

    // включаем 2 режим на 15 минут
    digitalWrite(REJ2, LOW);    // режим 2 On
    digitalWrite(BLUE, HIGH);   // Синий цвет On

    if (my_timer(900000, 3)) {    // Имя_Таймера(Время_В_Миллисекундах, Номер_Таймера)
      off();    //Отключаем ВСЕ
      h = 0;  //возвращаем переменную "h" в исходное состояние
      Auto_Povorot  = false; //отключаем Автоповороты, если были включены.
    }
  }


  if (i == 1) {       // Если "i" равер 1, то...
    if (f_i == 0)  //Выполняется 1 раз при i=1
    {
      off();  //Отключаем ВСЕ
      //Отключаем остальные режимы и готовим их на случай Включеня
      f_e = 0;
      f_f = 0;
      f_g = 0;
      f_h = 0;
      f_i = 1;
      f_j = 0;
      f_k = 0;
      f_l = 0;
      f_m = 0;

      a = 0;
      b = 0;
      c = 0;
      e = 0;
      f = 0;
      g = 0;
      h = 0;
      i = 1;
      j = 0;
      k = 0;
      l = 0;
      m = 0;
    }

    // включаем 2 режим на 30 минут
    digitalWrite(REJ2, LOW);    // режим 2 On
    digitalWrite(BLUE, HIGH);   // Синий цвет On

    if (my_timer(1800000, 4)) {    // Имя_Таймера(Время_В_Миллисекундах, Номер_Таймера)
      off();    //Отключаем ВСЕ
      i = 0;  //возвращаем переменную "i" в исходное состояние
      Auto_Povorot  = false; //отключаем Автоповороты, если были включены.
    }
  }


  if (j == 1) {       // Если "j" равер 1, то...
    if (f_j == 0)  //Выполняется 1 раз при j=1
    {
      off();  //Отключаем ВСЕ
      //Отключаем остальные режимы и готовим их на случай Включеня
      f_e = 0;
      f_f = 0;
      f_g = 0;
      f_h = 0;
      f_i = 0;
      f_j = 1;
      f_k = 0;
      f_l = 0;
      f_m = 0;

      a = 0;
      b = 0;
      c = 0;
      e = 0;
      f = 0;
      g = 0;
      h = 0;
      i = 0;
      j = 1;
      k = 0;
      l = 0;
      m = 0;
    }

    // включаем 2 режим на 45 минут
    digitalWrite(REJ2, LOW);    // режим 2 On
    digitalWrite(BLUE, HIGH);   // Синий цвет On

    if (my_timer(2700000, 5)) {    // Имя_Таймера(Время_В_Миллисекундах, Номер_Таймера)
      off();    //Отключаем ВСЕ
      j = 0;  //возвращаем переменную "j" в исходное состояние
      Auto_Povorot  = false; //отключаем Автоповороты, если были включены.
    }
  }


  /////////////////////////////////////////////////////////////////////////


  // ТАЙМЕРЫ ДЛЯ РЕЖИМА 3
  if (k == 1) {       // Если "k" равер 1, то...
    if (f_k == 0)  //Выполняется 1 раз при k=1
    {
      off();  //Отключаем ВСЕ
      //Отключаем остальные режимы и готовим их на случай Включеня
      f_e = 0;
      f_f = 0;
      f_g = 0;
      f_h = 0;
      f_i = 0;
      f_j = 0;
      f_k = 1;
      f_l = 0;
      f_m = 0;

      a = 0;
      b = 0;
      c = 0;
      e = 0;
      f = 0;
      g = 0;
      h = 0;
      i = 0;
      j = 0;
      k = 1;
      l = 0;
      m = 0;
    }

    // включаем 3 режим на 15 минут
    digitalWrite(REJ3, LOW);    // режим 3 On
    digitalWrite(RED, HIGH);    // Красный цвет On

    if (my_timer(900000, 6)) {    // Имя_Таймера(Время_В_Миллисекундах, Номер_Таймера)
      off();    //Отключаем ВСЕ
      k = 0;  //возвращаем переменную "k" в исходное состояние
      Auto_Povorot  = false; //отключаем Автоповороты, если были включены.
    }
  }


  if (l == 1) {       // Если "l" равер 1, то...
    if (f_l == 0)  //Выполняется 1 раз при l=1
    {
      off();  //Отключаем ВСЕ
      //Отключаем остальные режимы и готовим их на случай Включеня
      f_e = 0;
      f_f = 0;
      f_g = 0;
      f_h = 0;
      f_i = 0;
      f_j = 0;
      f_k = 0;
      f_l = 1;
      f_m = 0;

      a = 0;
      b = 0;
      c = 0;
      e = 0;
      f = 0;
      g = 0;
      h = 0;
      i = 0;
      j = 0;
      k = 0;
      l = 1;
      m = 0;
    }

    // включаем 3 режим на 30 минут
    digitalWrite(REJ3, LOW);    // режим 3 On
    digitalWrite(RED, HIGH);    // Красный цвет On

    if (my_timer(1800000, 7)) {    // Имя_Таймера(Время_В_Миллисекундах, Номер_Таймера)
      off();    //Отключаем ВСЕ
      l = 0;  //возвращаем переменную "l" в исходное состояние
      Auto_Povorot  = false; //отключаем Автоповороты, если были включены.
    }
  }


  if (m == 1) {       // Если "m" равер 1, то...
    if (f_m == 0)  //Выполняется 1 раз при m=1
    {
      off();  //Отключаем ВСЕ
      //Отключаем остальные режимы и готовим их на случай Включеня
      f_e = 0;
      f_f = 0;
      f_g = 0;
      f_h = 0;
      f_i = 0;
      f_j = 0;
      f_k = 0;
      f_l = 0;
      f_m = 1;

      a = 0;
      b = 0;
      c = 0;
      e = 0;
      f = 0;
      g = 0;
      h = 0;
      i = 0;
      j = 0;
      k = 0;
      l = 0;
      m = 1;
    }

    // включаем 3 режим на 45 минут
    digitalWrite(REJ3, LOW);    // режим 3 On
    digitalWrite(RED, HIGH);    // Красный цвет On

    if (my_timer(2700000, 8)) {    // Имя_Таймера(Время_В_Миллисекундах, Номер_Таймера)
      off();    //Отключаем ВСЕ
      m = 0;  //возвращаем переменную "m" в исходное состояние
      Auto_Povorot  = false; //отключаем Автоповороты, если были включены.
    }
  }
}


void loop()
{
  NoTimer();    //включаем в цикл loop функцию действия вентилятора БЕЗ ТАЙМЕРА
  YesTimer();   //включаем в цикл loop функцию действия вентилятора ПО ТАЙМЕРУ

  // Код отвечающий за Вкл./Выкл. Серво через Функции
  if (Auto_Povorot == true) {   //Если Auto_Povorot истина...
    AutoServoPovoroti();  //включаем Автоповороты
  } else {  //иначе...
    Povorot();    //включены Ручные повороты при зажатии кнопки
  }


  // Получение команд с ИК пульта...
  if (irrecv.decode(&results)) {
    // 4 Команды с пульта отвечающие за ручные и Автоповороты повороты Серво:
    if (results.value == 0xA25DB24D || results.value == 0x61A0A857) Right_Pin_1  = true;   // Поворот Вправо
    if (results.value == 0xA25D8A75 || results.value == 0x61A06897) Left_Pin_1   = true;   // Поворот Влево
    if (results.value == 0xA25D01FE || results.value == 0x61A042BD) Auto_Povorot = true;   //Автоповорот вкл.
    if (results.value == 0xA25D817E || results.value == 0x61A0C23D) Auto_Povorot = false;  //Автоповорот откл.


    // далее идут коды кнопок и режимы вентилятора...
    // всегда во всех режимах включен только один режим, остальные - off
    if (results.value == 0xA25D807F || results.value == 0x61A000FF) {  // a
      a = 1;
      b = 0;
      c = 0;
      e = 0;
      f = 0;
      g = 0;
      h = 0;
      i = 0;
      j = 0;
      k = 0;
      l = 0;
      m = 0;
    }
    if (results.value == 0xA25D40BF || results.value == 0x61A0807F) {  // b
      a = 0;
      b = 1;
      c = 0;
      e = 0;
      f = 0;
      g = 0;
      h = 0;
      i = 0;
      j = 0;
      k = 0;
      l = 0;
      m = 0;
    }
    if (results.value == 0xA25DC03F || results.value == 0x61A040BF) {  // c
      a = 0;
      b = 0;
      c = 1;
      e = 0;
      f = 0;
      g = 0;
      h = 0;
      i = 0;
      j = 0;
      k = 0;
      l = 0;
      m = 0;
    }
    if (results.value == 0xA25D48B7 || results.value == 0x61A018E7) {  // Выключить всё! - d
      a = 0;
      b = 0;
      c = 0;
      d = 1;
      e = 0;
      f = 0;
      g = 0;
      h = 0;
      i = 0;
      j = 0;
      k = 0;
      l = 0;
      m = 0;
    }

    // 1 режим по таймеру
    if (results.value == 0xA25D20DF || results.value == 0x61A0C03F) {  // e - 15 мин
      a = 0;
      b = 0;
      c = 0;
      e = 1;
      f = 0;
      g = 0;
      h = 0;
      i = 0;
      j = 0;
      k = 0;
      l = 0;
      m = 0;
    }
    if (results.value == 0xA25DE01F || results.value == 0x61A0609F) {  // f - 30 мин
      a = 0;
      b = 0;
      c = 0;
      e = 0;
      f = 1;
      g = 0;
      h = 0;
      i = 0;
      j = 0;
      k = 0;
      l = 0;
      m = 0;
    }
    if (results.value == 0xA25DA45B || results.value == 0x20DF52AD) {  // g - 45 мин
      a = 0;
      b = 0;
      c = 0;
      e = 0;
      f = 0;
      g = 1;
      h = 0;
      i = 0;
      j = 0;
      k = 0;
      l = 0;
      m = 0;
    }

    // 2 режим по таймеру
    if (results.value == 0xA25DA05F || results.value == 0x61A020DF) {  // h - 15 мин
      a = 0;
      b = 0;
      c = 0;
      e = 0;
      f = 0;
      g = 0;
      h = 1;
      i = 0;
      j = 0;
      k = 0;
      l = 0;
      m = 0;
    }
    if (results.value == 0xA25D10EF || results.value == 0x61A0E01F) {  // i - 30 мин
      a = 0;
      b = 0;
      c = 0;
      e = 0;
      f = 0;
      g = 0;
      h = 0;
      i = 1;
      j = 0;
      k = 0;
      l = 0;
      m = 0;
    }
    if (results.value == 0xA25D50AF || results.value == 0x61A0906F) {  // j - 45 мин
      a = 0;
      b = 0;
      c = 0;
      e = 0;
      f = 0;
      g = 0;
      h = 0;
      i = 0;
      j = 1;
      k = 0;
      l = 0;
      m = 0;
    }

    // 3 режим по таймеру
    if (results.value == 0xA25D609F || results.value == 0x61A0A05F) {  // k - 15 мин
      a = 0;
      b = 0;
      c = 0;
      e = 0;
      f = 0;
      g = 0;
      h = 0;
      i = 0;
      j = 0;
      k = 1;
      l = 0;
      m = 0;
    }
    if (results.value == 0xA25D906F || results.value == 0x61A010EF) {  // l - 30 мин
      a = 0;
      b = 0;
      c = 0;
      e = 0;
      f = 0;
      g = 0;
      h = 0;
      i = 0;
      j = 0;
      k = 0;
      l = 1;
      m = 0;
    }
    if (results.value == 0xA25D649B || results.value == 0x61A0B847) {  // m - 45 мин
      a = 0;
      b = 0;
      c = 0;
      e = 0;
      f = 0;
      g = 0;
      h = 0;
      i = 0;
      j = 0;
      k = 0;
      l = 0;
      m = 1;
    }

    irrecv.resume(); // получаем следующие значения...
  }
}

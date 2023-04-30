//Функция для Автоповоротов Servo
void auto_Rotation() {
  if (millis() - tmr_auto >= AUTO_DELAY_TIME_SERVO) {    // Если время больше или равно "AUTO_DELAY_TIME_SERVO", тогда...
    tmr_auto = millis();       // записываем время в "tmr_auto"
    servo_val += dir;           // Прибавляем "servo_val" значение градуса "dir"
    if (servo_val >= RIGHT_CORNER || servo_val <= LEFT_CORNER) dir = -dir;    // Разворачиваем в обратную сторону

    servo_val = constrain(servo_val, LEFT_CORNER, RIGHT_CORNER);    // Ограничим диапазон для безопасности от "LEFT_CORNER" до "RIGHT_CORNER"
    rotation.write(servo_val);    // Записываем угол в Servo
  }
}


void manual_Turns() {
  if (Right_Turns ==  true) servo_val += counter;   // Если флаг для правых поворотов поднят, то прибавляем к счётчику
  if (Left_Turns  ==  true) servo_val -= counter;   // Если флаг для левых поворотов поднят, то отнимаем от счётчика

  Right_Turns = false;    // Опускаем флаги для ручных поворотов
  Left_Turns = false;

  servo_val = constrain(servo_val, LEFT_CORNER, RIGHT_CORNER);    // Ограничим диапазон для безопасности от "LEFT_CORNER" до "RIGHT_CORNER"
  rotation.write(servo_val);    // Записываем угол в Servo
}

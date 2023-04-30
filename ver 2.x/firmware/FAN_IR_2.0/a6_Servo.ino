//Функция для Автоповоротов Servo
void auto_Rotation() {
  if (millis() - tmr_auto >= AUTO_DELAY_TIME_SERVO) {                       // Если время больше или равно "AUTO_DELAY_TIME_SERVO", тогда...
    servo_val += dir;                                                       // Прибавляем "servo_val" значение градуса "dir"
    if (servo_val >= RIGHT_CORNER || servo_val <= LEFT_CORNER) dir = -dir;  // Разворачиваем Servo в обратную сторону

    servo_val = constrain(servo_val, LEFT_CORNER, RIGHT_CORNER);  // Ограничим диапазон для безопасности от "LEFT_CORNER" до "RIGHT_CORNER"
    rotation.write(servo_val);                                    // Записываем угол в Servo

    tmr_auto = millis();  // Записываем время в "tmr_auto"
  }
}



void manual_Turns() {
  if (Right_Turns == true) servo_val += counter;  // Если флаг для правых поворотов поднят, то увеличиваем счётчик
  if (Left_Turns == true) servo_val -= counter;   // Если флаг для левых поворотов поднят, то уменьшаем счётчик

  //Опускаем флаги для ручных поворотов
  Right_Turns = false;
  Left_Turns = false;

  servo_val = constrain(servo_val, LEFT_CORNER, RIGHT_CORNER);  // Ограничим диапазон для безопасности от "LEFT_CORNER" до "RIGHT_CORNER"
  rotation.write(servo_val);                                    // Записываем угол в Servo
}

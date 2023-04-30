/*
 * IRremote: IRrecvDemo - демонстрирует получение ИК-кодов с помощью IRrecv
 * ИК-детектор должен быть подключен к входу RECV_PIN.
 * Версия: 0.1 Июль 2009 года.
 * Автор: Ken Shirriff
 * http://arcfn.com
 */


#include <IRremote.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
}

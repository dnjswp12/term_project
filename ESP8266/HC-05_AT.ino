#include <SoftwareSerial.h>
SoftwareSerial espserial(D7,D8);
void setup() {
  Serial.begin(9600);
  espserial.begin(9600);

}

void loop() {
  if(espserial.available())
  {
    Serial.write(espserial.read());
  }
  if(Serial.available())
  {
    espserial.write(Serial.read());
  }
}

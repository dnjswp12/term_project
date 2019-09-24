#include <SoftwareSerial.h>

SoftwareSerial espSerial(13,15); //RX2 TX2
void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  // put your setup code here, to run once:
}

void loop() {
  char a = 0;
  if(espSerial.available()){
    Serial.println("OK");
    a = espSerial.read();
    Serial.println(a);
    } 
}

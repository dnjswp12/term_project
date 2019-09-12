
const int cds = 13; 
int cdsVal;  
 
void setup() {
        pinMode(2,OUTPUT);
        Serial.begin(9600);
        ledcSetup(2, 5000, 8); 
        ledcAttachPin(2,2);
}
 
void loop() {
         
        Serial.println(analogRead(cds)); 
        delay(15);
        
        cdsVal = map(analogRead(cds),1300,2500,0,255); 
        // map(변환 할 값, 현재범위의 하한, 동 범위의 상한, 목표의 하한, 목표의 상한)
    
        cdsVal = constrain(cdsVal,0,255); 
  
        ledcWrite(2,cdsVal); 
        delay(10);  
}

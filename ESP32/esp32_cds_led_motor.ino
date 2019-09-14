 
const int cds = 13; 
//const int led = 2;  
int cdsVal;  
 
//Motor_PWM setting
int freq_m = 500;//2ms(500)
int motorChannel = 1;
int resolution_m = 10; 

void setup() {
        pinMode(2,OUTPUT);
        pinMode (18, OUTPUT); //DIR
        pinMode (4,OUTPUT); // EN
        Serial.begin(9600);
        //ledcSetup(2, 5000, 8); 
        //ledcAttachPin(2,2);
        // motor pwm
        ledcSetup(motorChannel,freq_m,resolution_m);
        ledcAttachPin(19,motorChannel);
}
void Motor_up()
{
  Serial.println("up");
  digitalWrite(18,LOW);
  ledcWrite(motorChannel,500);
  digitalWrite(4,LOW);
  }
void Motor_down()
{
  Serial.println("down");
  digitalWrite(18,HIGH);
  ledcWrite(motorChannel,500);
  digitalWrite(4,LOW);
  }
void Motor_stop()
{
  Serial.println("stop");
  digitalWrite(18,LOW);
  delay(1);
  digitalWrite(18,HIGH);
  delay(1);
  digitalWrite(4,HIGH); // EN
  }

 
void loop() {
        
        Serial.println(analogRead(cds)); 
        delay(100);
        //조도 센서로 측정된 센서값의 범위를 0~255까지의 범위로 선형 변환합니다.
        cdsVal = map(analogRead(cds),1300,2500,0,300); 
        // map(변환 할 값, 현재범위의 하한, 동 범위의 상한, 목표의 하한, 목표의 상한)
        
        cdsVal = constrain(cdsVal,0,300);
        Serial.println(cdsVal); 
        delay(100);
        if (cdsVal < 150) // 어두울때
        {
          Motor_down();
          digitalWrite(2,HIGH);
          } 
          else if(cdsVal > 250) // 밝을때 
          {
            Motor_up();
            digitalWrite(2,LOW);
            }
            else
            {
              Motor_stop();
              }
        
        //ledcWrite(2,cdsVal); 
        delay(10);  
}

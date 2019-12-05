/*****
 
 All the resources for this project:
 https://randomnerdtutorials.com/
 
*****/

// Loading the ESP8266WiFi library and the PubSubClient library
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(13, 15); // RX2(D7) TX2(D8)
// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "KIN";
const char* password = "41126469";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "114.205.87.39";

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);
//int timeIn=0;
// Connect an LED to each GPIO of your ESP8266
//const int ledGPIO5 = 5;
// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  //시간을 받아 저장 


  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic home/office/esp1/gpio2,
  // you check if the message is either 1 or 0. Turns the ESP GPIO according to the message
  // 우리가 실제적으로 건드려야할 곳 
  
    }

  


// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
// 8266이 MQTT broker에 재 접속하게 된다.
// 8266에서 더 많은 토픽을 구독하고 싶을때 아래 기능을 바꾸면 된다.
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
     
    if (client.connect("ESP8266Client2")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      // 더 많은 토픽을 구독 할 수 있다.
      // 토픽을 바꾸면 여기도 바꿔줘야
      //Serial.print("띠용");
      //client.subscribe("esp8266/function/LED");
      client.publish("esp8266/text","Welcome Smart Home");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
String send_data()
{
  //long lastmsg = 0;
 // long now = millis();
  // millis()는 아두이노로 프로그램을 돌리기 시작한 후 지난 시간을 밀리 초 숫자로 return, 
  // 50일 후에 오버플로우 되어 초기화
  char temp[]={0}; // 50글자를 받아오는 배열 한글은 한글자에 2바이트 20글자, 영어는 널문자 포함 40글자
 String uartmsg;
 //unsigned int length;
  /*for (int a=0;a<20;a++ ){
    temp[a] = 0; 
    } // 버퍼 초기화*/
    if(mySerial.available()){
      byte leng = mySerial.readBytes(temp,1);
      Serial.print("128 sent me: ");
     
 for (int i = 0; i < leng; i++) {
    Serial.print(temp[i]);
    uartmsg += temp[i];
    Serial.println(uartmsg);
  }
  }
  return uartmsg;
}
char spread(String c)
{
  if(c == "b")
      {client.publish("esp8266/led","BRIGHT");}
      else if(c == "d")
      {client.publish("esp8266/led","DARK");}
      else if(c == "u")
      {client.publish("esp8266/Motor","UP");}
      else if(c == "m")
      {client.publish("esp8266/Motor","DOWN");}
      else if(c == "s")
      {client.publish("esp8266/Motor","STOP");}
      else if(c == "a" )
      {client.publish("esp8266/auto","AUTO");}
      else if(c == "c" )
      {client.publish("esp8266/auto","STOP");}
      else if(c == "o" )
      {client.publish("esp8266/LAMP","ON");}
      else if(c == "f" )
      {client.publish("esp8266/LAMP","OFF");}
}
void setup() {
  //pinMode(ledGPIO5, OUTPUT);
  Serial.begin(9600);
  setup_wifi();
  mySerial.begin(9600);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// For this project, you don't need to change anything in the loop function. 
// Basically it ensures that you ESP is connected to your broker
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop()){
     
    client.connect("ESP8266Client2");}
    String test=send_data();
    spread(test);
  }    
  

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
 
MDNSResponder mdns;
 
ESP8266WebServer server(80);
String webPage;
const char* ssid     = "*******";      //wifi name
const char* password = "*******";  //wifi password
 
void setup() {
 
  pinMode(4, OUTPUT);  //led pin 16
 
  webPage += "<h1>HOME AutoMation</h1><p>LED ";
  webPage += "<a href=\"socket1On\"><button>ON</button></a>&nbsp;";
  webPage += "<a href=\"socket1Off\"><button>OFF</button></a></p>";
  webPage += "<p>bright<a href=\"socket2On\"><button>ON</button></a>&nbsp;";
  webPage += "<a href=\"socket2Off\"><button>OFF</button></a></p>";
  
  Serial.begin(9600);
  delay(100);
 
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) 
    Serial.println("MDNS responder started");
 
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  server.on("/socket1On", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(4, HIGH);
    delay(10); //Turn On LED
    
    //int i=9;
    //Serial.write(i); // Using UART
  });
  server.on("/socket1Off", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(4, LOW);
    delay(10); //Turn off LED
    
    //int a=0;
    //Serial.write(a); //Using UART
  });

  server.begin();
  Serial.println("HTTP server started");
}
 
void loop() {
  server.handleClient();
}

#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>


// #### LED Pin Setup #### //
int GREEN = D5; 
int YELLOW = D7;
int RED = D6;

// ### Sensor Variables sent ### //
String LED_pwr = "On";
String LED_color = "Green";
String LED_final = "Yellow";

// #### MQTT Server connection Setup - Raspberry Pi Broker #### //
int mqtt_server = "192.168.230.116";
int mqtt_port = 1883;  

WiFiClient Wifi;            //Setup Wifi object 
PubSubClient client(Wifi);  //Object that gives you all the MQTT functionality, access objects in PubSubClient Library

// ##### Wifi Connection Setup #### //
char WifiName[] = "Verizon-SM-G935V";            //SSID
char Password[] = "password";

// #### LED Functions Setup #### //
void TurnGREEN(){
  //TurnOFF();
  AllClear();
  digitalWrite(GREEN,HIGH);
  //digitalWrite(YELLOW,LOW);
  //digitalWrite(RED,LOW);
  Serial.println("GREEN");
  
}

void Flash(){
  digitalWrite(GREEN,HIGH);
  delay (200);
  digitalWrite(GREEN,LOW);
  delay (200);
  digitalWrite(GREEN,HIGH);
  delay (200);
  digitalWrite(GREEN,LOW);
  delay (200);
  digitalWrite(GREEN,HIGH);
  delay (200);
  digitalWrite(GREEN,LOW);
  delay (200);
}

void TurnYELLOW(){
  //TurnOFF();
  AllClear();
  digitalWrite(YELLOW,HIGH);
  Serial.println("YELLOW");
}

void TurnRED(){
  AllClear();
  digitalWrite(RED,HIGH);
  Serial.println("RED");
}

void AllClear(){
  digitalWrite(GREEN,LOW);
  digitalWrite(YELLOW,LOW);
  digitalWrite(RED,LOW);
}

void Msg_rcv(char* topic, byte* payload, unsigned int length){     //Unsigned int = Positive numbers (more range)
  if ((char) payload[0] == 'o'){
    if ((char) payload[1] == 'n'){
      LED_pwr = "On";
    }
    else{
      LED_pwr = "Off";
    }
  }
  else if ((char) payload[0] == 'g'){
    LED_color = "Green";
  }
  else if ((char) payload[0] == 'y'){
    LED_color = "Yellow";
  }
  else if ((char) payload[0] == 'r'){
    LED_color = "Red";
  }
}

void Turn_color(){
  if (LED_pwr = "On"){
    LED_final = LED_color;
    Serial.print (LED_color);
    Serial.println (" is on");
    }
   else{
    LED_final = "None";
    Serial.println ("Power is off");  
   }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(GREEN,OUTPUT);
  pinMode(YELLOW,OUTPUT);
  pinMode(RED,OUTPUT);
  Serial.begin(115200);                          //Starts the Serial Monitor (Input printed on screen)
  client.setServer(mqtt_server, mqtt_port);           

  WiFi.begin(WifiName,Password);
  while (WiFi.status() !=WL_CONNECTED){          //If not connected to Wifi, delay until connected
    delay (2000);
    Serial.println("Finding a Connection...");
  }

  Serial.println("Connection Started");         //Begin Connection to Wifi
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());               //IP assigned to Server by host wifi
}

void loop() {                                // put your main code here, to run repeatedly:
  
}


/// ## SOURCES ## ///
//https://techtutorialsx.com/2016/07/17/esp8266-http-get-requests/
//https://www.instructables.com/id/WiFi-Communication-Between-Two-ESP8266-Based-MCU-T/
//https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/
//http://fritzing.org/home/
//https://www.instructables.com/id/Motion-Detector-With-Blynk-Notifications-WeMos-D1-/
//Andrew Thomas, friend, M.S. candidate

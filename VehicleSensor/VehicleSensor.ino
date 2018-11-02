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
char* LED_pwr = "On";
char* LED_color = "Green";

// #### MQTT Server connection Setup - Raspberry Pi Broker #### //
char* mqtt_server = "192.168.230.116";  
int mqtt_port = 1883;  

WiFiClient Wifi;            //Setup Wifi object 
PubSubClient client(Wifi);  //Object that gives you all the MQTT functionality, access objects in PubSubClient Library

// ##### Wifi Connection Setup #### //
char WifiName[] = "Verizon-SM-G935V";            //SSID
char Password[] = "password";

// #### LED Functions Setup #### //
void TurnGREEN(){
  AllClear();
  digitalWrite(GREEN,HIGH);
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

// ### Look through payload (Message Sent) for LED instructions ### //

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

// ### Call the functions according to the payload and LED power being on or off ### //

void Turn_color(){
  if (LED_pwr == "On"){
    if (LED_color == "Green"){
      TurnGREEN();
    }
    else if (LED_color == "Red"){
      TurnRED();
    }
    else if (LED_color == "Yellow"){
      TurnYELLOW();
    }
    Serial.print (LED_color);
    Serial.println (" is on");
    }
   else{
    AllClear();
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
  client.setCallback(Msg_rcv);                   //Send payload to function (Msg_rcv)
  
  WiFi.begin(WifiName,Password);
  while (WiFi.status() !=WL_CONNECTED){          //If not connected to Wifi, delay until connected
    delay (2000);
    Serial.println("Finding a Connection...");
  }

  Serial.println("Connection Started");         //Begin Connection to Wifi
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());               //IP assigned to Server by host wifi

  while(!client.connect("LED_board")){          //LED_board is name of Wemos/arduino connected to code. Waiting to connect to Broker.
    Serial.println("Finding a Connection...");
  }
  client.subscribe("LED");
  Serial.println("Subscribed to LED");
}

void loop() {                                // put your main code here, to run repeatedly:
  client.loop();                             // pre-made function. Connection to MQTT run continuosly (Constantly listening)
  Turn_color();
}


/// ## SOURCES ## ///
//https://techtutorialsx.com/2016/07/17/esp8266-http-get-requests/
//https://www.instructables.com/id/WiFi-Communication-Between-Two-ESP8266-Based-MCU-T/
//https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/
//http://fritzing.org/home/
//https://www.instructables.com/id/Motion-Detector-With-Blynk-Notifications-WeMos-D1-/
//Andrew Thomas, friend, M.S. candidate

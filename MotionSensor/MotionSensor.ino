#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>


const int Ping = D7; //trigger - sends sound out to measure distance
const int Receive = D5; //Echo - Receives sound from trigger to measure travel time (microsec)
long TimeTravel;   //2 bit integer
int distance;      //1 bit integer

// ##### Wifi Connection Setup #### //
char WifiName[] = "Verizon-SM-G935V";            //SSID
char Password[] = "password";

void setup() {
  // put your setup code here, to run once:
  pinMode (Ping, OUTPUT);  
  pinMode(Receive, INPUT);    
  Serial.begin(9600);       //Begin serial monitor

  // ### Begin Connection to Wifi ### //
  WiFi.begin(WifiName,Password);
  while (WiFi.status() !=WL_CONNECTED){          //If not connected to Wifi, delay until connected
    delay (2000);
    Serial.println("Finding a Connection...");
  }

  Serial.println("Connection Started");         //Begin Connection to Wifi
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());               //IP assigned to Server by host wifi
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(Ping,LOW);    //Clear
  delayMicroseconds (2);
  digitalWrite(Ping,HIGH);    //Send out sound
  delayMicroseconds (10);       
  digitalWrite(Ping,LOW);     //Clear again

  HTTPClient Caller;
  
  TimeTravel = pulseIn (Receive,HIGH);       //Get the input of Receive
  distance = (TimeTravel*0.034)/2;          //converts time to cm. If want a more precise reading, do an average of 3 readings.
  if (distance > 0){                        // in cm
    if (distance > 10){
      Serial.println("Green - Too far");
      Caller.begin("http://192.168.43.177/green");  //Server is 192.168.43.177. Setup GET request to send to this
    }
    else if (distance < 10 && distance > 5){
      Serial.println("Yellow - Getting Close");
      Caller.begin("http://192.168.43.177/yellow");
    }
    else if (distance == 5) {
      Serial.println ("Red - Perfect");
      Caller.begin("http://192.168.43.177/red");
    }
    else if (distance < 5){
      Serial.println ("Flash Green - Too close");
      Caller.begin("http://192.168.43.177/flash");
    }
  }
    Caller.GET();                         //Sends the GET request
    Caller.end();
}

/// ## SOURCES ## ///
//https://techtutorialsx.com/2016/07/17/esp8266-http-get-requests/
//https://www.instructables.com/id/WiFi-Communication-Between-Two-ESP8266-Based-MCU-T/
//https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/
//http://fritzing.org/home/
//https://www.instructables.com/id/Motion-Detector-With-Blynk-Notifications-WeMos-D1-/
//Andrew Thomas, friend, M.S. candidate

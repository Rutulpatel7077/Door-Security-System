/*
  This program makes a single web request to a web server.
  It sends an email when motion is detected.
  Connect
  ESP8266 RX to Arduino Pin 0
  ESP8266 TX to Arduino Pin 1
*/

/*
   connect vcc of motion sensor to vcc of 321maker of arduino
   connect gnd of motion sensor to gnd of 321maker of arduino
   connect out of motion sensor to pin 8 of 321maker of arduino
   connect input of motion sensor to pin 7 of 321maker of arduino
   connect buzzer to pin 5 to the 321maker of arduino
   you can change the sensitivity distance and time delay on the sensor
*/

/* I include <SoftwareSerial.h> library for this project*/

int  trigPin =8;// assign pin 8 value
int echoPin = 7;// assign pin 7 value
int Buzzer = 5; // assign buzzer at pin 5

#include <SoftwareSerial.h>
SoftwareSerial ESPSerial(1, 0); // TX,RX for ESP8266


unsigned int interval = 60000;  //Update every Minute 
long int now=0, previous=-50000; //Used to hold time variables.
#define DST_IP "maker.ifttt.com"  //Mail Server

void setup() {
Serial.begin (9600);
pinMode(trigPin, OUTPUT); // set pin 8 as output
pinMode(echoPin, INPUT); // set pin 7 as input
pinMode(Buzzer, OUTPUT);  // set pin 5 as buzzer output

   Serial.begin(9600);  // Open serial Connection to Computer
  Serial1.begin(115200);  // Open serial Connection to ESP8266 
  Serial1.println("AT+RST");  //Issue Reset Command
  Serial.println("AT+RST");
  delay(1000);

  //DEBUG LOOP- display ESP output to serial Monitor.
  while (Serial1.available()) { 
  Serial.println(Serial1.read());
  }
  Serial.println("AT+CWMODE=1"); 
  Serial1.println("AT+CWMODE=1");  //Set single client mode.
  delay(5000);

  Serial.println("AT+CIFSR");
  Serial1.println("AT+CIFSR"); //Display IP Information
  
  //DEBUG LOOP- display ESP output to serial Monitor.
  while (Serial1.available()) {  
  Serial.println(Serial1.read());
  }
  delay(1000);  // delay for 1 sec to get enough time  to the server

  Serial.println("AT+CIPMUX=0");  
  Serial1.println("AT+CIPMUX=0");  //Sets up Single connection mode.
  delay(1000);

  //DEBUG LOOP- display ESP output to serial Monitor.
  while (Serial1.available()) { 
    Serial.write(Serial1.read());
  }
    delay(1000);  // delay 1 sec
}

void loop() {
  
int duration, distance; // declare variable for distance measuring
digitalWrite(trigPin, HIGH); // throw output for measuring distance
delayMicroseconds(1000); // delay microsecond
digitalWrite(trigPin, LOW);// collect input for calculating the distance
duration = pulseIn(echoPin, HIGH);  
distance = (duration/2) / 29.1; // formula of distance


if (distance >= 30 || distance <= 0){ // if distance is greater than 30 
  Serial.println("Everything is Good and safe"); // print message Everything is good and safe
  digitalWrite(Buzzer, LOW); // at the time buzzer is low
}
else if (distance<=30) {    // if distance is less than 30
    Serial.println("Someone  is  here"); // print message Someone is here
    delay(20000); // wait for 2 min
    distance = 0; // put distance is equal to zero
    delay(10000);// wait for 1 min
    digitalWrite(trigPin, HIGH); // send signal for measuring distance
    delayMicroseconds(1000);   // wait and take enough time for signal 
    digitalWrite(trigPin, LOW);  // collect the signal
    duration = pulseIn(echoPin, HIGH); // Measure distance
    distance = (duration/2) / 29.1; // formula of distance
    delay(2000); // wait for 2 sec
    Serial.println(distance); // print distance for check
        if (!(distance >= 30 || distance <= 0)){
              Serial.println("Someone is here and doing something wrong");
              Serial.println("BOOOM");
              tone(Buzzer, 1000, 500); // play 1000 Hz tone for 500 ms
              delay(500);       // delay 0.5s
              tone(Buzzer, 1000, 500);; // play 1000Hz tone for 500ms
              delay(500);
              tone(Buzzer, 1000, 500); // play 1000 Hz tone for 500 ms
              delay(500);
              tone(Buzzer, 1000, 500); // play 1000Hz tone for 500ms
              delay(500);
              digitalWrite(Buzzer,1000); // play 1000 Hz tone for 500 ms
              delay(500);
              digitalWrite(Buzzer, 1200); // play 1000Hz tone for 500ms
              delay(500);
              digitalWrite(Buzzer, 1200);
              delay(5000);
              sendMail() ;  // send alert e-mail to the user
         }
    }
        
     else {
      Serial.println("Don't Worry  Everything is Good Now");
     }

       delay(200000); // delay 20 min for restarting system

}

void sendMail(){ 

  
             //Builds the connection string for the ESP8266
    String cmd = "AT+CIPSTART=\"TCP\",\"";
    cmd += DST_IP;
    cmd += "\",80"; // port 
    Serial1.println(cmd);  //Run the command
    Serial.println(cmd);    //Print this to the debug window
    delay(1000);

            //DEBUG LOOP- display ESP output to serial Monitor.
    while (Serial1.available()) {
    Serial.write(Serial1.read());
    } 

  
        //if (client.connect(maker.iftt.com, 80))         
    String httpcmd="GET /trigger/intruder/with/key/c3SrXOdDFpD7-BaWxXKacViS9aotP-wpOSvB5-HqlEX";  // send a request to the server to send email
    httpcmd += " HTTP/1.1\r\n";
    httpcmd += "Host: maker.ifttt.com \n";
    httpcmd += "Connection: close\r\n\r\n";
    
    Serial.print("AT+CIPSEND=");
    Serial.println(httpcmd.length());
  
    Serial1.print("AT+CIPSEND=");
    Serial1.println(httpcmd.length());
    delay(1000);

    Serial.print(">");
    Serial1.println(httpcmd);
    Serial.println(httpcmd);
    delay(3000);

                //DEBUG LOOP- display ESP output to serial Monitor.
     while (Serial1.available()) {
      Serial.write(Serial1.read());
      }
   
    Serial.println("AT+CIPCLOSE");
    Serial1.println("AT+CIPCLOSE"); //Close the Web Connection
}

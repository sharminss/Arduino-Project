#include <SPI.h>
#include <SD.h>// sd card

#include "TinyGPS++.h"
#include "SoftwareSerial.h"

/*

 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend

 * An IR LED must be connected to Arduino PWM pin 3.

 * Version 0.1 July, 2009


 * Copyright 2009 Ken Shirriff

 * http://arcfn.com

 * 

 * 2 ta ultra and sender

 * com4

 */





#include <IRremote.h>// ir rcvr trans for side crack

#define trigPin 9//ultra 1 & 2 for sleeper

#define echoPin 8

#define trigPin1 7

#define echoPin1 6



//ultra for surface

#define trigPin2 5

#define echoPin2 4





File myFile;

SoftwareSerial serial_connection(0,1); //RX=pin 1, TX=pin 0 
TinyGPSPlus gps;

int isObstaclePin = 2;  // obstacle detect 

int isObstacle = HIGH;  // HIGH MEANS NO OBSTACLE

int sleeperCount=0;



int missingCount=0;

int alignCount=0;

int sleeperNum=0;
int sleeperDist=0;


int RECV_PIN = 3;//side crack er ir



IRrecv irrecv(RECV_PIN);
decode_results results;


void setup() {

  Serial.begin (9600); //for all sensor & gps
  serial_connection.begin(9600); //for gps
  
  pinMode(trigPin, OUTPUT);   //ultrasonic
  pinMode(echoPin, INPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  
  pinMode(isObstaclePin, INPUT);
  
  irrecv.enableIRIn();
  Serial.print("Initializing SD card...");
  
  pinMode(10, OUTPUT);
  if (!SD.begin(10)) {

    Serial.println("initialization failed!");

    return;

  }

  Serial.println("initialization done.");

  myFile = SD.open("test.csv", FILE_WRITE);

  myFile.print("Sleeper No"); 

    myFile.print(",");
    myFile.print("Distance(yard)"); 

    myFile.print(",");
    
    myFile.print("Latitude"); 

    myFile.print(",");

    myFile.print("Longitude"); 

    myFile.print(",");

    myFile.println("Problem");
  
    
    myFile.close();

}



void loop() { 
  float a,b;
  while(serial_connection.available())//While there are characters to come from the GPS
  {
    gps.encode(serial_connection.read());//This feeds the serial NMEA data into the library one char at a time
  }
  if(gps.location.isUpdated())
  {
    a=gps.location.lat();
    b=gps.location.lng();
    
  }

  //side crack rcvr
  
  if (irrecv.decode(&results)) {

    Serial.println(results.value, HEX);
    Serial.println("###############################side crack");
    Serial.println(" ");
    irrecv.resume();
      
    myFile = SD.open("test.csv", FILE_WRITE);
    myFile.print(sleeperNum);
    myFile.print(",");
    sleeperDist=sleeperNum*100;
    myFile.print(sleeperDist);
    myFile.print(",");
    myFile.print(a); 
    myFile.print(",");
    myFile.print(b);
    myFile.print(",");
    myFile.println("SIDE CRACK");    
    myFile.close();
  }



  //obstacle detection

  isObstacle = digitalRead(isObstaclePin);

  if (isObstacle == LOW)

  {

    Serial.println("OBSTACLE!!, OBSTACLE!!");
    myFile = SD.open("test.csv", FILE_WRITE);
     myFile.print(sleeperNum);
    myFile.print(",");
    sleeperDist=sleeperNum*100;
    myFile.print(sleeperDist);
    myFile.print(",");
    myFile.print(a); 
    myFile.print(",");
    myFile.print(b);
    myFile.print(",");
    myFile.println("OBSTACLE");     
    myFile.close();

    

  }



  //surface crack

  long duration3, distance3;

  digitalWrite(trigPin2, LOW); 
  digitalWrite(trigPin2, HIGH); 

  //delayMicroseconds(10); // Added this line

  digitalWrite(trigPin2, LOW);
  duration3 = pulseIn(echoPin2, HIGH);
  distance3 = (duration3/2) /29.1;

//  if (distance3 >= 200 ){

//    Serial.println("sensor 3 Out of range");

//  }

//  else {

//    Serial.print( "Distance3,");

//    Serial.print(distance3);

//    Serial.println("cm");

//    

//  }

 

  if(distance3 >5){

    Serial.println(distance3);

    Serial.println("??????.....surface crack....????????");

    myFile = SD.open("test.csv", FILE_WRITE);

     myFile.print(sleeperNum);

    myFile.print(",");
    sleeperDist=sleeperNum*100;
    myFile.print(sleeperDist);
    myFile.print(",");

    myFile.print(a); 

    myFile.print(",");

    myFile.print(b);

    myFile.print(",");

    myFile.println("Surface Crack");   

    

    myFile.close();

    

  }
 

  //sleeper check

  long duration, duration1,distance,distance1;

  digitalWrite(trigPin, LOW); 
  digitalWrite(trigPin1, LOW); // Added this line
  digitalWrite(trigPin, HIGH);

  digitalWrite(trigPin1, HIGH);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = (duration/2) /29.1;   

  digitalWrite(trigPin1, LOW);  

  duration1 = pulseIn(echoPin1, HIGH);  

  distance1 = (duration1/2) /29.1;





//  if (distance >= 200 || distance <= 0){
//
//    Serial.println("sensor 1 Out of range");
//
//  }
//
//  else {
//
//    Serial.print( "Distance1: ");
//
//    Serial.print(distance);
//
//    Serial.println(" cm");
//
//  }



   

//  if (distance1 >= 200 || distance1 <= 0){
//
//    Serial.println("sensor 2 Out of range");
//
//  }
//
//  else {
//
//    Serial.print( "Distance2: ");
//
//    Serial.print(distance1); 
//
//    Serial.println(" cm");  
//
//  }

  

  if (distance <=7 &&  distance1<=7){

    

    sleeperCount++;

    missingCount=0;

    alignCount=0;



    if(sleeperCount>10){

    sleeperNum++;

   
    sleeperCount=0;
    }
  }

  else if((distance<=7 && distance1>7  ) || (distance>7 && distance1<=7))

  {

    alignCount++;

    missingCount=0;

    sleeperCount=0;

    if(alignCount>10){

      sleeperNum++;

      Serial.println("sleeper alignment problem");

     myFile = SD.open("test.csv", FILE_WRITE);

     myFile.print(sleeperNum);

    myFile.print(",");
     sleeperDist=sleeperNum*100;
    myFile.print(sleeperDist);
    myFile.print(",");
     myFile.print(a); 

    myFile.print(",");

    myFile.print(b);

    myFile.print(",");

    myFile.println("sleeper alignment problem");

      

    myFile.close();

    alignCount=0; 

      

    }

     

  }

  else {



    missingCount++;

    sleeperCount=0;

    alignCount=0;

    



    if(missingCount>10){

    sleeperNum++;

    Serial.println("....sleeper not found...");

    myFile = SD.open("test.csv", FILE_WRITE);

    myFile.print(sleeperNum);

    myFile.print(",");
    sleeperDist=sleeperNum*100;
    myFile.print(sleeperDist);
    myFile.print(",");
    myFile.print(a); 

    myFile.print(",");

    myFile.print(b);

    myFile.print(",");

    myFile.println("sleeper missing");
    myFile.close();

    missingCount=0;     

    }    

    //delay(500);

  }  

  delay(100);
} 

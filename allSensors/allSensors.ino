
#include <SD.h>// sd card

#include <SPI.h>

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

int isObstaclePin = 2;  // obstacle detect 
int isObstacle = HIGH;  // HIGH MEANS NO OBSTACLE
int sleeperCount=0;
int missingCount=0;
int alignCount=0;
int sleeperNum=0;

int RECV_PIN = 3;//side crack er ir

IRrecv irrecv(RECV_PIN);

decode_results results;



void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
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
    myFile.print("Latitude"); 
    myFile.print(",");
myFile.print("Longitude"); 
    myFile.print(",");
myFile.println("Prblem"); 
   
    
    myFile.close();
  


}

void loop() { 
  

  //side crack rcvr
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    Serial.println("###############################side crack");
    Serial.println(" ");
    
    myFile = SD.open("test.csv", FILE_WRITE);
     myFile.print(sleeperNum);
    myFile.print(",");
    myFile.print("23.837622"); 
    myFile.print(",");
    myFile.print("90.358058");
    myFile.print(",");
    myFile.println("SIDE CRACK");
     
    myFile.close();
    irrecv.resume();
     
    
  }

  //obstacle detection
  isObstacle = digitalRead(isObstaclePin);
  if (isObstacle == LOW)
  {
    Serial.println("OBSTACLE!!, OBSTACLE!!");
    myFile = SD.open("test.csv", FILE_WRITE);
     myFile.print(sleeperNum);
    myFile.print(",");
    myFile.print("23.837622"); 
    myFile.print(",");
    myFile.print("90.358058");
    myFile.print(",");
    myFile.println("OBSTACLE");     
    myFile.close();
    
  }
//  else
//  {
//    Serial.println("clear");
//    
//  }


  //surface crack
  long duration3, distance3;
  digitalWrite(trigPin2, LOW); 

//  delayMicroseconds(10); // Added this line
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
 
  if(distance3 >4){
    Serial.println(distance3);
    Serial.println("??????.....surface crack....????????");
    myFile = SD.open("test.csv", FILE_WRITE);
     myFile.print(sleeperNum);
    myFile.print(",");
    myFile.print("23.837622"); 
    myFile.print(",");
    myFile.print("90.358058");
    myFile.print(",");
    myFile.println("Surface Crack");  
   
    
    myFile.close();
    
  }

  
  
  //sleeper check
  long duration, duration1,distance,distance1;
  digitalWrite(trigPin, LOW); 
 // delayMicroseconds(2);
  digitalWrite(trigPin1, LOW); // Added this line
  //delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  digitalWrite(trigPin1, HIGH);
  //delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) /29.1;   
  digitalWrite(trigPin1, LOW);  
  duration1 = pulseIn(echoPin1, HIGH);  
  distance1 = (duration1/2) /29.1;


//  if (distance >= 200 || distance <= 0){
//    Serial.println("sensor 1 Out of range");
//  }
//  else {
//    Serial.print( "Distance1: ");
//    Serial.print(distance);
//    Serial.println(" cm");
//   // delay(500);
//   
//    //sd card
//    myFile = SD.open("test.txt", FILE_WRITE);
//    myFile.println("distance1: ");
//    myFile.println(distance);  
//    myFile.close();
//  }
  //delay(500);
   
//  if (distance1 >= 200 || distance1 <= 0){
//    Serial.println("sensor 2 Out of range");
//  }
//  else {
//    Serial.print( "Distance2: ");
//    Serial.print(distance1);
//    Serial.println(" cm");
//    
//    myFile = SD.open("test.txt", FILE_WRITE);
//    myFile.println("distance2: ");
//    myFile.println(distance1);  
//    myFile.close();
//  }
  
  if (distance <=7 &&  distance1<=7){
    
    sleeperCount++;
    missingCount=0;
    alignCount=0;

    if(sleeperCount>10){
    sleeperNum++;
    Serial.println("sleeper detected");
    myFile = SD.open("test.csv", FILE_WRITE);
    myFile.print(sleeperNum);
    myFile.print(",");
    myFile.println("sleeper detected");
      
    myFile.close();
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
     myFile.print("23.837622"); 
    myFile.print(",");
    myFile.print("90.358058");
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
    myFile.print("23.837622"); 
    myFile.print(",");
    myFile.print("90.358058");
    myFile.print(",");
    myFile.println("sleeper missing");
      
    myFile.close();
    missingCount=0;
      
    }
   
    
    //delay(500);
  }

  
  delay(100);
  
  
}

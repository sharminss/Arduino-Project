

void setup() {
  // put your setup code here, to run once:
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  
// put your main code here, to run repeatedly:
//  for (int i = 0; i < 3; i++) {
//    irsend.sendSony(0xa90, 12);
//    
//    delay(40);
//  }
 
  

  

  analogWrite(9,80);
  analogWrite(11,80 ); 
 
  digitalWrite(8,HIGH);
  digitalWrite(7,LOW);
  digitalWrite(13,HIGH);
  digitalWrite(12,LOW);
  
  delay(500);

  
  digitalWrite(8,LOW);
  digitalWrite(7,LOW);
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
  
  delay(2000);
  
  
}

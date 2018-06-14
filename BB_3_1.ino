 // BB_3_1 
// Uses an ultrasonic sensor to detect distance
// Created by: Ali Meshkat, Artem Sotnikov
// Date: May 28th, 2018
//

#include <Servo.h>;

int pingT = 9;
int pingE = 8;
int toCm = 28;

int leftSig = 2;
int midSig = 3;
int rightSig = 4;

long duration;

Servo scanner;
Servo servoR;
Servo servoL;

void setup() {
  Serial.begin(9600);
  pinMode(pingT, OUTPUT);
  pinMode(pingE, INPUT);
  pinMode(leftSig, OUTPUT);
  pinMode(rightSig, OUTPUT);
  pinMode(midSig, OUTPUT);


  
  scanner.attach(11);
  servoR.attach(12);
  servoL.attach(10);

  int servoSet = scanF(1000);
  //standStill(1000000);
}

void standStill(int cycles) { // sets the robot to stand still for a given number of cycles
  for (int i = 0; i < cycles; i++) {
    servoR.writeMicroseconds(1500);
    servoL.writeMicroseconds(1500);  
    delay(20); 
  }
}

void moveForward(int cycles) { // sets the robot to move forward for a given number of cycles
  digitalWrite(midSig, HIGH);
  for (int i = 0; i < cycles; i++) {
    servoR.writeMicroseconds(1375);
    servoL.writeMicroseconds(1700);  
    delay(20);
  }  
    digitalWrite(midSig, LOW);

}

void moveBack(int cycles) { // sets the robot to move back for a given number of cycles
  for (int i = 0; i < cycles; i++) {
    servoR.writeMicroseconds(1700);
    servoL.writeMicroseconds(1300);  
    delay(20);
  }  
}

void turnLeft(int cycles) { // sets the robot to turn right for a given number of cycles
    digitalWrite(leftSig, HIGH);
  for (int i = 0; i < cycles; i++) {
    servoR.writeMicroseconds(1300);
    servoL.writeMicroseconds(1300);  
    delay(10);
  }  
      digitalWrite(leftSig, LOW);

}

void turnRight(int cycles) { // sets the robot to turn left for a given number of cycles
  digitalWrite(rightSig, HIGH);
  for (int i = 0; i < cycles; i++) {
    servoR.writeMicroseconds(1700);
    servoL.writeMicroseconds(1700);  
    delay(10);
  }
  digitalWrite(rightSig, LOW);
}

int scanAny(int directionCode, int delayTime) {
  int cm;
  if(delayTime == 1000 || delayTime == 2000){
  cm = 0;
  //delay(1000);
  for(int i = 0; i <= 4;i++){
    scanner.writeMicroseconds(directionCode);
    delay(delayTime/3);
    digitalWrite(pingT,LOW);
    delayMicroseconds(2);
    digitalWrite(pingT,HIGH);
    delayMicroseconds(5);
    digitalWrite(pingT,LOW);
  
    duration = pulseIn(pingE, HIGH, 25000);
  
    cm += duration/toCm/2;
  }
    cm = cm/4;
  }else {
    scanner.writeMicroseconds(directionCode);
    delay(delayTime);
    digitalWrite(pingT,LOW);
    delayMicroseconds(2);
    digitalWrite(pingT,HIGH);
    delayMicroseconds(5);
    digitalWrite(pingT,LOW);
  
    duration = pulseIn(pingE, HIGH, 25000);
  
    cm = duration/toCm/2;
  }
  if(cm == 0){
    cm = 5000;
    Serial.println(cm);
    return 5000;
  }
 Serial.println(cm); 
  return cm;
}

int scanR(int delayTime) {
  int result = scanAny(275, delayTime);
  Serial.print("Right: ");
  Serial.println(result);
  return result;
}

int scanL(int delayTime) {
  int result =  scanAny(2250, delayTime);
  Serial.print("Left: ");
  Serial.println(result);
  return result;
}

int scanF(int delayTime) {
  return scanAny(1375, delayTime);
}

void loop() {
  //int servoSet = scanF(1000);
  
  //int test = scanL();
  
  if (scanF(100) < 14) {
    standStill(1);
    if (scanR(1000) <=  scanL(2000)) {
      turnLeft(58);  
      
    } else {
      turnRight(60);  
    }
    standStill(1);
    int servoSet = scanF(1000);
    standStill(1);
  } else {
    moveForward(1);  
  }
  

  
  delay(200);
}



/*
 * [BoeBotMaze.ino]
 * The Boebot will find its way through a maze by using an ultrasonic sensor and two infrared sensors
 * the boebot plays 'Imperial March' when finds the exit after getting there
 * it also has led's to act as signals when turning or moving straight eg. the left led is on when the bot is turning left and middle led flashes when bot is moving straight
 * @author Ali Meshkat, Artem Sotnikov
 * @date June 19, 2018
 * @instructor Ms. Stretten
 */


#include <Servo.h> 


//defining pins
//ultrasonic
int pingT = 9; //trig
int pingE = 8; //echo
int toCm = 28;

//led's
int leftSig = 2;
int midSig = 3;
int rightSig = 4;

//defining vars
int turn;
long duration;

//servos
Servo scanner;
Servo servoR;
Servo servoL;

//speaker
int speaker = 5;

//NOTES &#39;c&#39; , &#39;d&#39;, &#39;e&#39;, &#39;f&#39;, &#39;g&#39;, &#39;a&#39;, &#39;b&#39;, &#39;C&#39;
int notes[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 }; //freq

#include "pitches.h"

int full_n = 1;
int half_n = 2;
int quart_n = 4;
int eigth_n = 8;

int melody[] = {NOTE_A4,NOTE_A4,NOTE_A4,NOTE_F4,NOTE_C5,NOTE_A4,NOTE_F4,NOTE_C5,NOTE_A4,NOTE_E5,NOTE_E5,NOTE_E5,NOTE_F5,NOTE_C5,NOTE_GS4,NOTE_F4,NOTE_C5,NOTE_A4};
int noteDuration[] = {half_n,half_n,half_n,quart_n,quart_n,half_n,quart_n,quart_n,full_n,half_n,half_n,half_n,quart_n,quart_n,half_n,quart_n,quart_n,half_n};

int leftLight = 11;
int leftIR = 12;

int rightLight = 6;
int rightIR = 7;


void setup() {
  Serial.begin(9600);
  
  pinMode(pingT, OUTPUT);
  pinMode(pingE, INPUT);
  pinMode(leftSig, OUTPUT);
  pinMode(rightSig, OUTPUT);
  pinMode(midSig, OUTPUT);
  pinMode(speaker, OUTPUT);

  pinMode(leftLight,OUTPUT);
  pinMode(rightLight,OUTPUT);

  pinMode(leftIR,INPUT);
  pinMode(rightIR,INPUT);

  turn = 0;
  
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

int irDetect(int ledPin, int recPin, long freq) {
  tone(ledPin,freq,8);

    delay(1);
    int ir = digitalRead(recPin);
    delay(1);
    return ir;
}

int irDetRight() {
  return irDetect(rightLight,rightIR,36100);
}

int irDetLeft() {
  return irDetect(leftLight,leftIR,38000);
}

int scanAny(int directionCode, int delayTime) {
  int cm;
  if(delayTime == 1000 || delayTime == 2000){
  cm = 0;
  //delay(1000);
  for(int i = 0; i <= 4;i++) {
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
  } else {
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

int fDis ;

void loop() {
  //int servoSet = scanF(1000);
  
  //int test = scanL();
  if(turn == 8){

  moveForward(240);
  standStill(1);
  
    for (int note = 0; note < sizeof(melody); note++) {
        tone(speaker,melody[note],(2000/noteDuration[note]));
        delay(2000/noteDuration[note]);
        digitalWrite(speaker,LOW);
        delay((2000/noteDuration[note])*0.3);  
    }   

    digitalWrite(speaker,LOW);
  }

  fDis = scanF(100);
  
  if (scanF(30) < 14) {
    standStill(1);
    if (scanR(1000) <=  scanL(2000)) {
      turnLeft(53);//59  
      turn++;
    } else {
      turnRight(54);  //60
      turn++;
    }
    standStill(1);
    int servoSet = scanF(1000);
    standStill(1);
  } 
      

  if (irDetLeft() == 1) {
    turnRight(2);
    Serial.println(irDetLeft());
  }  if (irDetRight() == 0) {
    turnLeft(2);
    Serial.println(irDetRight());
  }
  
  Serial.print("Left: ");
  Serial.println(irDetLeft());
  Serial.print("Right: ");
  Serial.println(irDetRight());

  moveForward(1);
  
  //delay(20);
}



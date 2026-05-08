#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

SoftwareSerial BT(10, 11);

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  150
#define SERVOMAX  600

#define BASE_CH     13
#define SHOULDER_CH 14
#define GRIPPER_CH  15

float baseAngle     = 130.0;   
float shoulderAngle = 35.0;  
float gripperAngle  = 20.0;  

int stepDelay = 20; 

void moveServo(int num, float angle) {
  int pulse = map((int)angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(num, 0, pulse);
}

void updateAllServos() {
  moveServo(BASE_CH, baseAngle);
  moveServo(SHOULDER_CH, shoulderAngle);
  moveServo(GRIPPER_CH, gripperAngle);
}

void smoothMove(int channel, float &currentAngle, float targetAngle) {
  while (abs(currentAngle - targetAngle) > 0.5) {
    if (currentAngle < targetAngle) currentAngle += 1.0; 
    else currentAngle -= 1.0; 
    moveServo(channel, currentAngle);
    delay(stepDelay); 
  }
  currentAngle = targetAngle;
  moveServo(channel, currentAngle);
}

void changeMode(String modeName, float targetBase, float targetShoulder) {
  BT.print("Mode: "); BT.println(modeName);
  smoothMove(BASE_CH, baseAngle, targetBase);
  delay(300); 
  smoothMove(SHOULDER_CH, shoulderAngle, targetShoulder);
  BT.println("OK");
}

void setup() {
  Serial.begin(115200);
  BT.begin(9600); 
  
  Wire.begin(); 
  pwm.begin();
  pwm.setPWMFreq(50);
  updateAllServos(); 
}

void loop() {
  if (BT.available()) {
    if (isDigit(BT.peek())) {
      int firstNum = BT.parseInt();
      
      if (firstNum == 13 || firstNum == 14 || firstNum == 15) {
        int angle = BT.parseInt();
        while(BT.available() > 0) { BT.read(); }
        
        if (firstNum == 13) baseAngle = angle;
        if (firstNum == 14) shoulderAngle = angle;
        if (firstNum == 15) gripperAngle = angle;
        
        moveServo(firstNum, angle); 
      }
      else if (firstNum == 1) { while(BT.available()>0) BT.read(); changeMode("1", 130.0, 35.0); }
      else if (firstNum == 2) { while(BT.available()>0) BT.read(); changeMode("2", 0.0, 55.0); }
      else if (firstNum == 3) { while(BT.available()>0) BT.read(); changeMode("3", 130.0, 170.0); }
    } 
    else {
      char cmd = BT.read();
      if(cmd != '\n' && cmd != '\r' && cmd != ' ') {
        bool armMoved = false;
        
        if      (cmd == 'T') { shoulderAngle -= 2.0; armMoved = true; } 
        else if (cmd == 'X') { shoulderAngle += 2.0; armMoved = true; } 
        else if (cmd == 'U') { baseAngle     += 2.0; armMoved = true; } 
        else if (cmd == 'D') { baseAngle     -= 2.0; armMoved = true; } 
        else if (cmd == 'G') { gripperAngle  += 5.0; armMoved = true; } 
        else if (cmd == 'H') { gripperAngle  -= 5.0; armMoved = true; } 

        if (armMoved) {
          baseAngle = constrain(baseAngle, 0.0, 180.0);
          shoulderAngle = constrain(shoulderAngle, 0.0, 180.0);
          gripperAngle = constrain(gripperAngle, 0.0, 180.0);
          
          moveServo(BASE_CH, baseAngle);
          moveServo(SHOULDER_CH, shoulderAngle);
          moveServo(GRIPPER_CH, gripperAngle);
          delay(20); 
        }
      }
    }
  }
}

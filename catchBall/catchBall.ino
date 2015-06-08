#include <ArduinoRobot.h>
#include <SPI.h>
#include <Wire.h>

int TOLERANCE_DISTANCE = 5;
int BALL_DISTANCE = 20;
int MAX_SPEED = 175;

void setup() {
  // put your setup code here, to run once:
  Robot.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  moveUntilWallFront(20);
  delay(2000);
}

void moveUntilWallFront (int targetDistance) {
  while (Robot.analogRead(M2) * 1.27 > (targetDistance + TOLERANCE_DISTANCE)){
    Robot.motorsWrite(MAX_SPEED, MAX_SPEED);
    delay(200);
  }
  Robot.motorsWrite(0, 0);
}

#include <ArduinoRobot.h>
#include <SPI.h>
#include <Wire.h>

int DISTANCE_FROM_WALL = 20;
int MAX_SPEED = 255;
int CORRECTION_SPEED = 100;

int chosenSensor = M0;
void setup() {
  // put your setup code here, to run once:
  Robot.begin();
  Robot.beginTFT();
  Robot.stroke(0, 0, 0);
  Robot.textSize(2);

  if (Robot.analogRead(M0) < Robot.analogRead(M4)){
    chosenSensor = M0;
    Robot.text("droite", 5,5);
    Robot.text(Robot.analogRead(M4), 5,80);
    Robot.text(Robot.analogRead(M0), 80,80);
  }
  else {
    chosenSensor = M4;
    Robot.text("gauche", 5,5);
    Robot.text(Robot.analogRead(M4), 5,80);
    Robot.text(Robot.analogRead(M0), 80,80);

  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Robot.motorsWrite(MAX_SPEED, MAX_SPEED);

  int wallDistance = (int) Robot.analogRead(chosenSensor) * 1.27;
  if (wallDistance > DISTANCE_FROM_WALL) {
    // On est loin du mur de droite
    if (chosenSensor == M0){
      Robot.motorsWrite(MAX_SPEED,CORRECTION_SPEED);
    }
    // On est loin du mur de gauche
    else {
      Robot.motorsWrite(CORRECTION_SPEED,MAX_SPEED);
    }
  }
  else if (wallDistance < DISTANCE_FROM_WALL) {
    // On est près du mur de droite
    if (chosenSensor == M0){
      Robot.motorsWrite(CORRECTION_SPEED,MAX_SPEED);
    }
    // On est près du mur de gauche
    else {
      Robot.motorsWrite(MAX_SPEED,CORRECTION_SPEED);
    }
  }
  delay(10);
}

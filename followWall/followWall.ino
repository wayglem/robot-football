#include <ArduinoRobot.h>
#include <SPI.h>
#include <Wire.h>

int DISTANCE_FROM_WALL = 20;
int MAX_SPEED = 255;
int CORRECTION_SPEED = 150;
int TOLERANCE_DISTANCE = 5;
int DISTANCE_TO_CUT_CORNER = 60;

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
  int wallSideDistance = (int) Robot.analogRead(chosenSensor) * 1.27;
  int wallFrontDistance = (int) Robot.analogRead(M2) * 1.27;
  int wallLeftDistance = (int) Robot.analogRead(M4) * 1.27;
  int wallRightDistance = (int) Robot.analogRead(M0) * 1.27;
  Robot.clearScreen();
  if (wallFrontDistance < DISTANCE_FROM_WALL *2 ) {
    Robot.text("collision imminente",5,5);
    if (wallLeftDistance < wallRightDistance) {
      Robot.text("Virage droite",5,80);
      Robot.motorsWrite(MAX_SPEED, -100);
    }
    else {
      Robot.text("Virage gauche",5,80);
      Robot.motorsWrite(-100, MAX_SPEED);
    }
    delay(250);
    Robot.motorsWrite(255,255);
    delay(50);
  }
  else if(wallFrontDistance > DISTANCE_TO_CUT_CORNER && 
    wallSideDistance > DISTANCE_FROM_WALL - TOLERANCE_DISTANCE && 
    wallSideDistance < DISTANCE_FROM_WALL + TOLERANCE_DISTANCE) {
    
      Robot.text("Rien devant",5,5);
      Robot.text("A fond !",5,80);
      Robot.motorsWrite(255,255);
      delay(300);
  }
  else if (wallSideDistance > DISTANCE_FROM_WALL + TOLERANCE_DISTANCE) {
    // On est loin du mur de droite
    if (chosenSensor == M0){
      Robot.text("Chicane Droite",5,5);
      chicaneDroite();
    }
    // On est loin du mur de gauche
    else {
      Robot.text("Chicane Gauche",5,5);
      chicaneGauche();
    }
  }
  else if (wallSideDistance < DISTANCE_FROM_WALL - TOLERANCE_DISTANCE) {
    // On est près du mur de droite
    if (chosenSensor == M0){
      Robot.text("Chicane Gauche",5,5);
      chicaneGauche();
    }
    // On est près du mur de gauche
    else {
      Robot.text("Chicane Droite",5,5);
      chicaneDroite();
    }
  }
  else {
    Robot.text("Rien",5,5);
    delay(200);
  }
}
void chicaneDroite() {
  Robot.motorsWrite(MAX_SPEED, 0);
  delay(200);
  Robot.motorsWrite(0, MAX_SPEED);
  delay(200);
  Robot.motorsWrite(MAX_SPEED, MAX_SPEED);
}
void chicaneGauche() {
  Robot.motorsWrite(0, MAX_SPEED);
  delay(200);
  Robot.motorsWrite(MAX_SPEED, 0);
  delay(200);
  Robot.motorsWrite(MAX_SPEED, MAX_SPEED);
}


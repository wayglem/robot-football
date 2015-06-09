#include <ArduinoRobot.h>
#include <SPI.h>
#include <Wire.h>

//int haut, bas, gauche, droite = 0;

int haut = 157;
int gauche = 84;
int bas = 283;
int droite = 219;

void setup() {
  // put your setup code here, to run once:
  Robot.begin();
  Robot.beginTFT();
  Robot.beginSpeaker();
  Robot.stroke(0, 0, 0);

  Robot.text("Compass : haut", 5, 5);
  Robot.waitContinue(BUTTON_MIDDLE);
  haut =  (int) Robot.compassRead();
  Robot.text(haut, 40, 70);
  delay(2000);
  Robot.clearScreen();
  Robot.text("Compass : gauche", 5, 5);
  Robot.waitContinue(BUTTON_MIDDLE);
  gauche =  (int) Robot.compassRead();
  Robot.text(gauche, 40, 70);
  delay(2000);
  Robot.clearScreen();
  Robot.text("Compass : bas", 5, 5);
  Robot.waitContinue(BUTTON_MIDDLE);
  bas =  (int) Robot.compassRead();
  Robot.text(bas, 40, 70);
  delay(2000);
  Robot.clearScreen();
  Robot.text("Compass : droite", 5, 5);
  Robot.waitContinue(BUTTON_MIDDLE);
  droite =  (int) Robot.compassRead();
  Robot.text(droite, 40, 70);
  delay(2000);
  Robot.clearScreen();
}

void loop() {
  // put your main code here, to run repeatedly:
  Robot.motorsWrite(150,150);
  delay(2000);
  Robot.motorsStop();
  delay(2000);
}

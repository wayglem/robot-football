#include <ArduinoRobot.h>
#include <SPI.h>
#include <Wire.h>

int haut = 158;
int gauche = 78;
int bas = 283;
int droite = 218;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  moveUntil(30);
  delay(1000);
}
void moveUntil(int distanceInCM){
  int sensorDistance = (int) Robot.analogRead(M2) * 1.27;
  while(sensorDistance > distanceInCM){
    Robot.motorsWrite(255,255);
    delay(40);
  }
  Robot.motorsStop();
}

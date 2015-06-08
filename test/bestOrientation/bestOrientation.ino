#include <ArduinoRobot.h>
#include <SPI.h>
#include <Wire.h>


int cap[4] = {148, 309, 230, 55};

void setup() {
  // put your setup code here, to run once:
  Robot.begin();
  Robot.beginTFT();
  Robot.stroke(0, 0, 0);
  Robot.textSize(2);
}

void loop() {
  // put your main code here, to run repeatedly:
  Robot.clearScreen();
  bestOrientation(Robot.compassRead());
  delay(200);
}
int bestOrientation (int compass) {
  int chosenDirection = cap[0];
  int min = abs(compass - cap[0]);
  Robot.text(compass, 5,5);
  for (int i=1;i<4;i++){
    if (abs(compass - cap[i])<min){
      min = abs(compass - cap[i]);
      chosenDirection = cap[i];
    }
  }
  Robot.text(chosenDirection, 50,50);
  return chosenDirection;
}


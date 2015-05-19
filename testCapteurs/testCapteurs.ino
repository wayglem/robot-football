
#include <ArduinoRobot.h>
#include <SPI.h>
#include <Wire.h>

int frontSensor = M2;
int leftSensor = M4;
int rightSensor = M0;
int rearSensor = M6;

void setup() {
  // put your setup code here, to run once:
  Robot.begin();
  Robot.beginTFT();
  Serial.begin(9600);
}

void loop() {
  String frontValue = String(Robot.analogRead(frontSensor));
  
  Robot.clearScreen();
  Robot.stroke(0, 0, 0);
  Robot.textSize(2);
  Robot.text(getOlympicDistance(frontSensor),54,5);  
  Robot.text(getOlympicDistance(leftSensor),5,80);  
  Robot.text(getOlympicDistance(rightSensor),100,80);  
  Robot.text(getOlympicDistance(rearSensor),54,140);
  
  Robot.text((int)Robot.compassRead(), 54, 80); //affiche la valeur du compas
  
  delay(1000);
}

int getOlympicDistance(int sensorPin) {
  Serial.print(sensorPin);
  Serial.print("[");
  int ITERATIONS = 10;
  int values[ITERATIONS];
  int min = 9999;
  int max = 0;
  int sum = 0;
  for (int i=0;i<ITERATIONS;i++){
    values[i] = Robot.analogRead(sensorPin);
    Serial.print(values[i]);
    Serial.print(", ");
    sum += values[i];
    if(values[i] < min) {
      min = values[i];
    }
    if(values[i] > max) {
      max = values[i];
    }
     delay(10);
  }
  Serial.print("]");
  Serial.println("");
  return (int) (((sum - (max + min )) / (ITERATIONS - 4))*1.27);
}

int getDistance(int sensorPin) {
  return (int) Robot.analogRead(sensorPin)*1.27;
}

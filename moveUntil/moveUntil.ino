#include <ArduinoRobot.h>
#include <SPI.h>
#include <Wire.h>

int haut = 158;
int gauche = 78;
int bas = 283;
int droite = 218;

void setup() {
  // put your setup code here, to run once:
  Robot.begin();
  Robot.beginTFT();
  Robot.stroke(0, 0, 0);
  Robot.beginSpeaker();
  Robot.textSize(2);
}

void loop() {
  // put your main code here, to run repeatedly:
  moveUntil(25);
  delay(3000);
}
void moveUntil(int distanceInCM){
  int sensorDistance = (int) Robot.analogRead(M2) * 1.27;
  bool isArrived = false;
  while(!isArrived) {
    while(sensorDistance > distanceInCM){
      Robot.motorsWrite(255,255);
      delay(10);
      sensorDistance = (int) Robot.analogRead(M2) * 1.27;
    }
    Robot.motorsStop();
    Robot.beep(BEEP_SIMPLE);
    int * sensorsValues = getOlympicDistance();
    if (sensorsValues[1] <= distanceInCM) {
      isArrived = true;
      Robot.beep(BEEP_DOUBLE);
    }
    sensorDistance = (int) Robot.analogRead(M2) * 1.27;    
    free(sensorsValues);
  }
  Robot.motorsStop();
}
// [0] = x2 (droite)
// [1] = y1 (haut)
// [2] = x1 (gauche)
// [3] = y2 (bas)
int * getOlympicDistance() {
  int ITERATIONS = 50;
  int values[4][ITERATIONS];
  int * sums = new int[4];
  sums[0] = 0;
  sums[1] = 0;
  sums[2] = 0;
  sums[3] = 0;
  int OLYMPICNBTRUNCATE = 10;
  for (int i = 0; i < ITERATIONS; i++) {
    values[0][i] = Robot.analogRead(M0);
    values[1][i] = Robot.analogRead(M2);
    values[2][i] = Robot.analogRead(M4);
    values[3][i] = Robot.analogRead(M6);
    delay(20);
  }
  sort(values[0], ITERATIONS);
  sort(values[1], ITERATIONS);
  sort(values[2], ITERATIONS);
  sort(values[3], ITERATIONS);
  for (int i = OLYMPICNBTRUNCATE - 1; i < ITERATIONS - OLYMPICNBTRUNCATE - 1; i++) {
    sums[0] = sums[0] + values[0][i];
    sums[1] = sums[1] + values[1][i];
    sums[2] = sums[2] + values[2][i];
    sums[3] = sums[3] + values[3][i];
  }
  for (int j = 0; j < 4; j++) {
    sums[j] = (int) (sums[j] / (ITERATIONS - (2 * OLYMPICNBTRUNCATE))) * 1.27;
  }
  return sums;
}
void sort(int a[], int size) {
  for (int i = 0; i < (size - 1); i++) {
    for (int o = 0; o < (size - (i + 1)); o++) {
      if (a[o] > a[o + 1]) {
        int t = a[o];
        a[o] = a[o + 1];
        a[o + 1] = t;
      }
    }
  }
}

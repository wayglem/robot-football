#include <ArduinoRobot.h>
#include <SPI.h>
#include <Wire.h>

int frontSensor = M2;
int leftSensor = M4;
int rightSensor = M0;
int rearSensor = M6;

int hPlan = 135;
int lPlan = 103;
int ml = 12; // marge largeur
int mh = 12; // marge hauteur
int a0 = 295;

void setup() {
  // put your setup code here, to run once:
  Robot.begin();
  Robot.beginTFT();
  Serial.begin(9600);
  Robot.stroke(0, 0, 0);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Robot.clearScreen();
  Robot.rect(ml, mh, lPlan, hPlan);
  Robot.pointTo(a0);
  int * distance = getOlympicDistance();
  
  Robot.circle(distance[2]+diamRobot+ml, distance[1]+diamRobot+mh, 4);
  
  
  Robot.textSize(2);
  Robot.text(distance[2]+diamRobot, 2, 80); //Affiche la distance de gauche
  Robot.text(distance[1]+diamRobot, 47, 3); // Affiche la distance du haut
  Robot.text(distance[0]+diamRobot, 93, 80); //Affiche la distance de droite
  Robot.text(distance[3]+diamRobot, 47, 145); //Affiche la distance du bas
  Robot.text((int)Robot.compassRead(), 47, 70); //Affiche la boussole
  free(distance);
  delay(1000);
  
}

// TODO : a transformer en .h
// [0] = x2 (droite)
// [1] = y1 (haut)
// [2] = x1 (gauche)
// [3] = y2 (bas)
int * getOlympicDistance() {
  int ITERATIONS = 50;
  int values[4][ITERATIONS];
  int a[ITERATIONS];
  int sum = 0;
  int * sums = new int[4];
  sums[0]=0;
  sums[1]=0;
  sums[2]=0;
  sums[3]=0;
  
  int OLYMPICNBTRUNCATE = 10;
  Serial.print("avant : [ ");
  for (int i=0;i<ITERATIONS;i++){
    values[0][i] = Robot.analogRead(M0);
    values[1][i] = Robot.analogRead(M2);
    a[i] = Robot.analogRead(M2);
    values[2][i] = Robot.analogRead(M4);
    values[3][i] = Robot.analogRead(M6);
    sum = sum + values[1][i];
    Serial.print(values[1][i]);
    Serial.print(", ");
    delay(20);
  }
  Serial.print("]\n");
  sort(values[0], ITERATIONS);
  sort(values[1], ITERATIONS);
  sort(values[2], ITERATIONS);
  sort(values[3], ITERATIONS);
  for (int i=OLYMPICNBTRUNCATE-1;i<ITERATIONS-OLYMPICNBTRUNCATE-1;i++) {
    sums[0] = sums[0]+values[0][i];
    sums[1] = sums[1]+values[1][i];
    sums[2] = sums[2]+values[2][i];
    sums[3] = sums[3]+values[3][i];
  }
  for(int j=0; j<4;j++){
   sums[j]=(int) (sums[j]/(ITERATIONS-(2*OLYMPICNBTRUNCATE)))*1.27;
  }
  Serial.println("");
  return sums;
}
void sort(int a[], int size) {
    for(int i=0; i<(size-1); i++) {
        for(int o=0; o<(size-(i+1)); o++) {
                if(a[o] > a[o+1]) {
                    int t = a[o];
                    a[o] = a[o+1];
                    a[o+1] = t;
                }
        }
    }
}

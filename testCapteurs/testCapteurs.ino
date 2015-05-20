
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
  int * distances =getRecurentDistance();
  Robot.text(distances[1],54,5);  
  Robot.text(distances[0],5,80);  
  Robot.text(distances[2],100,80);  
  Robot.text(distances[3],54,140);
  Robot.text((int)Robot.compassRead(), 54, 80); //affiche la valeur du compas
  free(distances);
  delay(3000);
}

int getDistance(int sensorPin) {
  return (int) Robot.analogRead(sensorPin)*1.27;
}

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

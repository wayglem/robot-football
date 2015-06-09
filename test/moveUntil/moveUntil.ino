#include <ArduinoRobot.h>
#include <SPI.h>
#include <Wire.h>

int MAX_SPEED = 150;
int CORRECTION_SPEED = 100;
int RAYON_ROBOT = 10;
int H_PLAN = 135;
int L_PLAN = 105;
int cap[4] = {158, 270, 214, 104};
int frontSensor;

void setup() {
  // put your setup code here, to run once:
  Robot.begin();
  Robot.beginTFT();
  Robot.stroke(0, 0, 0);
  Robot.textSize(2);
  activateSensor();
  Robot.beginSpeaker();
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:

  Robot.clearScreen();
  Robot.text("=>A",50,50);
  goTo(23,19);
  Robot.text("OK",90,50);
  delay(5000);
  Robot.clearScreen();
  Robot.text("=>C",50,50);
  goTo(42,45);
  Robot.text("OK",90,50);
  delay(5000);
  Robot.clearScreen();
  Robot.text("=>G",50,50);
  goTo(74,110);
  Robot.text("OK",90,50);
  delay(5000);
  }
void moveUntil(int distanceInCM){
  Robot.clearScreen();
  Robot.text("dist",3,3);
  Robot.text(distanceInCM, 80,3);
  Robot.text("val",3,50);
  Robot.text(getFrontValue(), 80,50);
  while (distanceInCM < getFrontValue()) {
    Robot.motorsWrite(150,150);
    delay(50);
  }
  Robot.motorsStop();
  delay(100);
}
int getFrontValue (){
  return sonar_read_mode(M2) * 1.27;
}
void goTo(int x, int y) {
  int TOLERANCE = 5;
  int * sensorValues = getSensorsValue();
  int * coordinates = robotCoordinates(sensorValues[0], sensorValues[1], sensorValues[2], sensorValues[3], Robot.compassRead());
  while(abs(coordinates[0] - x) > TOLERANCE || abs(coordinates[1] - y) > TOLERANCE){
    if(coordinates[1] - y > TOLERANCE) {
      Robot.pointTo(cap[0]);
      moveUntil(y);
    }
    else if (coordinates[1] - y < TOLERANCE) {
      Robot.pointTo(cap[1]);
      moveUntil(H_PLAN - y);
    }
    delay(1000);
    sensorValues = getSensorsValue();
    coordinates = robotCoordinates(sensorValues[0], sensorValues[1], sensorValues[2], sensorValues[3], Robot.compassRead());
    if(coordinates[0] - x < TOLERANCE) {
      Robot.pointTo(cap[2]);
      moveUntil(L_PLAN - x);
    }
    else if (coordinates[0] - x > TOLERANCE) {
      Robot.pointTo(cap[3]);
      moveUntil(x);
    }
    delay(1000);
    sensorValues = getSensorsValue();
    coordinates = robotCoordinates(sensorValues[0], sensorValues[1], sensorValues[2], sensorValues[3], Robot.compassRead());
    delay(200);
  }
  Robot.text(coordinates[0],3,110);
  Robot.text(coordinates[1],50,110);
  free(sensorValues);
  free(coordinates);
}

void moveStraight (int orientation){
  int TOLERANCE_COMPASS = 5;
    if (Robot.compassRead() > (orientation + TOLERANCE_COMPASS)%360) {
      Robot.motorsWrite(CORRECTION_SPEED,MAX_SPEED);
    }
    else if (Robot.compassRead() < (orientation - TOLERANCE_COMPASS)%360) {
      Robot.motorsWrite(MAX_SPEED, CORRECTION_SPEED);
    }
    else {
      Robot.motorsWrite(MAX_SPEED,MAX_SPEED);
    }
    delay(50);
}

int * getSensorsValue (){
  int * values = new int[4];
  values[0] = sonar_read_mode(M2) * 1.27;
  values[1] = sonar_read_mode(M6) * 1.27;
  values[2] = sonar_read_mode(M0) * 1.27;
  values[3] = sonar_read_mode(M4) * 1.27;
  return values;
}

/**
  returns an array of 2 int, the first is x coordinates, the second is y coordiantes.
*/
int * robotCoordinates (int frontDistance, int backDistance, int rightDistance, int leftDistance, int orientation) {
  int * coordinates = new int[2];
  if(bestOrientation (orientation) == cap[0]){
    if (leftDistance < rightDistance) {
      coordinates[0] = leftDistance + RAYON_ROBOT;
    }
    else {
      coordinates[0] = L_PLAN - rightDistance - RAYON_ROBOT;
    }
    if (frontDistance < backDistance) {
      coordinates[1] = frontDistance + RAYON_ROBOT;
    }
    else {
      coordinates[1] = H_PLAN - backDistance - RAYON_ROBOT;
    }
  }
  else if (bestOrientation (orientation) == cap[1]){
    if (leftDistance < rightDistance) {
      coordinates[0] = L_PLAN - leftDistance - RAYON_ROBOT;
    }
    else {
      coordinates[0] = rightDistance + RAYON_ROBOT;
    }
    if (frontDistance < backDistance) {
      coordinates[1] = H_PLAN - frontDistance - RAYON_ROBOT;
    }
    else {
      coordinates[1] = backDistance + RAYON_ROBOT;
    }
  }
  else if (bestOrientation (orientation) == cap[2]){
    if (leftDistance < rightDistance) {
      coordinates[1] = leftDistance + RAYON_ROBOT;
    }
    else {
      coordinates[1] = H_PLAN - rightDistance - RAYON_ROBOT;
    }
    if (frontDistance < backDistance) {
      coordinates[0] = L_PLAN - frontDistance - RAYON_ROBOT;
    }
    else {
      coordinates[0] = backDistance + RAYON_ROBOT;
    }
  }
  else if (bestOrientation (orientation) == cap[3]){
    if (leftDistance < rightDistance) {
      coordinates[1] = H_PLAN - leftDistance - RAYON_ROBOT;
    }
    else {
      coordinates[1] = rightDistance + RAYON_ROBOT;
    }
    if (frontDistance < backDistance) {
      coordinates[0] = frontDistance + RAYON_ROBOT;
    }
    else {
      coordinates[0] = L_PLAN - backDistance - RAYON_ROBOT;
    }
  }
  return coordinates;
}

int bestOrientation (int compass) {
  int chosenDirection = cap[0];
  int min = abs(compass - cap[0]);
  for (int i=1;i<4;i++){
    if (abs(compass - cap[i])<min){
      min = abs(compass - cap[i]);
      chosenDirection = cap[i];
    }
  }
  return chosenDirection;
}


// Lecture du capteur MaxSonar, 
// on considère que la valeur est bonne si deux lectures successives sont très proches
int sonar_read_mode( int pin) {
   int sum, prevprev, prev, last;
   prev = last = 0;
   do {
    if (prev) {
      prevprev = prev; 
    }
    else {
      prevprev = Robot.analogRead(pin); 
      delay(10); 
    }
    if (last) {
      prev = last;
    } 
    else { 
      prev = Robot.analogRead(pin); 
      delay(10); 
    }
    last = Robot.analogRead(pin);
    sum = (prevprev + prev)>>1;
  } while (abs(sum-last)>2);
  //return last * (2.54/2.);
  return last;
}


#define SONAR_SENSOR_TX D0

void activateSensor() {
	pinMode( SONAR_SENSOR_TX, OUTPUT); // la patte utilisée pour l'activation de la lecture des sonars
	digitalWrite( SONAR_SENSOR_TX, HIGH);
	delayMicroseconds(25);
	digitalWrite( SONAR_SENSOR_TX, LOW);
	pinMode( SONAR_SENSOR_TX, INPUT);
}

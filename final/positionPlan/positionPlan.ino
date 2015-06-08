#include <ArduinoRobot.h>
#include <SPI.h>
#include <Wire.h>

int eastDistance, northDistance, southDistance, westDistance;
int RAYON_ROBOT = 11;
int H_PLAN = 135;
int L_PLAN = 105;
int cap[4] = {148, 309, 230, 55};

int compass;

void setup() {
  // put your setup code here, to run once:
  Robot.begin();
  Robot.beginTFT();
  Robot.stroke(0, 0, 0);
  Robot.textSize(1);
  activateSensor();
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  Robot.clearScreen();
  int * sensorValues = getSensorsValue();
  printDistances(sensorValues[0], sensorValues[1], sensorValues[2], sensorValues[3]);
  int * coordinates = robotCoordinates(sensorValues[0], sensorValues[1], sensorValues[2], sensorValues[3], Robot.compassRead());
  Robot.text(coordinates[0], 35, 50);
  Robot.text(coordinates[1], 65, 50);
  free(sensorValues);
  free(coordinates);
  delay(500);
}

/**
Returns an array with sonar sensors value 
[0] north
[1] south
[2] east
[3] west
*/
int * getSensorsValue (){
  int * values = new int[4];
  values[0] = sonar_read_mode(M2) * 1.27;
  values[1] = sonar_read_mode(M6) * 1.27;
  values[2] = sonar_read_mode(M0) * 1.27;
  values[3] = sonar_read_mode(M4) * 1.27;
  return values;
}
void printDistances (int northDistance, int southDistance, int eastDistance, int westDistance) {
  Robot.text(westDistance, 2, 80); //Affiche la distance de gauche
  Robot.text(northDistance, 47, 3); // Affiche la distance du haut
  Robot.text(eastDistance, 93, 80); //Affiche la distance de droite
  Robot.text(southDistance, 47, 145); //Affiche la distance du bas  
}


/**
  returns an array of 2 int, the first is x coordinates, the second is y coordiantes.
*/
int * robotCoordinates (int northDistance, int southDistance, int eastDistance, int westDistance, int orientation) {
  int * coordinates = new int[2];
  if(bestOrientation (orientation) == cap[0]){
    if (westDistance < eastDistance) {
      coordinates[0] = westDistance + RAYON_ROBOT;
    }
    else {
      coordinates[0] = L_PLAN - eastDistance - RAYON_ROBOT;
    }
    if (northDistance < southDistance) {
      coordinates[1] = northDistance + RAYON_ROBOT;
    }
    else {
      coordinates[1] = H_PLAN - southDistance - RAYON_ROBOT;
    }
  }
  else if (bestOrientation (orientation) == cap[1]){
    if (westDistance < eastDistance) {
      coordinates[0] = L_PLAN - westDistance - RAYON_ROBOT;
    }
    else {
      coordinates[0] = eastDistance + RAYON_ROBOT;
    }
    if (northDistance < southDistance) {
      coordinates[1] = H_PLAN - northDistance - RAYON_ROBOT;
    }
    else {
      coordinates[1] = southDistance + RAYON_ROBOT;
    }
  }
  else if (bestOrientation (orientation) == cap[2]){
    if (westDistance < eastDistance) {
      coordinates[1] = westDistance + RAYON_ROBOT;
    }
    else {
      coordinates[1] = L_PLAN - eastDistance - RAYON_ROBOT;
    }
    if (northDistance < southDistance) {
      coordinates[0] = northDistance + RAYON_ROBOT;
    }
    else {
      coordinates[0] = H_PLAN - southDistance - RAYON_ROBOT;
    }
  }
  else if (bestOrientation (orientation) == cap[3]){
    if (westDistance < eastDistance) {
      coordinates[1] = L_PLAN - westDistance - RAYON_ROBOT;
    }
    else {
      coordinates[1] = eastDistance + RAYON_ROBOT;
    }
    if (northDistance < southDistance) {
      coordinates[0] = H_PLAN - northDistance - RAYON_ROBOT;
    }
    else {
      coordinates[0] = southDistance + RAYON_ROBOT;
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

#include <ArduinoRobot.h>
#include <SPI.h>
#include <Wire.h>

// Global variables
int MAX_SPEED = 150;
int CORRECTION_SPEED = 100;
int RAYON_ROBOT = 10;
int H_PLAN = 135;
int L_PLAN = 105;
int cap[4] = {172, 286, 220, 110};
int frontSensor;
int ballX = 78;
int ballY = 56;
int goalX = 47;
int goalY = 100;

char aTinyMelody[] = "8eF-FFga4b.a.g.F.8beee-d2e.1-";

void setup() {
  // initialisation
  Robot.begin();
  Robot.beginTFT();
  Robot.stroke(0, 0, 0);
  Robot.textSize(2);
  activateSensor();
  Robot.beginSpeaker();
  delay(2000);
}

void loop() {
  Robot.clearScreen();
  // Go to a point a little top of the ball 
  goTo(ballX, ballY-15);
  // Go bottom to catch the ball 
  Robot.pointTo(cap[1]);
  Robot.motorsWrite(150,150);
  delay(500);
  Robot.motorsStop();
  // Go in front of the goal
  goTo(goalX,goalY);
  Robot.beep(BEEP_DOUBLE);
  delay(1000);
  // Score the goal (Robot enter the goal with the ball)
  Robot.pointTo(cap[1]);
  Robot.motorsWrite(150,150);
  delay(1000);
  Robot.motorsStop();
  // Play a melody to celebrate
  Robot.playMelody(aTinyMelody);
  delay(6000);
    
}
//Go forward until you are at a distance of distanceInCM
void moveUntil(int distanceInCM){
  while (distanceInCM <getFrontValue()) {
    Robot.motorsWrite(MAX_SPEED,MAX_SPEED);
    delay(50);
  }
  Robot.motorsStop();
  delay(100);
}
int getFrontValue (){
  return sonar_read_mode(M2) * 1.27;
}
/**
Robot move to the point at (x,y). 
Robot first move on Y axis then on X axis. When done it checks if the Robot is at the point, if not it does last steps again. 
*/
void goTo(int x, int y) {
  int TOLERANCE = 5;
  int * sensorValues = getSensorsValue();
  int * coordinates = robotCoordinates(sensorValues[0], sensorValues[1], sensorValues[2], sensorValues[3], Robot.compassRead());
  while(abs(coordinates[0] - x) > TOLERANCE+3 || abs(coordinates[1] - y) > TOLERANCE+3){
    if(coordinates[1] - y > TOLERANCE) {
      Robot.pointTo(cap[0]);
      moveUntil(y-RAYON_ROBOT);      
    }
    else if (coordinates[1] - y < -TOLERANCE) {
      Robot.pointTo(cap[1]);
      moveUntil(H_PLAN - y - RAYON_ROBOT);
    }
    delay(1000);
    sensorValues = getSensorsValue();
    free(coordinates);
    coordinates = robotCoordinates(sensorValues[0], sensorValues[1], sensorValues[2], sensorValues[3], Robot.compassRead());
    free(sensorValues);
     if(coordinates[0] - x < -TOLERANCE) {
      Robot.pointTo(cap[2]);
      moveUntil(L_PLAN - x - RAYON_ROBOT);
    }
    else if (coordinates[0] - x > TOLERANCE) {
      Robot.pointTo(cap[3]);
      moveUntil(x - RAYON_ROBOT);
    }
    free(coordinates);
    delay(200);
    sensorValues = getSensorsValue();
    coordinates = robotCoordinates(sensorValues[0], sensorValues[1], sensorValues[2], sensorValues[3], Robot.compassRead());
    
    free(sensorValues);
    delay(1000);    
  }
  return;
}
/**
Returns an array with a stable value for each sensor.
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
/**
Returns the closest axis to compass value.
*/
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

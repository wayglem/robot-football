#include <ArduinoRobot.h>
#include <SPI.h>
#include <Wire.h>

int DISTANCE_FROM_WALL = 33;
int MAX_SPEED = 175;
int CORRECTION_SPEED = 150;
int TOLERANCE_DISTANCE = 5;
int DISTANCE_TO_CUT_CORNER = 60;
int ACCEPTED_WALL_DISTANCE = 60;

void setup() {
  // put your setup code here, to run once:
  Robot.begin();
  Robot.beginTFT();
  Robot.beginSpeaker();
  Robot.stroke(0, 0, 0);
  Robot.textSize(2);
  activateSensor();
  Robot.waitContinue(BUTTON_MIDDLE);
  delay(5000);
  moveUntilWallLeft();
}


void loop() {
  // put your main code here, to run repeatedly:
  Robot.clearScreen();
  parcours1();
}

void parcours1() {
  int eastDistance = sonar_read_mode(M0) * 1.27;
  int northDistance = sonar_read_mode(M2) * 1.27;
  int northWestDistance = sonar_read_mode(M3) * 1.27;
  int westDistance = sonar_read_mode(M4) * 1.27;
  int compass = Robot.compassRead();

  if (northDistance < 20){
    delay(300);
   eastDistance = sonar_read_mode(M0) * 1.27;
   northDistance = sonar_read_mode(M2) * 1.27;
   northWestDistance = sonar_read_mode(M3) * 1.27;
   westDistance = sonar_read_mode(M4) * 1.27;
   compass = Robot.compassRead();
   if (northDistance < 20) {
     if (westDistance > 80){
      Robot.motorsWrite(-255,-255);
      delay(250);
      turnLeft90();
      Robot.motorsWrite(MAX_SPEED, MAX_SPEED);
      delay(1500);
    }
    else {
      Robot.motorsWrite(-255,-255);
      delay(250);
      turnRight90();
      Robot.motorsWrite(MAX_SPEED, MAX_SPEED);
      delay(1500);
    }
   }
  }

  if (northDistance < 90 && westDistance < 80 && eastDistance > 120) {
    turnRight90();
    Robot.motorsWrite(MAX_SPEED, MAX_SPEED);
    delay(200);
  }
  else if(northDistance > 75 && northDistance < 120 && northWestDistance > 85) {   
    turnLeft90();
  }
  else if (westDistance > northWestDistance + 15){
    offsetLeft();
  }
  else {
    followWall(northWestDistance, westDistance);
  }
}


void moveUntilWallLeft () {
  //TODO: add sensor 3 check
  while (sonar_read_mode(M4)*1.27 > ACCEPTED_WALL_DISTANCE) {
    Robot.motorsWrite(MAX_SPEED, MAX_SPEED);
    delay(200);
  }
}

void followWall(int northWestDistance, int westDistance) {
  if (distanceFromLeftWall(northWestDistance, westDistance) > DISTANCE_FROM_WALL + TOLERANCE_DISTANCE) {
      // On est loin du mur de gauche
        chicaneGauche();
    }
  else if (distanceFromLeftWall(northWestDistance, westDistance) < DISTANCE_FROM_WALL - TOLERANCE_DISTANCE) {
      // On est près du mur de gauche
        chicaneDroite();
    }
  else {
      Robot.motorsWrite(MAX_SPEED, MAX_SPEED);
      delay(50);
  }
}
int distanceFromLeftWall(int northWestDistance, int westDistance){
  int distance = westDistance;
  if (northWestDistance < 100) {
    distance = (int) (distance + (northWestDistance * sqrt(2) /2))/2;
  }
  Robot.text(distance, 5,80);
  return distance;
}
void chicaneDroite() {
  Robot.motorsWrite(MAX_SPEED, 0);
  delay(250);
  Robot.motorsWrite(0, MAX_SPEED);
  delay(200);
  Robot.motorsWrite(MAX_SPEED, MAX_SPEED);
}
void chicaneGauche() {
  Robot.motorsWrite(0, MAX_SPEED);
  delay(250);
  Robot.motorsWrite(MAX_SPEED, 0);
  delay(200);
  Robot.motorsWrite(MAX_SPEED, MAX_SPEED);
}

void turnLeft90() {
    Robot.motorsWrite(0, 255);
    delay(300);
    Robot.motorsWrite(MAX_SPEED, MAX_SPEED);
}

void turnRight90() {
    Robot.motorsWrite(255, 0);
    delay(250);
    Robot.motorsWrite(MAX_SPEED, MAX_SPEED);
}

void offsetLeft() {
    Robot.motorsWrite(255,200);
    delay(200);
    Robot.motorsWrite(MAX_SPEED, MAX_SPEED);
}

// Lecture du capteur MaxSonar, 
// on considère que la valeur est bonne si deux lectures successives sont très proches
int sonar_read_mode( int pin) {

   int sum, prevprev, prev, last;
   prev = last = 0;
   do {
      if ( prev) prevprev = prev; else { prevprev = Robot.analogRead(pin); delay(10); }
      if ( last) prev = last; else { prev = Robot.analogRead(pin); delay(10); }
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

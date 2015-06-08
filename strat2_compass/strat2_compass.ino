#include <ArduinoRobot.h>
#include <SPI.h>
#include <Wire.h>
  int direction;
  int cap[6] = {321, 246, 180, 86, 165, 64};
  int MAX_SPEED = 255;
  int CORRECTION_SPEED = 200;

void setup() {
  // put your setup code here, to run once:
  Robot.begin();
  Robot.beginTFT();
  Robot.stroke(0, 0, 0);
  Robot.textSize(2);
  activateSensor();
  Robot.waitContinue(BUTTON_MIDDLE);  
}

void loop() {
  // put your main code here, to run repeatedly:
  Robot.clearScreen();
   int distanceNorth = sonar_read_mode(M2) * 1.27;
   
   direction = bestDirection(Robot.compassRead());
   avancerSuivantDirection(direction);
   //Robot.motorsStop();
   Robot.motorsWrite(0,0);
   delay(100);
   
   int distanceEast = sonar_read_mode(M0) * 1.27;
   int distanceWest = sonar_read_mode(M4) * 1.27;
   turnOnStop(Robot.compassRead(),distanceEast, distanceWest);
}

int bestDirection (int compass) {
  int chosenDirection = cap[0];
  int min = abs(compass - cap[0]);
  Robot.text(compass, 5,5);
  for (int i=1;i<6;i++){
    if (abs(compass - cap[i])<min){
      min = abs(compass - cap[i]);
      chosenDirection = cap[i];
    }
  }
  Robot.text(chosenDirection, 50,5);
  return chosenDirection;
}

void avancerSuivantDirection(int direction){
  Robot.clearScreen();
  int TOLERANCE_COMPASS = 5;
  int distanceNorth = (int) sonar_read_mode(M2) * 1.27;
  Robot.text(distanceNorth, 80,5);
  while(distanceNorth > 70){
    distanceNorth = (int) sonar_read_mode(M2) * 1.27;
    Robot.text(distanceNorth, 50,60);
    if (Robot.compassRead() > (direction + TOLERANCE_COMPASS)%360) {
      Robot.motorsWrite(CORRECTION_SPEED,MAX_SPEED);
    }
    else if (Robot.compassRead() < (direction - TOLERANCE_COMPASS)%360) {
      Robot.motorsWrite(MAX_SPEED, CORRECTION_SPEED);
    }
    else {
      Robot.motorsWrite(MAX_SPEED,MAX_SPEED);
    }
    delay(50);
  }
}
void turnOnStop (int compass, int east, int west) {
//void turnOnStop (int compass) {
  if (east < 80 && west < 80){
    if (east < west) {
      Robot.motorsWrite(100, MAX_SPEED);
      delay(100);
      Robot.motorsWrite(0,0);
      return;
    }
    else {
      Robot.motorsWrite(MAX_SPEED,100);
      delay(100);
      Robot.motorsWrite(0,0);
      return;
    }
  }
  Robot.text("Je tourne !", 5, 5);
  int direction = bestDirection(compass);
  int turnDirection = 500;
  for (int i=0; i<5; i++) {
    if (direction = cap[i]) {
      turnDirection = cap[i+1];
    }
  }
  if (turnDirection == 500) {
    turnDirection = cap[0];
  }
  Robot.pointTo(turnDirection);
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

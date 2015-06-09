#include <ArduinoRobot.h>
#include <SPI.h>
#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
   Robot.begin();
   Serial.begin(9600);
  activateSensor();
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(sonar_read_mode(M2) * 1.27);
  Serial.print(", ");
  delay(100);
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

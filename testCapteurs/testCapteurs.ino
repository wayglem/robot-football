http://stackoverflow.com/questions/7670561/how-to-get-htaccess-to-work-on-mamp
il y a quelques secondes

#include <ArduinoRobot.h>
#include <TFT.h>
#include <SPI.h>
#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Robot.begin();
  Robot.beginTFT();
}

void loop() {
  int tk0 = Robot.analogRead(TK0);
  int tk2 = Robot.analogRead(TK2);
  int tk4 = Robot.analogRead(TK4);
  int tk6 = Robot.analogRead(TK6);

  String debug = "Debug Capteurs";
  debug += tk0;
  debug += "   -   ";
  debug += tk2;
  debug += "   -   ";
  debug += tk4;
  debug += "   -   ";
  debug += tk6;
  // put your main code here, to run repeatedly:
  Robot.text(debug);
  delay(100);
}

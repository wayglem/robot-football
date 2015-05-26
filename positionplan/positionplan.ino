#include <ArduinoRobot.h>
#include <SPI.h>
#include <Wire.h>

int frontSensor = M2;
int leftSensor  = M4;
int rightSensor = M0;
int rearSensor  = M6;

int rayonRobot = 11;
int hPlan = 135;
int lPlan = 103;
int ml = 12; // marge largeur
int mh = 12; // marge hauteur
int TOLERANCE_BOUSSOLE = 20;
int haut = 158;
int gauche = 78;
int bas = 283;
int droite = 218;

void setup() {
  // put your setup code here, to run once:
  Robot.begin();
  Robot.beginTFT();
  Serial.begin(9600);
  Robot.stroke(0, 0, 0);
/*
  Robot.text("Compass setting", 5, 5);
  Robot.waitContinue(BUTTON_MIDDLE);
  a0 =  (int) Robot.compassRead();
  Robot.text(a0, 40, 70);
  delay(1000);
  Robot.clearScreen();
  */
  Robot.waitContinue(BUTTON_MIDDLE);
}

void loop() {
  // put your main code here, to run repeatedly:
  Robot.clearScreen();
  Robot.rect(ml, mh, lPlan, hPlan);
//  Robot.pointTo(a0);
//  int * position = getPosition();
  int * position = getPositionField();
//  Robot.circle(position[0], position[1], 4);
  Robot.textSize(2);
  Robot.text(position[0], 5, 80);
  Robot.text(position[1], 55, 5);
  
  Robot.text((int)Robot.compassRead(), 47, 70); //Affiche la boussole

  free(position);
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
int * getPosition () {
  int * distances = getOlympicDistance();
  int * position = new int [2];
  // Si le capteur droit est plus proche d'un mur, calculer avec ce capteur
  if (distances[0] < distances[2]) {
    position[0] = lPlan + ml - (distances[0] + rayonRobot);
  }
  else {
    position[0] = distances[2] + rayonRobot + ml;
  }
  // Si le capteur bas est plus proche d'un mur, calculer avec ce capteur
  if (distances[3] < distances[1]) {
    position[1] = hPlan + mh - (distances[3] + rayonRobot);
  }
  else {
    position[1] = distances[1] + rayonRobot + mh;
  }
  Robot.text(distances[2] + rayonRobot, 2, 80); //Affiche la distance de gauche
  Robot.text(distances[1] + rayonRobot, 47, 3); // Affiche la distance du haut
  Robot.text(distances[0] + rayonRobot, 93, 80); //Affiche la distance de droite
  Robot.text(distances[3] + rayonRobot, 47, 145); //Affiche la distance du bas

  free(distances);
  return position;
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
//Retourne un tableau correspondant au coordonnees du centre du robot sur le terrain
// [0]=x
// [1] = y
int * getPositionField () {
  int * distances = getOlympicDistance();
  int orientation = (int) Robot.compassRead();
  int * position = new int [2];
  Robot.clearScreen();
  if (orientation < (haut + TOLERANCE_BOUSSOLE) % 360 && orientation > (haut - TOLERANCE_BOUSSOLE) % 360){
    // Si le capteur droit est plus proche d'un mur, calculer avec ce capteur
    Robot.text("haut", 5, 5);
    if (distances[0] < distances[2]) {
      position[0] = lPlan - (distances[0] + rayonRobot);
    }else {
      position[0] = distances[2] + rayonRobot;
    }
    // Si le capteur bas est plus proche d'un mur, calculer avec ce capteur
    if (distances[3] < distances[1]) {
      position[1] = hPlan - (distances[3] + rayonRobot);
    }else {
      position[1] = distances[1] + rayonRobot;
    }
  }
  //Si le robot est tourne vers la droite
  else if (orientation < (droite + TOLERANCE_BOUSSOLE) % 360 && orientation > (droite - TOLERANCE_BOUSSOLE) % 360){
    Robot.text("droite", 5, 5);
    if (distances[1] < distances[3]) {
      position[0] = lPlan - (distances[1] + rayonRobot);
    }else {
      position[0] = distances[3] + rayonRobot;
    }
    if (distances[0] < distances[2]) {
      position[1] = distances[0] + rayonRobot;
    }else {
      position[1] = hPlan - (distances[2] + rayonRobot);
    }   
  }
  //Si le robot est tourne vers la gauche
  else if (orientation < (gauche + TOLERANCE_BOUSSOLE) % 360 && orientation > (gauche - TOLERANCE_BOUSSOLE) % 360){
    Robot.text("gauche", 5, 5);
    if (distances[1] < distances[3]) {
      position[0] = distances[1] + rayonRobot;
    }else {
      position[0] = lPlan - (distances[3] + rayonRobot);
    }
    if (distances[0] < distances[2]) {
      position[1] = distances[0] + rayonRobot;
    }else {
      position[1] = hPlan - (distances[2] + rayonRobot);
    } 
  }
  //Si le robot est tourne vers le bas
  else if (orientation < (bas + TOLERANCE_BOUSSOLE) % 360 && orientation > (bas - TOLERANCE_BOUSSOLE) % 360){  
    Robot.text("bas", 5, 5);
    if (distances[0] < distances[2]) {
      position[0] = distances[0] + rayonRobot;
    }else {
      position[0] = lPlan - (distances[2] + rayonRobot);
    }
    if (distances[1] < distances[3]) {
      position[1] = hPlan - (distances[1] + rayonRobot);
    }else {
      position[1] = distances[3];
    }
  }
  else {
    Robot.text("autre", 5, 5);
    Robot.text(orientation, 40, 70);
    Robot.pointTo(haut);
    getPositionField();
  }
  free(distances);
  return position;
}

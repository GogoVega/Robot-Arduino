// MIT License

// Copyright (c) 2022 Gauthier Dandele

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <Arduino.h>

// Buzzer
extern const int BuzzerPin = 8;

// Definition des sorties Moteurs
#define borneENA 2  // Vitesse Motor A
#define borneIN1 30  // Marche avant
#define borneIN2 31  // Marche arrière
#define borneIN3 32  // Marche avant
#define borneIN4 33  // Marche arrière
#define borneENB 3  // Vitesse Motor B
#define borneENC 4  // Vitesse Motor C
#define borneIN5 34  // Marche avant
#define borneIN6 35  // Marche arrière
#define borneIN7 36  // Marche avant
#define borneIN8 37  // Marche arrière
#define borneEND 5  // Vitesse Motor D

extern void WriteVitesseRobot(int VG, int VD) {
  struct SensG {
    byte IN1;
    byte IN2;
    int ENA;
  } G;

  struct SensD {
    byte IN3;
    byte IN4;
    int ENB;
  } D;

  // Avant - Arrière - Arrêt
  if (VG >= 40) {
    G = {HIGH, LOW, VG};
  } else if (VG <= -40) {
    G = {LOW, HIGH, abs(VG)};
  } else {
    G = {LOW, LOW, 0};
  }
  if (VD >= 40) {
    D = {HIGH, LOW, VD};
  } else if (VD <= -40) {
    D = {LOW, HIGH, abs(VD)};
  } else {
    D = {LOW, LOW, 0};
  }

  // Outputs
  digitalWrite(borneIN1, G.IN1);
  digitalWrite(borneIN2, G.IN2);
  analogWrite(borneENA, G.ENA);

  digitalWrite(borneIN3, D.IN3);
  digitalWrite(borneIN4, D.IN4);
  analogWrite(borneENB, D.ENB);
}

extern void ReadVitesseRobot(int VRX, int VRY) {
  int VG, VD;

  // Gauche - Doite
  if (VRY > 100) {
    VG = max(VRX - 50, -255);
    VD = min(VRX + 50, 255);
  } else if (VRY < -100) {
    VG = min(VRX + 50, 255);
    VD = max(VRX - 50, -255);
  } else {
    VG = VRX;
    VD = VRX;
  }

  // Buzzer marche arrière
  if (VG <= -40 && VD <= -40) {
    tone(BuzzerPin, 200, 500);
  }

  WriteVitesseRobot(VG, VD);
}

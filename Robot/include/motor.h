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

#ifndef __MOTOR_H
#define __MOTOR_H

#include <type.h>

void WriteVitesseRobot(int VG, int VD) {
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
  if (VG >= 50) {
    G = {HIGH, LOW, VG};
  } else if (VG <= -50) {
    G = {LOW, HIGH, abs(VG)};
  } else {
    G = {LOW, LOW, 0};
  }
  if (VD >= 50) {
    D = {HIGH, LOW, VD};
  } else if (VD <= -50) {
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

void ReadVitesseRobot(int VRX, int VRY) {
  int VG, VD;

  // Gauche - Doite
  if (VRY > 100) {
    VG = max(VRX - 60, -255);
    VD = min(VRX + 60, 255);
  } else if (VRY < -100) {
    VG = min(VRX + 60, 255);
    VD = max(VRX - 60, -255);
  } else {
    VG = VRX;
    VD = VRX;
  }

  // Buzzer marche arrière
  if (VG <= -50 && VD <= -50) {
    tone(BuzzerPin, 200, 500);
  }

  WriteVitesseRobot(VG, VD);
}

#endif

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
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SerialTransfer.h>
#include <Ultrasonic.h>
#include <Wire.h>

SerialTransfer RecTransfer;

Ultrasonic sonar(12, 13);

#define borneENA A0  // Vitesse Motor A
#define borneIN1 20  // Marche avant
#define borneIN2 21  // Marche arrière
#define borneIN3 22  // Marche avant
#define borneIN4 23  // Marche arrière
#define borneENB A1  // Vitesse Motor B
#define borneENC A2  // Vitesse Motor C
#define borneIN5 24  // Marche avant
#define borneIN6 25  // Marche arrière
#define borneIN7 26  // Marche avant
#define borneIN8 27  // Marche arrière
#define borneEND A3  // Vitesse Motor D

// LCD
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Buzzer
const int BuzzerPin = 22;

struct STRUCT {
  int AxeX_Robot;
  int AxeY_Robot;
  int BP_OP;
  int BP_UD;
  int RFID_State;
  int Distance;
} data;

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

  // Output
  digitalWrite(borneIN1, G.IN1);
  digitalWrite(borneIN2, G.IN2);
  analogWrite(borneENA, G.ENA);

  digitalWrite(borneIN3, D.IN3);
  digitalWrite(borneIN4, D.IN4);
  analogWrite(borneENB, D.ENB);
}

void ReadVitesseRobot() {
  int VRX = data.AxeX_Robot;
  int VRY = data.AxeY_Robot;
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

  if (VG <= -50 && VD <= -50) {
    Buzzer(HIGH);
  } else {
    Buzzer(LOW);
  }

  WriteVitesseRobot(VG, VD);
}

void Sonar() {
  float Distance = sonar.read();

  if (Distance < 5) {
    Buzzer(HIGH);
  } else {
    Buzzer(LOW);
  }
}

void Buzzer(byte Value) {
  digitalWrite(BuzzerPin, Value);
}

void setup() {
  Serial1.begin(38400);
  RecTransfer.begin(Serial1);

  // LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DEMARRAGE");
}

void loop() {
  uint16_t recSize = 0;
  recSize = RecTransfer.txObj(data, recSize);
  RecTransfer.sendData(recSize);

  if (RecTransfer.available()) {
    uint16_t recSize = 0;
    recSize = RecTransfer.rxObj(data, recSize);
  }
}

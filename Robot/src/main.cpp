// MIT License

// Copyright (c) 2022 Gauthier Dandele

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SerialTransfer.h>
#include <Wire.h>
#include <SPI.h>

SerialTransfer RecTransfer;

const int borneENA = A0;       // Vitesse Motor A
const int borneIN1 = 20;       // Marche avant
const int borneIN2 = 21;       // Marche arrière
const int borneIN3 = 22;       // Marche avant
const int borneIN4 = 23;       // Marche arrière
const int borneENB = A1;       // Vitesse Motor B
const int borneENC = A2;       // Vitesse Motor C
const int borneIN5 = 24;       // Marche avant
const int borneIN6 = 25;       // Marche arrière
const int borneIN7 = 26;       // Marche avant
const int borneIN8 = 27;       // Marche arrière
const int borneEND = A3;       // Vitesse Motor D

// LCD
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

struct STRUCT {
  int AxeX_Robot;
  int AxeY_Robot;
  int BP_OP;
  int BP_UD;
  int RFID_State;
  int Distance;
} data;

void WriteVitesseRobot(int v1, int v2) {
  if (v1 >= 50) {
    digitalWrite(borneIN1, HIGH);
    digitalWrite(borneIN2, LOW);
    analogWrite(borneENA, v1);
  } else if (v1 <= -50) {
    digitalWrite(borneIN1, LOW);
    digitalWrite(borneIN2, HIGH);
    analogWrite(borneENA, abs(v1));
  } else {
    digitalWrite(borneIN1, LOW);
    digitalWrite(borneIN2, LOW);
    analogWrite(borneENA, 0);
  }
  if (v2 >= 50) {
    digitalWrite(borneIN3, HIGH);
    digitalWrite(borneIN4, LOW);
    analogWrite(borneENB, v2);
  } else if (v2 <= -50) {
    digitalWrite(borneIN3, LOW);
    digitalWrite(borneIN3, HIGH);
    analogWrite(borneENB, abs(v2));
  } else {
    digitalWrite(borneIN3, LOW);
    digitalWrite(borneIN4, LOW);
    analogWrite(borneENB, 0);
  }
}

void ReadVitesseRobot(){
  int VRX = data.AxeX_Robot;
  int VRY = data.AxeY_Robot;
  int vitesseG, vitesseD;

  // Gauche - Doite
  if (VRY > 100){
    vitesseG = max(VRX - 50, -255);
    vitesseD = min(VRX + 50, 255);
  } else if (VRY < -100) {
    vitesseG = min(VRX + 50, 255);
    vitesseD = max(VRX - 50, -255);
  } else {
    vitesseG = VRX;
    vitesseD = VRX;
  }
  WriteVitesseRobot(vitesseG, vitesseD);
}

void setup() {
  Serial1.begin(38400);
  RecTransfer.begin(Serial1);

  // LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("DEMARRAGE");
}

void loop() {
  uint16_t recSize = 0;
  recSize = RecTransfer.txObj(data, recSize);
  RecTransfer.sendData(recSize);

  if(RecTransfer.available()) {
    uint16_t recSize = 0;
    recSize = RecTransfer.rxObj(data, recSize);
  }
}

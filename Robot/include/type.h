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

#ifndef __TYPE_H
#define __TYPE_H

#include <SerialTransfer.h>

SerialTransfer RecTransfer;

// Automatic Mode
#define AutoPin 45

// Batterie
#define BatteryPin A0

// Bluethooth State
#define BluethoothPin 44

// Buzzer
#define BuzzerPin 8

// LCD
extern const int rs = 22, en = 23, d4 = 24, d5 = 25, d6 = 26, d7 = 27;

// LEDs
#define LEDFrontPin 46    // LED Avant
#define LEDBackPin 47     // LED Arrière
#define LEDReversePin 48  // LED Marche arrière

// Tempos (100ms)
int Flag = 0;

// Définition des sorties Moteurs
#define borneENA 2   // Vitesse Motor A
#define borneIN1 30  // Marche avant
#define borneIN2 31  // Marche arrière
#define borneIN3 32  // Marche avant
#define borneIN4 33  // Marche arrière
#define borneENB 3   // Vitesse Motor B
#define borneENC 4   // Vitesse Motor C
#define borneIN5 34  // Marche avant
#define borneIN6 35  // Marche arrière
#define borneIN7 36  // Marche avant
#define borneIN8 37  // Marche arrière
#define borneEND 5   // Vitesse Motor D

// RFID
#define SDA_RFID 6  // Pin SDA
#define RST_RFID 7  // Pin RST
// MISO 50, MOSI 51, SCK (SCL) 52

// Servomoteurs
#define ServoPin1 9
#define ServoPin2 10

// Sonar
#define Trig 40
#define Echo 41

// Structure des données Bluethooth
struct STRUCT {
  int Axe_X;
  int Axe_Y;
  int BP_OC;
  int BP_UD;
  byte Code[4];
  int RFID_State;
  int Distance;
} data;

#endif

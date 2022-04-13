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

#include <led.h>
#include <type.h>

#define minSpeed 60  // Vitesse minimale de démarrage du moteur
#define limitSpeed (minSpeed - 10)  // Vitesse minimale (précision)

struct Sense {
  byte DO1;
  byte DO2;
  int AO;
} dataMotor;

// Avant - Arrière - Arrêt
Sense SenseRotation(int speed) {
  if (speed >= limitSpeed) {
    return {HIGH, LOW, speed};
  } else if (speed <= -limitSpeed) {
    return {LOW, HIGH, abs(speed)};
  } else {
    return {LOW, LOW, 0};
  }
}

void WriteSpeeds(int leftSpeed, int rightSpeed) {
  Sense left = SenseRotation(leftSpeed);
  digitalWrite(borneIN1, left.DO1);
  digitalWrite(borneIN2, left.DO2);

  Sense right = SenseRotation(rightSpeed);
  digitalWrite(borneIN3, right.DO1);
  digitalWrite(borneIN4, right.DO2);

  analogWrite(borneENA, left.AO);
  analogWrite(borneENB, right.AO);

  // Si marche arrière
  if (leftSpeed <= -limitSpeed && rightSpeed <= -limitSpeed) {
    // Feux marche arrière
    digitalWrite(LEDReversePin, HIGH);

    // Buzzer marche arrière
    if (Flag == 1) {
      tone(BuzzerPin, 200, 500);
    }
  } else {
    // Feux marche arrière OFF
    digitalWrite(LEDReversePin, LOW);
  }
}

void ReadSpeeds(int Speed_X, int Speed_Y) {
  int leftSpeed = 0, rightSpeed = 0;

  // Gauche - Droite
  if (Speed_Y > 100) {
    leftSpeed = max(Speed_X - minSpeed, -255);
    rightSpeed = min(Speed_X + minSpeed, 255);
  } else if (Speed_Y < -100) {
    leftSpeed = min(Speed_X + minSpeed, 255);
    rightSpeed = max(Speed_X - minSpeed, -255);
  } else {
    leftSpeed = Speed_X;
    rightSpeed = Speed_X;
  }

  WriteSpeeds(leftSpeed, rightSpeed);
}

#endif

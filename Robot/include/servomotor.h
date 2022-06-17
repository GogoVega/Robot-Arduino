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

#ifndef __SERVOMOTOR_H
#define __SERVOMOTOR_H

#include <Servo.h>
#include <type.h>

Servo servo_OC;
Servo servo_UD;

// Renvoie l'angle voulu
uint8_t servoWrite(uint8_t angles, uint8_t state) {
  if (state == 1 && angles < 180) {
    angles++;
  } else if (state == 2 && angles > 0) {
    angles--;
  }

  return angles;
}

// Gestion servomoteurs
void servoGo() {
  static uint8_t angles[2] = {0, 0};

  angles[0] = servoWrite(angles[0], data.BP_OC);
  angles[1] = servoWrite(angles[1], data.BP_UD);

  servo_OC.write(angles[0]);
  servo_UD.write(angles[1]);
}

#endif

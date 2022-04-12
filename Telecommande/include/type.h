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

SerialTransfer SendTransfer;

// Batterie
#define BatteryPin A2

// Bluethooth State
#define BluethoothPin 6

// Bouton Poussoir Pince
#define OpenPince 2   // Ouvrir Pince
#define ClosePince 3  // Fermer Pince
#define UpPince 4     // Monter Pince
#define DownPince 5   // Descendre Pince

// Joysticks Robot
#define AxeX A0
#define AxeY A1

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

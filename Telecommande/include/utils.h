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

#ifndef __UTILS_H
#define __UTILS_H

#include <type.h>

// Mapping value for -255 to 255
int JoystickValue(int pin) {
  int value = map(analogRead(pin), 0, 1023, -255, 255);
  return value;
}

// CMD Ouverture-Fermeture de la Pince
int EtatBP_OC() {
  int BP1 = digitalRead(OpenPince);
  int BP2 = digitalRead(ClosePince);
  int Read = data.BP_OC;

  // Open - Close
  if (BP1 && !BP2) {
    return 1;
  } else if (BP2 && !BP1) {
    return 0;
  } else {
    return Read;
  }
}

// CMD Moter-Descendre la pince
int EtatBP_UD() {
  int BP3 = digitalRead(UpPince);
  int BP4 = digitalRead(DownPince);

  // Monter - Descendre - Arrêt
  if (BP3 && !BP4) {
    return 1;
  } else if (BP4 && !BP3) {
    return 2;
  } else {
    return 0;
  }
}

#endif

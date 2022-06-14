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
#define JoystickValue(pin) (map(analogRead(pin), 0, 1023, -255, 255))

// CMD Moter-Descendre la pince
int EtatBP(int pin_1, int pin_2) {
  const int BP1 = digitalRead(pin_1);
  const int BP2 = digitalRead(pin_2);

  // Monter - Descendre - Arrêt
  // Fermer - Ouvrir - Arrêt
  if (BP1 && !BP2) {
    return 1;
  } else if (BP2 && !BP1) {
    return 2;
  }

  return 0;
}

#endif

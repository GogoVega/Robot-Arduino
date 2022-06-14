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

#ifndef __LED_H
#define __LED_H

#include <Adafruit_NeoPixel.h>
#include <type.h>

// LEDs State
#define AUTO_MODE 10
#define DECONNECTE 8

Adafruit_NeoPixel strip(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

// Alerte lumineuse
void Alert(uint32_t color) {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, color);
  }

  // 250ms
  if (Flag == 1 || Flag == 11) {
    strip.show();
  } else if (Flag == 6 || Flag == 16) {
    strip.clear();
    strip.show();
  }
}

// Clignotement
int Blinking(uint32_t color, int num, int actual) {
  static int sens = 1;

  // INIT
  if (!actual) {
    strip.clear();
    strip.show();
    sens = 1;
  }

  if (sens == 1) {
    if (actual < num) {
      strip.setPixelColor(actual, color);
      strip.show();
      actual++;
    } else {
      sens = -1;
    }
  }
  if (sens == -1) {
    if ((actual - 1) >= 0) {
      strip.setPixelColor(actual, strip.Color(0, 0, 0));
      strip.show();
      actual--;
    } else {
      sens = 1;
    }
  }

  return actual;
}

// Feux marche arrière
void Backlights(boolean newState) {
  static boolean oldState;
  uint32_t color;

  if (newState != oldState) {
    switch (newState) {
      case 0:
        color = strip.Color(255, 255, 255);
        break;
      case 1:
        color = strip.Color(0, 0, 0);
        break;
    }

    strip.setPixelColor(1, color);
    strip.setPixelColor(3, color);
    oldState = newState;
  }
}

// Feux de croisement
void Headlights(boolean newState) {
  const int pixelAddress[4] = {0, 4, 10, 14};
  static boolean oldState;

  if (newState) {
    if (newState != oldState) {
      strip.clear();
      strip.show();

      for (int i = 0; i < 4; i++) {
        uint32_t color;

        if (i < 2)
          color = strip.Color(255, 255, 255);
        else
          color = strip.Color(255, 0, 0);

        strip.setPixelColor(pixelAddress[i], color);
      }

      strip.show();
      oldState = true;
    }
  } else {
    oldState = false;
  }
}

// Eclairage démarrage du robot
void StartUp(uint32_t color, int wait) {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

// Etat des LEDs
int BlinkState() {
  if (digitalRead(AutoPin)) {
    return AUTO_MODE;
  } else if (!digitalRead(BluethoothPin)) {
    return DECONNECTE;
  } else {
    return data.RFID_State;
  }
}

// Gestion du strip LEDs
void Blink() {
  static int actualPixel = 0;
  static int oldState;
  int newState = BlinkState();

  // INIT
  if (newState != oldState) {
    if (newState != 1 && newState != 10)
      Headlights(false);
    actualPixel = 0;
    oldState = newState;
  }

  switch (newState) {
    case 0:
      actualPixel = Blinking(strip.Color(0, 0, 255), NUMPIXELS, actualPixel);
      break;
    case 1:
    case 10:
      Headlights(true);
      break;
    case 2:
      Alert(strip.Color(255, 165, 0));  // Orange
      break;
    case 3:
      Alert(strip.Color(0, 255, 0));  // Vert
      break;
    case 4:
    case 5:
      Alert(strip.Color(255, 0, 0));  // Rouge
      break;
    case 8:
      actualPixel = Blinking(strip.Color(255, 165, 0), NUMPIXELS, actualPixel);
      break;
  }
}

#endif

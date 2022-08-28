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
  }
}

// Clignotement
int Blinking(uint32_t color, int num, int actual) {
  static int sens = 1;

  // INIT
  if (!actual) {
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


```c
#include <Adafruit_NeoPixel.h>
#include <type.h>

Adafruit_NeoPixel strip(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

// Alerte lumineuse
void Alert(uint32_t color) {
  for(int i = 0; i < NUMPIXELS; i++) {
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
  if (actual == 0) {
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
  } if (sens == -1) {
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
void Backlights(boolean state) {
  static boolean oldState;
  uint32_t color;

  if (state != oldState) {
    switch (state) {
      case 0:
        color = strip.Color(255, 255, 255);
        break;
      case 1:
        color = strip.Color(0, 0, 0);
        break;
    }

    strip.setPixelColor(1, color);
    strip.setPixelColor(3, color);
    oldState = state;
  }
}

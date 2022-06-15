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

// Feux de croisement
void Headlights() {
  const int address[4] = {0, 4, 10, 14};
  static boolean oldState;

  if (!oldState) {
    if (digitalRead(AutoPin) || data.RFID_State == 1) {
      strip.clear();

      for(int i = 0; i < 4; i++) {
        uint32_t color;

        if (i < 2)
          color = strip.Color(255, 255, 255);
        else
          color = strip.Color(255, 0, 0);

        strip.setPixelColor(address[i], color);
      }

      strip.show();
      oldState = true;
    }
  } else {
    if (data.RFID_State != 1) {
      oldState = false;
    }
  }
}


// Eclairage démarrage du robot
void StartUp(uint32_t color, int wait) {
  for(int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

// Gestion du strip LEDs
void Blink() {
  static int actualPixel = 0;

  Headlights();

  if (!digitalRead(AutoPin)) {
    if (!digitalRead(BluethoothPin)) {
      actualPixel = Blinking(strip.Color(255, 165, 0), NUMPIXELS, actualPixel);
    } else {
      switch (data.RFID_State) {
        case 0:
          actualPixel = Blinking(strip.Color(0, 0, 255), NUMPIXELS, actualPixel);
          break;
        case 2:
          Alert(strip.Color(255, 165, 0)); // Orange
          break;
        case 3:
          Alert(strip.Color(0, 255, 0));   // Vert
          break;
        case 4:
        case 5:
          Alert(strip.Color(255, 0, 0));   // Rouge
          break;
      }

      if (data.RFID_State) {
        actualPixel = 0;
      }
    }
  } else {
    actualPixel = 0;
  }
}

```

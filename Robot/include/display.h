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

#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <LiquidCrystal.h>
#include <Wire.h>
#include <type.h>

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Valeurs Batterie
void Batterie(double* Values) {
  const uint16_t pinRead = analogRead(BatteryPin);

  Values[0] = (float) (map(pinRead, 0, 1023, 0, 500)) / 100;
  Values[1] = (float) (map(pinRead, 0, 1023, 0, 10000)) / 100;
}

// Valeurs Bluethooth et RFID
String LCDState() {
  if (digitalRead(AutoPin)) {
    return F("   MODE  AUTO");
  } else if (!digitalRead(BluethoothPin)) {
    return F("  Connexion...");
  } else {
    switch (data.RFID_State) {
      case 0:
        return F("   Verrouille");
      case 1:
        return F(" Deverrouille");
      case 2:
        return F("Badgez nvl carte");
      case 3:
        return F("Carte ajoutee");
      case 4:
        return F("Carte refusee!");
      case 5:
        return F("Aucune carte!");
      case 9:
        return F("ERROR FLASH!!!");
    }
  }

  return F("ERROR STATE");
}

// Vérifie si changement
boolean CheckChange(String* oldDisplay, String* newDisplay) {
  boolean change = 0;

  if (newDisplay[0] != oldDisplay[0]) {
    oldDisplay[0] = newDisplay[0];
    change = 1;
  }
  if (newDisplay[1] != oldDisplay[1]) {
    oldDisplay[1] = newDisplay[1];
    change = 1;
  }
  if (newDisplay[2] != oldDisplay[2]) {
    oldDisplay[2] = newDisplay[2];
    change = 1;
  }

  return change;
}

// Update LCD display
void Update(String* newDisplay) {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(newDisplay[0]);

  lcd.setCursor(0, 1);
  lcd.print(newDisplay[1]);

  lcd.setCursor(11, 1);
  lcd.print(newDisplay[2]);
}

// Gestion du LCD
void Display() {
  static String oldDisplay[3] = {}, newDisplay[3] = {};

  newDisplay[0] = LCDState();

  // Chaque seconde
  if (Flag == 1) {
    double Values[2] = {};
    Batterie(Values);
    newDisplay[1] = String(Values[1]) + "%";
    newDisplay[2] = String(Values[0]) + "V";
  }

  // Si changement => Update LCD
  if (CheckChange(oldDisplay, newDisplay)) {
    Update(newDisplay);
  }
}

#endif

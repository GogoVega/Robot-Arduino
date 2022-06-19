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

#include <SSD1306AsciiWire.h>
#include <type.h>

SSD1306AsciiWire oled;

// Valeurs Batterie
void Batterie(double* Values) {
  const uint16_t pinRead = analogRead(BatteryPin);

  Values[0] = (float)(map(pinRead, 0, 1023, 0, 500)) / 100;
  Values[1] = (float)(map(pinRead, 0, 1023, 0, 10000)) / 100;
}

// Etat liaison Bluethooth
String Bluethooth() {
  if (digitalRead(BluethoothPin))
    return F("  Connecte  ");

  return F("Connexion...");
}

// Etat RFID
String RFID_State() {
  switch (data.RFID_State) {
    case 0:
      return F("Robot: Verrouille     ");
    case 1:
      return F("Robot: Deverrouille   ");
    case 2:
      return F("Badgez nouvelle carte ");
    case 3:
      return F("Nouvelle carte ajoutee");
    case 4:
      return F("Carte refusee!!!      ");
    case 5:
      return F("Aucune carte ajoutee! ");
    case 9:
      return F("  !!! ERROR FLASH !!! ");
  }

  return F("    ERROR DISPLAY     ");
}

// Etat Pince et Distance
String DisplayState() {
  if (data.Distance > 20 || data.Distance == 0) {
    if (data.BP_OC == 1) {
      return F("Pince: Ouverture");
    } else if (data.BP_OC == 2) {
      return F("Pince: Fermeture");
    }

    return F("Pince: Arret");
  }

  return "Distance: " + String(data.Distance) + " cm";
}

// Gestion de l'OLED
void Display() {
  static uint8_t Flag = 0;

  if (++Flag == 20)
    Flag = 0;

  oled.setCursor(30, 0);
  oled.print(Bluethooth());

  oled.setCursor(0, 3);
  oled.print(DisplayState());

  oled.setCursor(0, 5);
  oled.print(RFID_State());

  if (Flag == 1) {
    double Values[2] = {};
    Batterie(Values);

    oled.setCursor(0, 7);
    oled.print(String(Values[0]) + "%");

    oled.setCursor(98, 7);
    oled.print(String(Values[1]) + "V");
  }
}

#endif

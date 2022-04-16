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

#include <GyverOLED.h>
#include <type.h>

GyverOLED<SSH1106_128x64> oled(0x3C);

// Valeurs Batterie
double* Batterie() {
  static double Output[2] = {};
  const float Tension = map(analogRead(BatteryPin), 0, 1023, 0, 500);
  const float ChargeLevel = map(Tension, 0, 500, 0, 10000);
  Output[0] = ChargeLevel / 100;
  Output[1] = Tension / 100;

  return Output;
}

// Etat liaison Bluethooth
String Bluethooth() {
  if (digitalRead(BluethoothPin)) {
    return "  Connecte";
  } else {
    return "Connexion...";
  }
}

// Etat RFID
String RFID_State() {
  switch (data.RFID_State) {
    case 0:
      return "Robot: Verrouille";
    case 1:
      return "Robot: Deverrouille";
    case 2:
      return "Badgez nouvelle carte";
    case 3:
      return "Nouvelle carte ajoutee";
    case 4:
      return "Carte refusee!";
    case 5:
      return "Aucune carte ajoutee!";
  }
}

// Etat Pince et Distance
String DisplayState() {
  if (data.Distance > 20 || data.Distance == 0) {
    if (data.BP_OC == 1) {
      return "Pince: Ouverture";
    } else if (data.BP_OC == 2) {
      return "Pince: Fermeture";
    } else {
      return "Pince: Arret";
    }
  } else {
    return "Distance: " + String(data.Distance) + " cm";
  }
}

// Gestion de l'OLED
void Display() {
  oled.clear();

  oled.setCursor(30, 0);
  oled.print(Bluethooth());

  oled.setCursor(0, 3);
  oled.print(DisplayState());

  oled.setCursor(0, 5);
  oled.print(RFID_State());

  const double* Values = Batterie();

  oled.setCursor(0, 7);
  oled.print(String(Values[0]) + "%");

  oled.setCursor(98, 7);
  oled.print(String(Values[1]) + "V");

  oled.update();
}

#endif

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

#include <Adafruit_I2CDevice.h>
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

// Gestion de l'OLED
void Display() {
  oled.clear();

  oled.setCursor(30, 0);
  oled.print(Bluethooth());

  String Print;
  const int distance = data.Distance;

  if (distance > 20 || distance == 0) {
    if (data.BP_OC == 1) {
      Print = "Pince: Ouverture";
    } else if (data.BP_OC == 2) {
      Print = "Pince: Fermeture";
    } else {
      Print = "Pince: Arret";
    }
  } else {
    Print = "Distance: " + String(distance) + " cm";
  }

  oled.setCursor(0, 3);
  oled.print(Print);

  oled.setCursor(0, 5);
  switch (data.RFID_State) {
    case 0:
      oled.print("Robot: Verrouille");
      break;
    case 1:
      oled.print("Robot: Deverrouille");
      break;
    case 2:
      oled.print("Badgez nouvelle carte");
      break;
    case 3:
      oled.print("Nouvelle carte ajoutee");
      break;
    case 4:
      oled.print("Carte refusee!");
      break;
    case 5:
      oled.print("Aucune carte ajoutee!");
      break;
  }

  const double* Values = Batterie();

  oled.setCursor(0, 7);
  oled.print(String(Values[0]) + "%");

  oled.setCursor(98, 7);
  oled.print(String(Values[1]) + "V");

  oled.update();
}

#endif

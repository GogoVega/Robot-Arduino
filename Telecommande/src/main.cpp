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

#include <Adafruit_I2CDevice.h>
#include <Arduino.h>
#include <GyverOLED.h>
#include <SerialTransfer.h> 
#include <Wire.h>
#include <rfid.h>

SerialTransfer SendTransfer;

// OLED
GyverOLED<SSH1106_128x64> oled(0x3C);

// Joysticks Robot
const int AxeX_Gauche = A0, AxeY_Gauche = A1;

// Bouton Poussoir Pince
const int OpenPince = 2, ClosePince = 3;  // Ouvrir-Fermer Pince
const int UpPince = 4, DownPince = 5;     // Monter-Descendre Pince

// Batterie
const int PinBattery = A2;

// State Bluethooth
const int BluethoothPin = 6;

// Structure des données Bluethooth
struct STRUCT {
  int AxeX;
  int AxeY;
  int BP_OC;
  int BP_UD;
  byte Code[4];
  int RFID_State;
  int Distance;
} data;

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

// Valeurs Batterie
double* Batterie() {
  float Tension = map(analogRead(PinBattery), 0, 1023, 0, 500);
  float ChargeLevel = map(Tension, 0, 500, 0, 10000);
  static double Output[2] = {(ChargeLevel / 100), (Tension / 100)};
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
  String Print;

  oled.clear();

  oled.setCursor(30, 0);
  oled.print(Bluethooth());

  if (data.Distance > 20) {
    if (data.BP_OC) {
      Print = "Pince: Ouverte";
    } else {
      Print = "Pince: Fermee";
    }
  } else {
    Print = "Distance: " + String(data.Distance) + " cm";
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

// Gestion RFID
int RFID() {
  int State = data.RFID_State;

  if (!State) {
    State = CheckCode(data.Code);
  } else if (State) {
    State = CheckCode(data.Code);
    if (State) {
      State = 0;
    }

  } else if (State == 2) {
    State = NewCard(data.Code);
  }

  return State;
}

void setup() {
  Serial1.begin(38400);
  SendTransfer.begin(Serial1);

  // OLED
  delay(250);
  oled.init();
  oled.clear();
  oled.print("Demarrage...");
  oled.update();
  delay(500);
}

void loop() {
  Display();

  // Si message reçu => Lecture
  if (SendTransfer.available()) {
    uint16_t sendSize = 0;
    sendSize = SendTransfer.rxObj(data, sendSize);
  }

  // Envoie si Bluethooth connecté
  if (digitalRead(BluethoothPin)) {
    uint16_t sendSize = 0;

    data.AxeX = JoystickValue(AxeX_Gauche);
    data.AxeY = JoystickValue(AxeY_Gauche);
    data.BP_OC = EtatBP_OC();
    data.BP_UD = EtatBP_UD();

    // Si code RFID reçu
    if (data.Code[0] != 0) {
      data.RFID_State = RFID();
      data.Code[0] = 0;
      data.Code[1] = 0;
      data.Code[2] = 0;
      data.Code[3] = 0;
    }

    sendSize = SendTransfer.txObj(data, sendSize);
    SendTransfer.sendData(sendSize);
  }

  delay(1000);
}

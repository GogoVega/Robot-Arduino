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

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SerialTransfer.h>
#include <Wire.h>

SerialTransfer SendTransfer;

// LCD
const int rs = 9, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Joysticks
const int AxeX_Gauche = A0, AxeY_Gauche = A1;  // Robot
const int AxeX_Droite = A2, AxeY_Droite = A3;  // Bras

// Bouton Poussoir Pince
const int OpenPince = 22, ClosePince = 23;  // Ouvrir-Fermer Pince
const int UpPince = 24, DownPince = 25;     // Monter-Descendre Pince

// Batterie
const int PinBattery = A4;

// Structure des données
struct STRUCT {
  int AxeX_Robot;
  int AxeY_Robot;
  int BP_OC;
  int BP_UD;
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
  int Read = data.BP_OC || 0;
  int Write;

  // Open - Close
  if (BP1 && !BP2) {
    Write = 1;
    return Write;
  } else if (BP2 && !BP1) {
    Write = 0;
    return Write;
  } else {
    return Read;
  }
}

// CMD Moter-Descendre la pince
int EtatBP_UD() {
  int BP3 = digitalRead(UpPince);
  int BP4 = digitalRead(DownPince);
  int Write;

  // Arrêt - Monter - Descendre
  if (BP3 && !BP4) {
    Write = 1;
  } else if (BP4 && !BP3) {
    Write = 2;
  } else {
    Write = 0;
  }
  return Write;
}

// Valeur Batterie
float Batterie() {
  float ChargeLevel, Tension;
  Tension = analogRead(PinBattery) * 0.85;
  ChargeLevel = map(Tension, 875, 750, 100, 0);
  return ChargeLevel;
}

// Gestion du LCD
void Display(String a, int x, int y) {
  lcd.clear();
  lcd.setCursor(x, y);
  lcd.print(a);
}

void setup() {
  Serial1.begin(38400);
  SendTransfer.begin(Serial1);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DEMARRAGE");
}

void loop() {
  uint16_t sendSize = 0;
  data.AxeX_Robot = JoystickValue(AxeX_Gauche);
  data.AxeY_Robot = JoystickValue(AxeY_Gauche);
  data.BP_OC = EtatBP_OC();
  data.BP_UD = EtatBP_UD();
  sendSize = SendTransfer.txObj(data, sendSize);
  SendTransfer.sendData(sendSize);

  // Si message reçu => Lecture
  if (SendTransfer.available()) {
    uint16_t sendSize = 0;
    sendSize = SendTransfer.rxObj(data, sendSize);
  }

  Display("PRET", 0, 0);

  delay(1000);
}

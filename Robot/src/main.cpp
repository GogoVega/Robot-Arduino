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
#include <automatic.h>
#include <display.h>
#include <led.h>
#include <motor.h>
#include <rfid.h>
#include <servomotor.h>
#include <sonar.h>
#include <type.h>

void setup() {
  // LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("DEMARRAGE...");

  // Buzzer
  tone(BuzzerPin, 120, 500);

  delay(250);

  // Bluethooth
  Serial1.begin(38400);
  RecTransfer.begin(Serial1);

  // RFID
  SPI.begin();
  rfid.PCD_Init();

  // Servomotors
  servo_OC.attach(ServoOCPin);
  servo_UD.attach(ServoUDPin);
  servoGo();

  // Strip LED
  strip.begin();
  strip.show();
  strip.setBrightness(Brightness);
  StartUp(strip.Color(0, 255, 0), 20);
}

void loop() {
  unsigned long Time = millis();
  uint8_t Write = 0;

  // Tempo signalisation
  if (++Flag == 20)
    Flag = 0;

  // Affichage LCD
  Display();

  // Gestion des LEDs
  Blink();

  // Mode Automatique - Manuel
  if (digitalRead(AutoPin)) {
    automatic();
  } else {
    const uint8_t status = RecTransfer.available();

    // Si message reçu => Lecture
    if (status) {
      uint16_t recSize = 0;
      recSize = RecTransfer.rxObj(data, recSize);
    }

    // Si Robot déverrouillé
    if (data.RFID_State == 1) {
      ReadSpeeds(data.Axe_X, data.Axe_Y);
      servoGo();
    } else {
      WriteSpeeds(0, 0);
    }

    // Envoie si Bluethooth connecté
    if (digitalRead(BluethoothPin)) {
      // Si carte RFID détectée
      if (rfid.PICC_IsNewCardPresent()) {
        byte* CodeRead = ReadRFID();
        Write = 1;

        for (uint8_t i = 0; i < 4; i++) {
          data.Code[i] = CodeRead[i];
        }

        data.RFID_State = StateRFID(data.RFID_State);
      }

      // Envoie distance si < 25cm
      uint16_t distance = Sonar();
      if (distance < 25 && distance != 0) {
        Write = 1;
        data.Distance = distance;
      } else if (distance > 25 && distance != 0) {
        Write = 1;
        data.Distance = 0;
      }

      // Ecriture si changement et pas d'erreur
      if (Write && status) {
        uint16_t recSize = 0;
        recSize = RecTransfer.txObj(data, recSize);
        RecTransfer.sendData(recSize);
      }
    }
  }

  while (millis() < Time + 50) {
  }
}

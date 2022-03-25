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
#include <type.h>
#include <motor.h>
#include <servomotor.h>
#include <sonar.h>
#include <rfid.h>

void setup() {
  Serial1.begin(38400);
  RecTransfer.begin(Serial1);

  // Buzzer
  tone(BuzzerPin, 100, 250);

  // RFID
  SPI.begin();
  rfid.PCD_Init();

  // Servomotors
  servo.attach(ServoPin1);
  servo.attach(ServoPin2);

  // LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("DEMARRAGE...");
}

void loop() {
  Display();

  // Mode Automatique
  if (digitalRead(AutoPin)) {
    automatic();
  }

  // Si message reçu => Lecture
  if (RecTransfer.available()) {
    uint16_t recSize = 0;
    recSize = RecTransfer.rxObj(data, recSize);
  }

  // Si Robot déverrouillé
  if (data.RFID_State == 1) {
    ReadVitesseRobot(data.Axe_X, data.Axe_Y);
    // Servo Call
  }

  // Envoie si Bluethooth connecté
  if (digitalRead(BluethoothPin) && !digitalRead(AutoPin)) {
    uint16_t recSize = 0;

    // Si carte RFID détectée
    if (rfid.PICC_IsNewCardPresent()) {
      byte* CodeRead = ReadRFID();

      data.Code[0] = CodeRead[0];
      data.Code[1] = CodeRead[1];
      data.Code[2] = CodeRead[2];
      data.Code[3] = CodeRead[3];
      data.RFID_State = StateRFID(data.RFID_State);
    }

    // Init RFID
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    data.Distance = Sonar();

    recSize = RecTransfer.txObj(data, recSize);
    RecTransfer.sendData(recSize);
  }

  delay(1000);
}

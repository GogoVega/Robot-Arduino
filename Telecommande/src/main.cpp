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
#include <display.h>
#include <rfid.h>
#include <type.h>
#include <utils.h>

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
  unsigned long Time = millis();

  // Gestion OLED
  Display();

  // Si message reçu => Lecture
  if (SendTransfer.available()) {
    uint16_t sendSize = 0;
    sendSize = SendTransfer.rxObj(data, sendSize);
  }

  // Envoie si Bluethooth connecté
  if (digitalRead(BluethoothPin)) {
    uint16_t sendSize = 0;

    data.Axe_X = JoystickValue(AxeX);
    data.Axe_Y = JoystickValue(AxeY);
    data.BP_OC = EtatBP(OpenPince, ClosePince);
    data.BP_UD = EtatBP(UpPince, DownPince);

    // Si code RFID reçu
    if (data.Code[0] != 0) {
      data.RFID_State = RFID(data.RFID_State);
      data.Code[0] = 0;
      data.Code[1] = 0;
      data.Code[2] = 0;
      data.Code[3] = 0;
    }

    sendSize = SendTransfer.txObj(data, sendSize);
    SendTransfer.sendData(sendSize);
  }

  while (millis() < Time + 50) {
  }
}

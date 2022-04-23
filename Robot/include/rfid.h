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

#ifndef __RFID_H
#define __RFID_H

#include <MFRC522.h>
#include <SPI.h>
#include <type.h>

MFRC522 rfid(SDA_RFID, RST_RFID);

// Gestion Etat RFID:
//
// 0 Robot Verouillé
// 1 Robot Déverrouillé
// 2 Badgez nouvelle carte
// 3 Nouvelle carte ajoutée
// 4 Carte refusée
// 5 Aucune carte ajoutée
// 9 ERROR FLASH
int StateRFID(int State) {
  switch (State) {
    case 3:
    case 4:
    case 5:
    case 9:
      return 0;
  }

  return State;
}

// Lecture Badge RFID
byte* ReadRFID() {
  static byte Response[4] = {};

  if (rfid.PICC_ReadCardSerial()) {
    for (int i = 0; i < 4; i++) {
      Response[i] = rfid.uid.uidByte[i];
    }

    // Init RFID
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }

  return Response;
}

#endif

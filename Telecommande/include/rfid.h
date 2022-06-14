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

#include <RFIDtoEEPROM.h>
#include <type.h>

RFIDtoEEPROM myCard;

// Save new Card
uint8_t NewCard() {
  if (myCard.SaveCard(data.Code)) {
    return 3;
  }

  return 9;
}

// Check Code RFID reçu:
uint8_t CheckCode() {
  // Number of Cards saved
  const int len = myCard.CardNumber();

  if (!len) {
    if (digitalRead(DownPince)) {
      // Save the first Card
      return NewCard();
    }

    return 5;
  }

  if (myCard.CardCheck(data.Code)) {
    // Si déverrouillé => Verrouiller
    if (data.RFID_State == 1) {
      return 0;
    }

    // Si demande enregistrement
    if (digitalRead(DownPince)) {
      return 2;
    }

    return 1;
  }

  return 4;
}

// Gestion RFID
//
// 0 Robot Verouillé
// 1 Robot Déverrouillé
// 2 Badgez nouvelle carte
// 3 Nouvelle carte ajoutée
// 4 Carte refusée
// 5 Aucune carte ajoutée
// 9 ERROR FLASH
int RFID(int State) {
  switch (State) {
    case 0:
    case 1:
      State = CheckCode();
      break;
    case 2:
      State = NewCard();
      break;
  }

  return State;
}

#endif

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

#include <RFID_Card.h>

#if defined(__AVR__)
// Arduino

#include <EEPROM.h>

// Reset the number of cards
void RFID_Card::clear() {
  EEPROM.write(0, 0);
}


// Number of Cards saved
int RFID_Card::cardNumber() {
  nbr = EEPROM.read(0);
  return nbr;
}


// Save the new Card
boolean RFID_Card::saveCard(byte Code[4]) {
  bool mismatch = false;
  nbr = cardNumber();

  for (int n = 0; n < 4; n++) {
    EEPROM.write(((nbr * 4) + 1 + n), Code[n]);
  }

  EEPROM.write(0, (nbr + 1));

  if (EEPROM.read(1) != (nbr + 1))
    return true;

  for (int n = 0; n < 4; n++) {
    if (Code[n] != EEPROM.read(((nbr * 4) + 1 + n)))
      mismatch = true;
  }

  return mismatch;
}


// Check if the Card matches
boolean RFID_Card::cardCheck(byte Code[4]) {
  nbr = cardNumber();

  for (int i = 0; i < nbr; i++) {
    byte Code_Read[] = {};

    for (int n = 0; n < 4; n++) {
      Code_Read[n] = EEPROM.read((i * 4) + n + 1);
    }

    if (Code_Read[0] == Code[0] && Code_Read[1] == Code[1] &&
        Code_Read[2] == Code[2] && Code_Read[3] == Code[3]) {
      return true;
    }
  }

  return false;
}

#elif defined(ARDUINO_ARCH_RP2040)
// Raspberry Pi Pico

#include "hardware/flash.h"

#define FLASH_TARGET_OFFSET (256 * 1024)

const uint8_t *flash_target_contents =
    (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);


// Erasing target region
void RFID_Card::clear() {
  flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
}


// Number of Cards saved
int RFID_Card::cardNumber() {
  nbr = flash_target_contents[0];
  return nbr;
}


// Save the new Card
boolean RFID_Card::saveCard(byte Code[4]) {
  uint8_t random_data[FLASH_PAGE_SIZE];
  bool mismatch = false;
  nbr = cardNumber();

  random_data[0] = (nbr + 1);
  for (int n = 0; n < 4; n++) {
    random_data[((nbr * 4) + 1 + n)] = Code[n];
  }

  flash_range_program(FLASH_TARGET_OFFSET, random_data, FLASH_PAGE_SIZE);

  for (unsigned int i = 0; i < FLASH_PAGE_SIZE; i++) {
    if (random_data[i] != flash_target_contents[i])
      mismatch = true;
  }

  return mismatch;
}


// Check if the Card matches
boolean RFID_Card::cardCheck(byte Code[4]) {
  const int i = ((nbr - 1) * 4) + 1;
  nbr = cardNumber();

  for (int n = 0; n < nbr; n++) {
    if (Code[0] == flash_target_contents[i] && Code[1] ==
        flash_target_contents[i + 1] && Code[2] == flash_target_contents[i + 2] &&
            Code[3] == flash_target_contents[i + 3]) {
      return true;
    }
  }

  return false;
}

#else

#error “Unsupported board selected!”

#endif
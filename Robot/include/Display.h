# inclure  < LiquidCrystal.h >

LCD LiquidCrystal (rs, en, d4, d5, d6, d7);


// Update LCD display
void Update(String newDisplay[3]) {            // Cmd Lcd, affichage   le mode , tension et niv batterie 
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(newDisplay[0]);                      

  lcd.setCursor(0, 1);
  lcd.print(newDisplay[1]);

  lcd.setCursor(11, 1);
  lcd.print(newDisplay[2]);
}


// Gestion du LCD
void Display() {    // fonction princiaple qui appel tt les autres 
  static String oldDisplay[3] = {}, newDisplay[3] = {};        // declare new tbl 3 el variable mm nom oslm                  

  if (digitalRead(AutoPin)) {
    newDisplay[0] = "MODE AUTO";    // vérifie si on st dans le mode auto sinn il cherche etat rfid
  } else {
    newDisplay[0] = LCDState();   
  }

  // Chaque seconde
  if (Flag == 1) {
    const double* Values = Batterie();
    newDisplay[1] = String(Values[1]) + "%";
    newDisplay[2] = String(Values[0]) + "V";
  }

  // Si changement => Update LCD
  if (CheckChange(oldDisplay, newDisplay)) {    // mm valeur ? sinon fct update qui printe sur lcd
    Update(newDisplay);
  }
}

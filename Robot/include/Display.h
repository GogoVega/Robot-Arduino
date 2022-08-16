#inclure < LiquidCrystal.h>

LCD LiquidCrystal(rs, en, d4, d5, d6, d7);

// Update LCD display
void Update(String newDisplay[3]) {  // Cmd Lcd, affichage   le mode , tension
                                     // et niv batterie
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(newDisplay[0]);

  lcd.setCursor(0, 1);
  lcd.print(newDisplay[1]);

  lcd.setCursor(11, 1);
  lcd.print(newDisplay[2]);
}

// Gestion du LCD
void Display() {  // fonction princiaple qui appel tt les autres
  static String oldDisplay[3] = {},
                newDisplay[3] =
                    {};  // declare new tbl 3 el variable mm nom oslm

  if (digitalRead(AutoPin)) {
    newDisplay[0] = "MODE AUTO";  // vérifie si on st dans le mode auto sinn il
                                  // cherche etat rfid
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
  if (CheckChange(
          oldDisplay,
          newDisplay)) {  // mm valeur ? sinon fct update qui printe sur lcd
    Update(newDisplay);
  }
}

// Vérification si changement
boolean CheckChange(
    String oldDisplay[3],
    String newDisplay[3]) {  // Vérification anciennes et nouvelles valeurs =?,
                             // oui : aucun changement , nn : egalisation x3
  boolean change = 0;

  if (newDisplay[0] != oldDisplay[0]) {
    oldDisplay[0] = newDisplay[0];  // si elles sont dif alors on les =
    change = 1;
  }
  if (newDisplay[1] != oldDisplay[1]) {
    oldDisplay[1] = newDisplay[1];
    change = 1;
  }
  if (newDisplay[2] != oldDisplay[2]) {
    oldDisplay[2] = newDisplay[2];
    change = 1;
  }

  return change;
}

// Valeurs Bluethooth et RFID
String LCDState() {                   // affichages des message sur LCD
  if (!digitalRead(BluethoothPin)) {  // reg si bluethoot est co
    return "  Connexion...";          // si il est pas co il affiche co
  } else {                            // sinon mess
    switch (data.RFID_State) {
      case 0:
        return "   Verrouille";
      case 1:  // else     en fct du num de la variable Rfid_State =>    /appui
               // bouton quidit que je vx badger encore 1x
        return " Deverrouille";
      case 2:
        return "Badgez nvl carte";
      case 3:
        return "Carte ajoutee";
      case 4:
        return "Carte refusee!";
      case 5:
        return "Aucune carte!";
    }
  }

  return "ERROR";
}

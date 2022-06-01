String LCDState() {
  if (!digitalRead(BluethoothPin)) {
    return "  Connexion...";
  } else {
    switch (data.RFID_State) {
      case 0:
        return "   Verrouille";
      case 1:
        return " Deverrouille";
      case 2:
        return "Badgez nvll carte";    // Affichage des messages sur Lcd après connexion Blueth
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


// Valeurs Batterie                     
float* Batterie() {
  static float Output[2] = {};
  const float Tension = map(analogRead(BatteryPin), 0, 1023, 0, 500);
  const float ChargeLevel = map(Tension, 0, 500, 0, 10000);                              //Conversion des entrées analogiques , valeur tension et baterrie
  Output[0] = Tension / 100;
  Output[1] = ChargeLevel / 100;

  return Output;
}




// Update LCD display
void Update(String newDisplay[3]) {            // Cmd Lcd, affichage du mode ,la tension et niv batterie 
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(newDisplay[0]);                      

  lcd.setCursor(0, 1);
  lcd.print(newDisplay[1]);

  lcd.setCursor(11, 1);
  lcd.print(newDisplay[2]);
}



// Vérification si changement
boolean CheckChange(String oldDisplay[3], String newDisplay[3]) {  // Vérification anciennes et nouvelles valeurs =?, oui : aucun changement , nn : egalisation x3   
  boolean change = 0;

  if (newDisplay[0] != oldDisplay[0]) {
    oldDisplay[0] = newDisplay[0]; // si elles sont dif alors on les = 
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


// Gestion du LCD
void Display() {    // fonction princiaple qui appel tt les autres 
  static String oldDisplay[3] = {}, newDisplay[3] = {}; 
  if (digitalRead(AutoPin)) {
    newDisplay[0] = "MODE AUTO";    // vérifie si on st dans le mode auto sinn il cherche etat rfid
  } else {
    newDisplay[0] = LCDState();   
  }

  // Chaque seconde
  if (Flag == 1) {
    const double* Values = Batterie();              //Affichage tension et % batterie
    newDisplay[1] = String(Values[1]) + "%";
    newDisplay[2] = String(Values[0]) + "V";
  }
 // Si changement => Update LCD
  if (CheckChange(oldDisplay, newDisplay)) {
    Update(newDisplay);
  }
}


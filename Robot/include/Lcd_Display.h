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

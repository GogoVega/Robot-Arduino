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
        return "Badgez nvll carte";
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

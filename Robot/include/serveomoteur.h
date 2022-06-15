/* Inclut la librairie Servo pour manipuler le servomoteur */
#include <Servo.h>

int bp1 = 2;
int bp2 = 7;

int position = 0;

/* Créer un objet Servo pour contrôler le servomoteur */
Servo monServomoteur;

void setup()
{
  // Bouton poussoir 1
  pinMode(bp1, INPUT_PULLUP);
  pinMode(bp2, INPUT_PULLUP);

  // Attache le servomoteur à la broche 2
  monServomoteur.attach(5);
  monServomoteur.write(0);

  Serial.begin(9600);
}
void loop()
{
  // Récuperer valeur de bp1
  int b1 = digitalRead(bp1);
  int b2 = digitalRead(bp2);

  // Si Appuie BP1
  if (b1 == LOW) {
    monServomoteur.write(position);
    position = position + 1;
    if (position > 180) {
      position = 180;
    }
    delay(10);
  }

  // Si Appuie BP2
  if (b2 == LOW) {
    monServomoteur.write(position);
    position = position - 1;
    if (position < 0) {
      position = 0;
    }
    delay(10);
  }

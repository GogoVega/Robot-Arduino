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

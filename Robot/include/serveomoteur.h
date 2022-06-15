/* Inclut la librairie Servo pour manipuler le servomoteur */
#include <Servo.h>

int bp1 = 2;
int bp2 = 7;

int position = 0;

/* Créer un objet Servo pour contrôler le servomoteur */
Servo monServomoteur;

void setup()
{
  // Bouto

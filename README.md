Servo servo_OC;
Servo servo_UD;

//renvoie l'angle voulu
int servoWrite(int angles, int state) {
  if (state == 1 && angles < 180) {
    angles++;
  } else if (state == 2 && angles > 0 {
    angles--;
  }
  return angles;
}

//gestion servomoteurs
void servoGo() {
  static int angles[2] = {0, 0};

  angles[0] = servoWrite(angles[0], data.BP_OC);
  angles[1] = servoWrite(angles[1], data.BP_UD);

  servo_OC.write(angles[0]);
  servo_UD.write(angles[1]);
}

#include <LiquidCrystal.h>
#include <NewPing.h>
#define trigger_output 8
#define echo_input 9
#define control_input A0
#define max_distance 200

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7, trig = 8, echo = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
NewPing sonar(trigger_output, echo_input, max_distance);
int rawvalue = 0;

#include <Arduino.h>


int cislo_robota;

void setup_config()
{
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  if (digitalRead(2) == 0) cislo_robota = 0;
  else if (digitalRead(3) == 0) cislo_robota = 1;
  if (digitalRead(6) == 0) cislo_robota = 2;
}
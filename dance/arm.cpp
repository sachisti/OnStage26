#include <Arduino.h>
#include <Servo.h>

static unsigned long arm_update_freq = 30;

Servo s[6];
int selected_servo = 0;
int positions[6] = {90, 90, 90, 90, 90, 90};
int targets[6] = {90, 90, 90, 90, 90, 90};

int zaciatocne[6] = {74, 132, 179, 104, 113, 90};

void setup_arm()
{ 
  delay(3000);

  s[0].attach(27);
  s[1].attach(26);
  s[2].attach(22);
  s[3].attach(9);
  s[4].attach(8);
  s[5].attach(7);

  delay(3000);
  for (int i = 0; i < 6; i++)
  {
    positions[i] = zaciatocne[i];
    s[i].write(90);
    delay(1000);
  }
}

void new_arm_target(int target_positions[])
{
  for (int i = 0; i < 6; i++)
    targets[i] = target_positions[i];
}

unsigned long last_time_updated = 0;
void loop_arm()
{
  if (millis() - last_time_updated > arm_update_freq)
  {
      for (int servo = 0; servo < 6; servo++)
      {
        if (positions[servo] > targets[servo])
        {
           positions[servo]--;
           s[servo].write(positions[servo]); 
        }
        else if (positions[servo] < targets[servo])
        {
           positions[servo]++;
           s[servo].write(positions[servo]); 
        }
      }
      last_time_updated = millis();
  } 
}

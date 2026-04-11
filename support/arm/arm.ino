#include <Servo.h>

void print_usage()
{
  Serial.println("Control keys:");
  Serial.println(" * / :  select servo");
  Serial.println(" + - :  move servo");
}

Servo s[6];
int selected_servo = 0;
int positions[6] = {90, 90, 90, 90, 90, 90};

void setup()
{
  delay(2000);
  Serial.begin(115200);
  
  delay(3000);
  print_usage();

  s[0].attach(27);
  s[1].attach(26);
  s[2].attach(22);
  s[3].attach(9);
  s[4].attach(8);
  s[5].attach(7);

  delay(3000);
  for (int i = 0; i < 6; i++)
  {
    positions[i] = 90;
    s[i].write(90);
    delay(1000);
  }
}

void execute_command(char cmd)
{
  int update_servo = 0;
  int update_position = 0;
  switch (cmd)
  {
    case '*': if (selected_servo < 5) selected_servo++; update_servo = 1;  break;
    case '/': if (selected_servo > 0) selected_servo--; update_servo = 1; break;
    case '+': if (positions[selected_servo] < 180) positions[selected_servo]++; update_position = 1; break; 
    case '-': if (positions[selected_servo] > 0) positions[selected_servo]--; update_position = 1;  break; 
    case '?': print_usage(); break;
  }
  if (update_servo) 
  {
    Serial.print("Servo "); 
    Serial.println(selected_servo);
  }
  if (update_position)
  {
    s[selected_servo].write(positions[selected_servo]); 
    Serial.println(positions[selected_servo]);
  }
}

void loop()
{
  char cmd = Serial.read();
  execute_command(cmd);
}

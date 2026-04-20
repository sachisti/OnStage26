#include "lidar.h"
#include "mecanum.h"
#include "arm.h"

void setup()
{
  Serial.begin(115200);
  setup_mecanum();
  setup_arm();
}

void setup1()   // druhe jadro
{
  setup_lidar();
}

void loop()
{
  loop_arm();
}

void loop1()
{
  loop_lidar();
}
#include "lidar.h"
#include "mecanum.h"
#include "arm.h"
#include "husky.h"
#include "danceplan.h"

void setup()
{
  Serial.begin(115200);
  setup_mecanum();
  setup_arm();
  setup_husky();
  setup_danceplan();
}

void setup1()   // druhe jadro
{
  setup_lidar();
}

void loop()
{
  loop_arm();
  loop_husky();
  loop_danceplan();
}

void loop1()
{
  loop_lidar();
}
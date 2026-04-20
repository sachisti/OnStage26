#include <Pico4Drive.h>
#include <math.h>

Pico4Drive board;
static int aktualna_korekcia;

void setup() {
  board.init();
  aktualna_korekcia = 0;
  Serial.begin(115200);
}

/************************************************/

void left_front_motor(int speed) {
  board.set_driver_PWM(-speed, p2d_drv1);
}

void left_rear_motor(int speed) {
  board.set_driver_PWM(speed, p2d_drv2);
}

void right_front_motor(int speed) {
  board.set_driver_PWM(speed, p2d_drv4);
}

void right_rear_motor(int speed) {
  board.set_driver_PWM(speed, p2d_drv3);
}

/************************************************/

void left_front_motor_fwd(int speed) {
  board.set_driver_PWM(-speed, p2d_drv1);
}

void left_front_motor_bwd(int speed) {
  board.set_driver_PWM(speed, p2d_drv1);
}

void left_rear_motor_fwd(int speed) {
  board.set_driver_PWM(speed, p2d_drv2);
}

void left_rear_motor_bwd(int speed) {
  board.set_driver_PWM(-speed, p2d_drv2);
}

void right_front_motor_fwd(int speed) {
  board.set_driver_PWM(speed, p2d_drv4);
}

void right_front_motor_bwd(int speed) {
  board.set_driver_PWM(-speed, p2d_drv4);
}

void right_rear_motor_fwd(int speed) {
  board.set_driver_PWM(speed, p2d_drv3);
}

void right_rear_motor_bwd(int speed) {
  board.set_driver_PWM(-speed, p2d_drv3);
}

void stop_all_motors() {
  board.set_driver_PWM(0, p2d_drv1);
  board.set_driver_PWM(0, p2d_drv2);
  board.set_driver_PWM(0, p2d_drv3);
  board.set_driver_PWM(0, p2d_drv4);
}
/************************************************/

void otocka_vlavo(int speed)
{
    left_front_motor_bwd(speed);
    left_rear_motor_bwd(speed);
    right_front_motor_fwd(speed);
    right_rear_motor_fwd(speed);
}

void otocka_vpravo(int speed)
{
    left_front_motor_fwd(speed);
    left_rear_motor_fwd(speed);
    right_front_motor_bwd(speed);
    right_rear_motor_bwd(speed);
}

void oblucik_vpravo(int speed1, int speed2)
{
    left_front_motor_fwd(speed1);
    left_rear_motor_bwd(speed2);
    right_front_motor_bwd(speed1);
    right_rear_motor_fwd(speed2);
}

void oblucik_vlavo(int speed1, int speed2)
{
    left_front_motor_bwd(speed1);
    left_rear_motor_fwd(speed2);
    right_front_motor_fwd(speed1);
    right_rear_motor_bwd(speed2);
}

void motors(int left_front, int left_rear, int right_front, int right_rear)
{
  if (aktualna_korekcia != 0)  // tocime vpravo
  {
    left_front += 200 * aktualna_korekcia;
    left_rear += 200 * aktualna_korekcia;
    right_front += -200 * aktualna_korekcia;
    right_rear += -200 * aktualna_korekcia;
    
    int extra = 0, extra2;

    if (abs(left_front) > board.analogWriteMax) {
      extra = abs(left_front) - board.analogWriteMax;
    }
    if (abs(left_rear) > board.analogWriteMax) {
      extra2 = abs(left_rear) - board.analogWriteMax;
      if (extra2 > extra) extra = extra2;
    }
    if (abs(right_front) > board.analogWriteMax) {
      extra2 = abs(right_front) - board.analogWriteMax;
      if (extra2 > extra) extra = extra2;
    }
    if (abs(right_rear) > board.analogWriteMax) {
      extra2 = abs(right_rear) - board.analogWriteMax;
      if (extra2 > extra) extra = extra2;
    }

    if (extra > 0)
    {
      extra += board.analogWriteMax;
      left_front = board.analogWriteMax * left_front / extra;
      left_rear = board.analogWriteMax * left_rear / extra;
      right_front = board.analogWriteMax * right_front / extra;
      right_rear = board.analogWriteMax * right_rear / extra;
    }
  }

  board.set_driver_PWM(-left_front, p2d_drv1);
  board.set_driver_PWM(left_rear, p2d_drv2);
  board.set_driver_PWM(right_front, p2d_drv4);
  board.set_driver_PWM(right_rear, p2d_drv3);
}

void sikmo(int speed, int alfa) // 0-vpred, 90- vpravo, 180-vzad, 270-vlado
{
  if (abs(alfa) > 360) alfa = alfa % 360;  
  if (alfa < 0) alfa += 360;
  // alfa je uz iba 0 - 360

  if (alfa < 90)
  {
    motors(speed, 
           speed * (45 - alfa) / 45,
           speed * (45 - alfa) / 45,
           speed);    
  }
  else if (alfa <= 180)
  {
    motors(speed * (135 - alfa) / 45, 
           speed,
           speed,
           speed * (135 - alfa) / 45);
  }
  else if (alfa <= 270)
  {
    motors(speed, 
           speed * (alfa - 225) / 45,
           speed * (alfa - 225) / 45,
           speed);
  }
  else
  {
    motors(speed * (alfa - 315) / 45, 
           speed,
           speed,
           speed * (alfa - 315) / 45);
  }
}

void vzad(int speed)
{
    left_front_motor_bwd(speed);
    left_rear_motor_bwd(speed);
    right_front_motor_bwd(speed);
    right_rear_motor_bwd(speed);
}

void vpred(int speed)
{
    left_front_motor_fwd(speed);
    left_rear_motor_fwd(speed);
    right_front_motor_fwd(speed);
    right_rear_motor_fwd(speed);
}

void vpravo(int speed)
{
    left_front_motor_fwd(speed);
    left_rear_motor_bwd(speed);
    right_front_motor_bwd(speed);
    right_rear_motor_fwd(speed);
}

void vlavo(int speed)
{
    left_front_motor_bwd(speed);
    left_rear_motor_fwd(speed);
    right_front_motor_fwd(speed);
    right_rear_motor_bwd(speed);
}

/************************************************/

void test_motors_one_by_one()
{
  Serial.println("left front");
  left_front_motor_fwd(500);
  delay(3000);
  left_front_motor_bwd(500);
  delay(3000);
  left_front_motor_fwd(0);

  Serial.println("left rear");
  left_rear_motor_fwd(500);
  delay(3000);
  left_rear_motor_bwd(500);
  delay(3000);
  left_rear_motor_fwd(0);

  Serial.println("right front");
  right_front_motor_fwd(500);
  delay(3000);
  right_front_motor_bwd(500);
  delay(3000);
  right_front_motor_fwd(0);

  Serial.println("right rear");
  right_rear_motor_fwd(500);
  delay(3000);
  right_rear_motor_bwd(500);
  delay(3000);
  right_rear_motor_fwd(0);
  delay(30000);
}

void korekcia(int kam)   // 0 - zrusit, +1 - +2 - doprava mierne, silno, -1, -2 - dolava mierne, silno
{
  aktualna_korekcia = kam;
}

void test_sikmo()
{
  korekcia(2);
  for (int i = 0; i < 360; i += 10)
  {
    sikmo(1000, i);
    delay(2000);
    sikmo(1000, 180 + i);
    delay(2000);
  }
  stop_all_motors();
}

/************************************************/

void loop()
{
  test_sikmo();

  delay(3000);
  vpred(800);
  delay(3000);
  stop_all_motors();

  delay(3000);
  vzad(800);
  delay(3000);
  stop_all_motors();

  delay(3000);
  vlavo(800);
  delay(3000);
  stop_all_motors();

  delay(3000);
  vpravo(800);
  delay(3000);
  stop_all_motors();

  delay(3000);
  otocka_vlavo(800);
  delay(3000);
  stop_all_motors();

  delay(3000);
  otocka_vpravo(800);
  delay(3000);
  stop_all_motors();

  delay(3000);
  oblucik_vlavo(400, 900);
  delay(3000);
  stop_all_motors();

  delay(3000);
  oblucik_vpravo(400, 900);
  delay(3000);
  stop_all_motors();

  delay(3000);
  oblucik_vlavo(900, 400);
  delay(3000);
  stop_all_motors();

  delay(3000);
  oblucik_vpravo(900, 400);
  delay(3000);
  stop_all_motors();

  delay(10000);

}


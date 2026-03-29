#include <Pico4Drive.h>
#include <math.h>

Pico4Drive board;

void setup() {
  board.init();
  Serial.begin(115200);
}

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

void sikmo_vpravo(int speed1, int speed2)
{
    left_front_motor_fwd(speed1);
    left_rear_motor_bwd(speed2);
    right_front_motor_bwd(speed1);
    right_rear_motor_fwd(speed2);
}

void sikmo_vlavo(int speed1, int speed2)
{
    left_front_motor_bwd(speed1);
    left_rear_motor_fwd(speed2);
    right_front_motor_fwd(speed1);
    right_rear_motor_bwd(speed2);
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

void loop()
{
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
  sikmo_vlavo(400, 900);
  delay(3000);
  stop_all_motors();

  delay(3000);
  sikmo_vpravo(400, 900);
  delay(3000);
  stop_all_motors();

  delay(3000);
  sikmo_vlavo(900, 400);
  delay(3000);
  stop_all_motors();

  delay(3000);
  sikmo_vpravo(900, 400);
  delay(3000);
  stop_all_motors();

  delay(10000);

}


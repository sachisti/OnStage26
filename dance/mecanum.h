#ifndef _MECANUM_H_
#define _MECANUM_H_

void setup_mecanum();

void left_front_motor(int speed);
void left_rear_motor(int speed);
void right_front_motor(int speed);
void right_rear_motor(int speed);

void stop_all_motors();
/************************************************/

void otocka_vlavo(int speed);
void otocka_vpravo(int speed);
void oblucik_vpravo(int speed1, int speed2);
void oblucik_vlavo(int speed1, int speed2);

void motors(int left_front, int left_rear, int right_front, int right_rear);

void sikmo(int speed, int alfa); // 0-vpred, 90- vpravo, 180-vzad, 270-vlado

void vzad(int speed);
void vpred(int speed);
void vpravo(int speed);
void vlavo(int speed);
/************************************************/

void korekcia(int kam);   // 0 - zrusit, +1 - +2 - doprava mierne, silno, -1, -2 - dolava mierne, silno

#endif

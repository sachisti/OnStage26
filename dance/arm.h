#ifndef _ARM_H_
#define _ARM_H_

#define PAZA         0
#define LAKET        1
#define ZLOMZAPASTIE 2
#define OTOCZAPASTIE 3
#define PRSTY        4
#define OTACA        5

void setup_arm();
void new_arm_target(int target_positions[]);
void loop_arm();

#endif
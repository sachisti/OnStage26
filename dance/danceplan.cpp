#include <Arduino.h>
#include "mecanum.h"
#include "arm.h"
#include "config.h"


#define MAX_POCET_KROKOV  10

int pocet_krokov[POCET_ROBOTOV] = { 10, 10, 10 };

#define POCET_POZICII 4

// prikazy
#define ZASTAV         0                  // nic
#define SIKMO          1                  // rychlost, uhol
#define OTOCKA_VPRAVO  2                  // rychlost
#define OTOCKA_VLAVO   3                  // rychlost
#define OBLUCIK_VLAVO  4                  // rychlost1, rychlost2
#define OBLUCIK_VPRAVO 5                  // rychlost1, rychlost2
#define RAMENO         6                  // index pozicie (napr. 3 => pozicia3)

#define NIC            -1

int pozicia0[6] = {74, 132, 179, 104, 113, 90 };
int pozicia1[6] = {74, 132, 179, 104, 113, 90};
int pozicia2[6] = {74, 132, 179, 104, 113, 90};
int pozicia3[6] = {74, 132, 179, 104, 113, 90};

int *pozicie[] = { pozicia0, pozicia1, pozicia2, pozicia3 };


unsigned long cas_kroku[POCET_ROBOTOV][MAX_POCET_KROKOV] = {{ 2000, 4000, 6000, 8000, 10000, 12000, 14000, 16000, 18000, 20000 },
                                                         { 2000, 4000, 6000, 8000, 10000, 12000, 14000, 16000, 18000, 20000 },
                                                         { 2000, 4000, 6000, 8000, 10000, 12000, 14000, 16000, 18000, 20000 } };
int prikaz_kroku[POCET_ROBOTOV][MAX_POCET_KROKOV] = {
                                 { SIKMO, 
                                   SIKMO, 
                                   SIKMO, 
                                   SIKMO, 
                                   OTOCKA_VPRAVO, 
                                   OTOCKA_VLAVO,
                                   OBLUCIK_VLAVO,
                                   OBLUCIK_VPRAVO,
                                   ZASTAV,
                                   RAMENO
                                   },
                                   { SIKMO, 
                                   SIKMO, 
                                   SIKMO, 
                                   SIKMO, 
                                   OTOCKA_VPRAVO, 
                                   OTOCKA_VLAVO,
                                   OBLUCIK_VLAVO,
                                   OBLUCIK_VPRAVO,
                                   ZASTAV,
                                   RAMENO
                                   },
                                   { SIKMO, 
                                   SIKMO, 
                                   SIKMO, 
                                   SIKMO, 
                                   OTOCKA_VPRAVO, 
                                   OTOCKA_VLAVO,
                                   OBLUCIK_VLAVO,
                                   OBLUCIK_VPRAVO,
                                   ZASTAV,
                                   RAMENO
                                   } };
int param1[POCET_ROBOTOV][MAX_POCET_KROKOV] = {           // rychlost alebo index pozicie ramena
          { 800,
            800,
            800,
            800,     
            800,
            800,
            800,
            800,
            NIC,
            1 },            // pozicia1
          { 800,
            800,
            800,
            800,     
            800,
            800,
            800,
            800,
            NIC,
            1 },            // pozicia1
          { 800,
            800,
            800,
            800,     
            800,
            800,
            800,
            800,
            NIC,
            1 }            // pozicia1
};
int param2[POCET_ROBOTOV][MAX_POCET_KROKOV] = {           // druha rychlost alebo uhol
          { 0,
            180,
            90,
            270,
            NIC,
            NIC,
            200,
            200,
            NIC,
            NIC },
          { 0,
            180,
            90,
            270,
            NIC,
            NIC,
            200,
            200,
            NIC,
            NIC },
          { 0,
            180,
            90,
            270,
            NIC,
            NIC,
            200,
            200,
            NIC,
            NIC }
};

static int krok = 0;
static unsigned long cas_startu;

void setup_danceplan()
{
   int krok = 0;
   cas_startu = millis();
   Serial.print("dance start r");
   Serial.println(cislo_robota);
}

void loop_danceplan()
{
   if (krok == pocet_krokov[cislo_robota]) return;

   if (millis() - cas_startu > cas_kroku[cislo_robota][krok])
   {
      Serial.print("krok "); Serial.println(krok);
      if (prikaz_kroku[cislo_robota][krok] == ZASTAV) stop_all_motors();
      else if (prikaz_kroku[cislo_robota][krok] == SIKMO) sikmo(param1[cislo_robota][krok], param2[cislo_robota][krok]);
      else if (prikaz_kroku[cislo_robota][krok] == OTOCKA_VPRAVO) otocka_vpravo(param1[cislo_robota][krok]);
      else if (prikaz_kroku[cislo_robota][krok] == OTOCKA_VLAVO) otocka_vlavo(param1[cislo_robota][krok]);
      else if (prikaz_kroku[cislo_robota][krok] == OBLUCIK_VLAVO) oblucik_vlavo(param1[cislo_robota][krok], param2[cislo_robota][krok]);
      else if (prikaz_kroku[cislo_robota][krok] == OBLUCIK_VPRAVO) oblucik_vpravo(param1[cislo_robota][krok], param2[cislo_robota][krok]);
      else if (prikaz_kroku[cislo_robota][krok] == RAMENO) new_arm_target(pozicie[param1[cislo_robota][krok]]);
      krok ++;
   }
}
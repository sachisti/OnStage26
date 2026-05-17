#include <Arduino.h>
#include "mecanum.h"
#include "arm.h"
#include "config.h"


#define MAX_POCET_KROKOV  15

int pocet_krokov[POCET_ROBOTOV] = { 15, 15, 15 };

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

int pozicia0[6] = {1, 92, 1, 66, 89, 0 };
int pozicia1[6] = {1, 92, 1, 66, 89, 0 };
int pozicia2[6] = {1, 92, 1, 66, 89, 0 };
int pozicia3[6] = {1, 92, 1, 66, 89, 0 };

int *pozicie[] = { pozicia0, pozicia1, pozicia2, pozicia3 };


unsigned long cas_kroku[POCET_ROBOTOV][MAX_POCET_KROKOV] = {{ 1500, 3500, 6800, 8000, 9800, 13400, 14400, 18500, 25100, 27100,28000, 29150,31150,32150,33150 },
                                                         { 1500, 3500, 6800, 8000, 9800, 13400, 16400, 20500, 23100, 29100,30000, 29150,31150,32150,33150 },
                                                         { 1500, 3500, 6800, 8000, 9800, 13400, 16400, 20500, 23100, 29100,30000, 29150,31150,32150,33150 } };
int prikaz_kroku[POCET_ROBOTOV][MAX_POCET_KROKOV] = {
                                 { RAMENO,
                                   SIKMO,
                                   OTOCKA_VPRAVO, 
                                   SIKMO,
                                   OTOCKA_VPRAVO,
                                   SIKMO,
                                   RAMENO,
                                   OBLUCIK_VLAVO,
                                   OBLUCIK_VPRAVO,
                                   ZASTAV,
                                   RAMENO,
                                   OBLUCIK_VLAVO,
                                   OBLUCIK_VPRAVO,
                                   OBLUCIK_VLAVO,
                                   OBLUCIK_VPRAVO
                                   },
                                 { RAMENO,
                                   SIKMO,
                                   OTOCKA_VPRAVO, 
                                   SIKMO,
                                   OTOCKA_VPRAVO,
                                   SIKMO,
                                   RAMENO,
                                   OBLUCIK_VLAVO,
                                   OBLUCIK_VPRAVO,
                                   ZASTAV,
                                   RAMENO,
                                   OBLUCIK_VLAVO,
                                   OBLUCIK_VPRAVO,
                                   OBLUCIK_VLAVO,
                                   OBLUCIK_VPRAVO
                                   },
                                 { RAMENO,
                                   SIKMO,
                                   OTOCKA_VPRAVO, 
                                   SIKMO,
                                   OTOCKA_VPRAVO,
                                   SIKMO,
                                   RAMENO,
                                   OBLUCIK_VLAVO,
                                   OBLUCIK_VPRAVO,
                                   ZASTAV,
                                   RAMENO,
                                   OBLUCIK_VLAVO,
                                   OBLUCIK_VPRAVO,
                                   OBLUCIK_VLAVO,
                                   OBLUCIK_VPRAVO
                                   } };
int param1[POCET_ROBOTOV][MAX_POCET_KROKOV] = {           // rychlost alebo index pozicie ramena
          { 1,
            700,
            800,
            800,
            800,
            800,
            2,
            800,
            800,
            NIC,       
            3,
            800,
            800,
            800,
            800},            // pozicia1
          { 1,
            700,
            800,
            800,
            800,
            800,
            2,
            800,
            800,
            NIC,       
            3,
            800,
            800,
            800,
            800},            // pozicia1
          { 1,
            700,
            800,
            800,
            800,
            800,
            2,
            800,
            800,
            NIC,       
            3,
            800,
            800,
            800,
            800}              // pozicia1
};
int param2[POCET_ROBOTOV][MAX_POCET_KROKOV] = {           // druha rychlost alebo uhol
          {
            NIC,
            0,
            NIC,
            180,
            NIC,
            90,
            NIC,
            200,
            200,
            NIC,
            NIC,
            200,
            200,
            200,
            200},
{
            NIC,
            0,
            NIC,
            180,
            NIC,
            90,
            NIC,
            200,
            200,
            NIC,
            NIC,
            200,
            200,
            200,
            200},            
{
            NIC,
            0,
            NIC,
            180,
            NIC,
            90,
            NIC,
            200,
            200,
            NIC,
            NIC,
            200,
            200,
            200,
            200}
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

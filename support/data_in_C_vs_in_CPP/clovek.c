#include <stdio.h>

#define POCET_LUDI 5

int roknar[POCET_LUDI] = { 1990, 2010, 2005, 2008, 2011 };
char *priezvisko[POCET_LUDI] = { "Hrasko", "Makovy", "Fazula", "Mrkva", "Kukurica"};
char *skola[POCET_LUDI] = { "Frantisek", "Ursula", "Alexia", "Gamca", "Saru" };
char *krajina[POCET_LUDI] = { "FR", "UZ", "AZ", "GR", "IT" };

void vypis(int n)
{
    printf("%s, nar. %d, student %s, z krajiny %s\n", priezvisko[n], roknar[n], skola[n], krajina[n]);
}

void main()
{
    printf("zoznam ludi:\n\n");
    for (int i = 0; i < POCET_LUDI; i++)
        vypis(i);
    priezvisko[1][1] = 'A';
}

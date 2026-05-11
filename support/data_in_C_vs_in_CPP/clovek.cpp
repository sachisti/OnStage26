#include <stdio.h>

#define POCET_LUDI 6

class clovek {

protected:
    int roknar;
    const char *priezvisko;
    const char *skola;
    const char *krajina;

public:
    clovek(int r, const char *p, const char *s, const char *k)
    {
        roknar = r;
        priezvisko = p;
        skola = s;
        krajina = k;
    }

    void virtual vypis() 
    {
        printf("%s, nar. %d, student %s, z krajiny %s\n",
               priezvisko, roknar, skola, krajina);
    }
};

class ziak : public clovek {
    int rocnik;

public:
    ziak(int r, const char *p, const char *s, const char *k, int roc) : clovek(r, p, s, k)
    {
       rocnik = roc;
    }
    void virtual vypis() 
    {
        printf("ziak %s, nar. %d, student %s, z krajiny %s, rocnik %d\n",
               priezvisko, roknar, skola, krajina, rocnik);
    }
};


clovek *ludia[POCET_LUDI] = {
    new clovek(1990, "Hrasko", "Frantiska", "FR"),
    new clovek(2010, "Makovy", "Ursula", "UZ"),
    new clovek(2005, "Fazula", "Alexia", "AZ"),
    new ziak(2009, "Petrzlen", "Adlerka", "SK", 3),
    new clovek(2008, "Mrkva", "Gamca", "GR"),
    new clovek(2011, "Kukurica", "Sura", "IT")
};

int main()
{
    printf("zoznam ludi:\n\n");

    for (int i = 0; i < POCET_LUDI; i++)
        ludia[i]->vypis();

    return 0;
}

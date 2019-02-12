#ifndef SI_H
#define SI_H

#include "types.h"
#include "Box2DCar.h"
#include "car.h"

class SI : public Car
{
    private:
        void przyspieszenie() { Car::SetKey(1); }
        void hamowanie() { Car::SetKey(0); }
        void skretL() { Car::SetKey(2); }
        void skretP() { Car::SetKey(3); }

        double DocelowyKat;
        int AktualnyFragment;
        const static int nMinV = 200;
        pt::Point pos;
        int indeks;
        //bool* fragmenty;
        int nKat;
        int skret;
        bool b;
        int type;
        float roznica;
        float lastTurn;
        zakret zakrety[2];
        kolizja kol;
        int steerlevel;

        bool checkCollisions();
        inline void wyrownanie();

    public:
        bool AktualizujPozycje(const mapa* tTrasa);
        SI(sf::Image& img, b2World* world, mapa* tTrasa, pt::Point pos);
        ~SI();
};

#endif

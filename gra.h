#ifndef GRA_H
#define GRA_H

#include <SFML/Graphics.hpp>
#include "car.h"
#include "game.h"
#include "types.h"
#include <fstream>
#include <string>
#include <Box2D/Box2D.h>

using namespace sf;

class Gra
{
    private:
        int nTrasa;
        int nCar;
        int nOkrazenia;
        int nLiczbaTras;
        int nLiczbaSamochodow;
        Image* imgTrasy;
        Image* imgSamochody;
        mapa* tTrasy;
        samochod* samochody;
        std::fstream NazwyTras;
        Image background;
        Sprite sBackground;
        Image whiteBackground;
        Shape PrzyciskP;
        Shape PrzyciskL;
        float t1[4];
        float t2[4];
        bool KoniecGry;
        int nLiczbaPrzeciwnikow;
        bool Dalej;

        Game* game1;
        b2World world;

    public:
        Gra(RenderWindow& window);
        ~Gra();

        bool Menu(const Input& wejscie, Event event, RenderWindow& window);
        bool WyborTrasy(const Input& wejscie, Event event, RenderWindow& window);
        bool WyborSamochodu(const Input& wejscie, Event event, RenderWindow& window);
        bool game(const Input& wejscie, Event event, RenderWindow& window);
        bool Okrazenia(const Input& wejscie, Event event, RenderWindow& window);
        bool Koniec(float LacznyCzas, float NajlepszeOkrazenie, const Input& wejscie, Event event, RenderWindow& window);
        bool LiczbaPrzeciwnikow(const Input& wejscie, Event event, RenderWindow& window);
        bool Main(const Input& wejscie, Event event, RenderWindow& window);
};

#endif

#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "car.h"
#include "SI.h"
#include "types.h"
#include <fstream>
#include <string>
#include <Box2D/Box2D.h>

using namespace sf;
using namespace std;

class Game
{
    private:
        int nTrasa;
        int nCar;
        int nOkrazenia;
        int nLiczbaPrzeciwnikow;

        int pos[2];
        int indeks;
        int nOkrazenie;
        float *fCzasyOkrazen;
        float NajlepszeOkrazenie;
        float LacznyCzas;
        double dt;

        Car* car1;
        String str;
        String str2;
        String str3;
        String str4;
        String str5;
        mapa tTrasa;
        char ch[50];
        Clock czas;
        SI** SIcars;
        View& view;

        b2Body* m_body;
        b2Body* m_body1;
        b2Body* m_body2;
        b2Body* m_body3;

        bool Odliczanie(const Input& wejscie, Event event, RenderWindow& window);
        int MenuPauza(const Input& wejscie, Event event, RenderWindow& window);

    public:
        Game(b2World& world, int okrazenia, int liczbaprzeciwnikow, Image& img, RenderWindow& window, int trasa, string strNazwaPng, char* chNazwaTxt);
        ~Game();

        int Gra(b2World& world, const Input& wejscie, Event event, RenderWindow& window, float* u1, float* u2);
};

#endif

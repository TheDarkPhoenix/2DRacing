#ifndef TYPES_H
#define TYPES_H

#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include "point.h"

using namespace sf;

struct fragment
{
    int nPozycja[2];
    int nTyp;
    char chInf;
    int nKtoryPunkt;
    int siInf;
};

struct zakret
{
    pt::Point pos;
    int type;
    zakret(pt::Point pos1, int type1){pos = pos1; type = type1;};
    zakret(){pos = 0; type = 0;};
};

struct mapa
{
    char chNazwaTxt[100];
    std::string strNazwaPng;
    Image img;
    Sprite trasa;
    fragment* aTablicaFragmentow;
    int nRozmiary[2];
    int nRozmiaryFragmentu[2];
    std::fstream fTrasa;
    int nLiczbaPunktowKontrolnych;
    bool* aPunktyKontrolne;
    std::vector<zakret> zakrety;
    ~mapa() {if (aPunktyKontrolne) delete[] aPunktyKontrolne; if (aTablicaFragmentow) delete[] aTablicaFragmentow;}
};

struct samochod
{
    std::string strNazwaPng;
    Sprite car;
};

struct kolizja
{
    bool C;
    bool Rb;
    bool Lb;
    bool Fb;
    bool Bb;
    int Rt;     ///1 - static 2 - dynamic
    int Lt;
    int Ft;
    int Bt;
};

#endif

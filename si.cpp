#include "si.h"
#include <iostream>

using namespace std;

SI::SI(sf::Image& img, b2World* world, mapa* tTrasa, pt::Point pos) : Car(img, world, pos)
{
    //fragmenty = new bool[tTrasa->nRozmiary[0] * tTrasa->nRozmiary[1]];
    /*for (int i = 0; i < tTrasa->nRozmiary[0] * tTrasa->nRozmiary[1]; ++i)
        fragmenty[i] = false;*/
    DocelowyKat = 0;
    AktualnyFragment = 0;
    skret = 1;
    zakrety[0] = tTrasa->zakrety[0];
    zakrety[1] = tTrasa->zakrety[1];
    b = false;
    type = 0;
    lastTurn = 0;
}

SI::~SI()
{
    /*if (fragmenty)
        delete[] fragmenty;*/
}

/*void SI::Set(mapa* tTrasa, pt::Point pos)
{


}*/

bool SI::AktualizujPozycje(const mapa* tTrasa)
{
    if (!(Car::GetSprPos().x > 0 && Car::GetSprPos().x < tTrasa->trasa.GetSize().x && Car::GetSprPos().y > 0 && Car::GetSprPos().y < tTrasa->trasa.GetSize().y))
        return false;
    steerlevel = 16;
    pos.x = static_cast<int>((Car::GetSprPos().x - (static_cast<int>(Car::GetSprPos().x) % tTrasa->nRozmiaryFragmentu[0])) / tTrasa->nRozmiaryFragmentu[0]);
    pos.y = static_cast<int>((Car::GetSprPos().y - (static_cast<int>(Car::GetSprPos().y) % tTrasa->nRozmiaryFragmentu[1])) / tTrasa->nRozmiaryFragmentu[1]);
    if (pos.y == 1)
        indeks = tTrasa->nRozmiary[0] + pos.x;
    else if (pos.y > 1)
        indeks = (pos.y * tTrasa->nRozmiary[0]) + pos.x;
    else
        indeks = pos.x;
    if (indeks > tTrasa->nRozmiary[0] * tTrasa->nRozmiary[1] || indeks < 0)
        return false;
    if (tTrasa->aTablicaFragmentow[indeks].nTyp == 0)
    {
        if (Car::GetSpeed() < nMinV)
            przyspieszenie();
        if ((DocelowyKat == 0 && GetSprRot() > 5 && GetSprRot() < 180) || GetSprRot() > DocelowyKat+5)
        {
            skretL();
        }
        else if ((DocelowyKat == 0 && GetSprRot() < 355 && GetSprRot() > 180) || GetSprRot() < DocelowyKat-5)
        {
            skretP();
        }
    }
    if (AktualnyFragment != indeks)
    {
        if (zakrety[0].pos == pos)
        {
            if (tTrasa->aTablicaFragmentow[indeks].siInf == 3)
            {
                DocelowyKat += 90;
                if (DocelowyKat >= 360)
                    DocelowyKat = fmod(DocelowyKat, 360);
            }
            else if (tTrasa->aTablicaFragmentow[indeks].siInf == 2)
            {
                if (DocelowyKat >= 90)
                    DocelowyKat -= 90;
                else if (DocelowyKat == 0)
                    DocelowyKat = 270;
            }
            b = true;
        }
        AktualnyFragment = indeks;
    }
    if (DocelowyKat == 0 && GetSprRot() > 180)
        roznica = 360 - GetSprRot();
    else if (DocelowyKat == 90 && GetSprRot() > 180)
        roznica = 450 - GetSprRot();
    else if (DocelowyKat == 270 && GetSprRot() < 180)
        roznica = -GetSprRot() - 90;
    else
        roznica = DocelowyKat - GetSprRot();

    if (tTrasa->aTablicaFragmentow[indeks].siInf == 1 || tTrasa->aTablicaFragmentow[indeks].siInf == 4)
    {
        if (b)
        {
            if (skret + 1 < tTrasa->nLiczbaPunktowKontrolnych)
                ++skret;
            else
                skret = 0;
            zakrety[0] = zakrety[1];
            zakrety[1] = tTrasa->zakrety[skret];
            type = 0;
            b = false;
        }
        przyspieszenie();
        if (DocelowyKat == 0)
        {
            //cout << Car::car.GetPosition().x << ' ' << pos.x*tTrasa->nRozmiaryFragmentu[0] << endl;
            if (zakrety[1].type == 2)
            {
                if (Car::GetSprPos().x - pos.x*tTrasa->nRozmiaryFragmentu[0] < 90)
                {
                    if (lastTurn < 2)
                    {
                        skretP();
                        steerlevel = 8;
                    }
                    else if (lastTurn < 3)
                        wyrownanie();
                }
                else
                    wyrownanie();
            }
            else if (zakrety[1].type == 3)
            {
                if (Car::GetSprPos().x - pos.x*tTrasa->nRozmiaryFragmentu[0] > 60)
                {
                    if (lastTurn > -2)
                    {
                        skretL();
                        steerlevel = 8;
                    }
                    else if (lastTurn > -3)
                        wyrownanie();
                }
                else
                    wyrownanie();
            }
        }
        else if (DocelowyKat == 180)
        {
            if (zakrety[1].type == 2)
            {
                if (Car::GetSprPos().x - pos.x*tTrasa->nRozmiaryFragmentu[0] > 60)
                {
                    if (lastTurn < 2)
                    {
                        skretP();
                        steerlevel = 8;
                    }
                    else if (lastTurn < 3)
                        wyrownanie();
                }
                else
                    wyrownanie();
            }
            else if (zakrety[1].type == 3)
            {
                if (Car::GetSprPos().x - pos.x*tTrasa->nRozmiaryFragmentu[0] < 90)
                {
                    if (lastTurn > -2)
                    {
                        skretL();
                        steerlevel = 8;
                    }
                    else if (lastTurn > -3)
                        wyrownanie();
                }
                else
                    wyrownanie();
            }
        }
        else if (DocelowyKat == 90)
        {
            //cout << pos.y*tTrasa->nRozmiaryFragmentu[1] << ' ' << Car::car.GetPosition().y << endl;
            if (zakrety[1].type == 2)
            {
                if (Car::GetSprPos().y - pos.y*tTrasa->nRozmiaryFragmentu[1] > 60)
                {
                    if (lastTurn < 2)
                    {
                        skretP();
                        steerlevel = 8;
                    }
                    else if (lastTurn < 3)
                        wyrownanie();
                }
                else
                    wyrownanie();
            }
            else if (zakrety[1].type == 3)
            {
                if (Car::GetSprPos().y - pos.y*tTrasa->nRozmiaryFragmentu[1] < 90)
                {
                    if (lastTurn > -2)
                    {
                        skretL();
                        steerlevel = 8;
                    }
                    else if (lastTurn > -3)
                        wyrownanie();
                }
                else
                    wyrownanie();
            }
        }
        else if (DocelowyKat == 270)
        {
            if (zakrety[1].type == 2)
            {
                if (Car::GetSprPos().y - pos.y*tTrasa->nRozmiaryFragmentu[1] < 90)
                {
                    if (lastTurn < 2)
                    {
                        skretP();
                        steerlevel = 8;
                    }
                    else if (lastTurn < 3)
                        wyrownanie();
                }
                else
                    wyrownanie();
            }
            else if (zakrety[1].type == 3)
            {
                if (Car::GetSprPos().y - pos.y*tTrasa->nRozmiaryFragmentu[1] > 60)
                {
                    if (lastTurn > -2)
                    {
                        skretL();
                        steerlevel = 8;
                    }
                    else if (lastTurn > -3)
                        wyrownanie();
                }
                else
                    wyrownanie();
            }
        }
        lastTurn = roznica;
        /*if (car.GetRotation() > DocelowyKat + 5 && car.GetRotation() < DocelowyKat + 85)
        {
            skretP();
        }
        else if (((DocelowyKat == 0 && car.GetRotation() > 355) && car.GetRotation() < 175) || (car.GetRotation() < DocelowyKat-5 && car.GetRotation() > DocelowyKat - 85))
        {
            skretL();
        }*/
    }
    else if (tTrasa->aTablicaFragmentow[indeks].siInf == 2)
    {
        /*if (Car::GetSpeed() >= nMinV)
            hamowanie();
        else */if (Car::GetSpeed() < nMinV)
            przyspieszenie();
        //cout << '1' << endl;
        if ((DocelowyKat == 270 && GetSprRot() < 90) || (GetSprRot() > DocelowyKat+5 && GetSprRot() < DocelowyKat + 179))
        {
            skretP();
        }
        else if ((DocelowyKat == 0 && GetSprRot() < 180) || (GetSprRot() < DocelowyKat-5 && GetSprRot() > DocelowyKat - 79))
        {
            skretL();
        }
    }
    else if (tTrasa->aTablicaFragmentow[indeks].siInf == 3)
    {
        /*if (Car::GetSpeed() >= nMinV)
            hamowanie();
        else */if (Car::GetSpeed() < nMinV)
            przyspieszenie();

        if ((DocelowyKat == 0 && GetSprRot() < 180) || GetSprRot() < DocelowyKat-5)
        {
            skretL();
        }
        else if ((DocelowyKat == 270 && GetSprRot() < 90) || (GetSprRot() > DocelowyKat+5 && GetSprRot() < DocelowyKat + 179))
        {
            skretP();
        }
    }
    //checkCollisions();
    Car::Update(steerlevel);
}

bool SI::checkCollisions()
{
    kol = Car::Collision();
    if (!kol.C)
        return false;
    if (kol.Rb && kol.Lb)
    {

    }
    else if (kol.Rb)
    {

    }
    else if (kol.Lb)
    {

    }

    if (kol.Fb && kol.Bb)
    {

    }
    else if (kol.Fb)
    {

    }
    else if (kol.Bb)
    {

    }
    return true;
}

void SI::wyrownanie()
{
    if (roznica < -2 || roznica > 180)
    {
        skretP();
        steerlevel = 4;
    }
    else if (roznica > 2 || roznica < - 180)
    {
        skretL();
        steerlevel = 4;
    }
    else if (lastTurn - roznica > 0.4)
    {
        skretL();
        steerlevel = 2;
    }
    else if (lastTurn - roznica < -0.4)
    {
        skretP();
        steerlevel = 2;
    }
}

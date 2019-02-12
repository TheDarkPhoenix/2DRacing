#ifndef CAR_H
#define CAR_H
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include "Box2DCar.h"
#include "point.h"

using namespace sf;

class Car
{
    private:
        double nV;
        int a;
        pt::Point kierunek;
        pt::Point fPozycja;
        double angle;
        bool bKlawisze[4];
        TDCar* b2car;
        Sprite car;

    protected:
        kolizja Collision() {return b2car->collision();};

    public:
        Car(sf::Image& img, b2World* world, pt::Point pos);
        ~Car();

        Sprite GetCar() const {return car;};
        Vector2f GetSprPos() const {return car.GetPosition();};
        float GetSprRot() const {return car.GetRotation();};

        bool Update(int steerlevel = 16);

        bool SetKey(int i) {if (i >= 0 && i < 4)bKlawisze[i] = true;};
        pt::Point GetPosition() const {return fPozycja;};
        void UpdatePosition() {fPozycja = pt::PointMake(car.GetPosition().x, car.GetPosition().y);};
        double GetAngle() const {return angle;};
        pt::Point GetShift() const {return kierunek;};
        double GetSpeed () const {return nV;};
        void SetAngle() {angle = 0.0;};
        bool IsKeyDown(int i) const {if (i >= 0 && i < 4) if (bKlawisze[i]) return true; else return false;}
};

#endif

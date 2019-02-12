#include "car.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Car::Car(sf::Image& img, b2World* world, pt::Point pos)
{
    b2car = new TDCar(world, pos);
    car.SetImage(img);
    car.Resize(img.GetWidth()/14, img.GetHeight()/14);
    nV = 0;
    angle = 0.0;
    car.SetCenter(img.GetWidth()/2, img.GetHeight()/2);
    for (int i = 0; i < 4; ++i)
        bKlawisze[i] = false;
    car.SetPosition(b2car->m_body->GetPosition().x*10, b2car->m_body->GetPosition().y*10);
    fPozycja = pt::PointMake(car.GetPosition().x, car.GetPosition().y);
    kierunek = 0;
    a = 2500 * 1.5f;
    bKlawisze[0] = 0;
    bKlawisze[1] = 0;
    bKlawisze[2] = 0;
    bKlawisze[3] = 0;
}

Car::~Car()
{
    if (b2car)
        delete b2car;
}

bool Car::Update(int steerlevel)
{
    if (b2car)
        b2car->update(bKlawisze, steerlevel);
    if (b2car)
        angle = -(b2car->m_body->GetAngle())*57.2957795 - car.GetRotation();
    car.Rotate(angle);
    if (b2car)
        nV = b2car->m_body->GetLinearVelocity().Normalize()*10;
    car.SetPosition(b2car->m_body->GetPosition().x*10, b2car->m_body->GetPosition().y*10);
    kierunek.x = b2car->m_body->GetPosition().x*10-fPozycja.x;
    kierunek.y = b2car->m_body->GetPosition().y*10-fPozycja.y;
    fPozycja.x += kierunek.x;
    fPozycja.y += kierunek.y;
    bKlawisze[0] = 0;
    bKlawisze[1] = 0;
    bKlawisze[2] = 0;
    bKlawisze[3] = 0;
}

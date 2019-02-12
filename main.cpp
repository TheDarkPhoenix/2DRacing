/*
    Author: Maciej Stêpieñ
    Copyright(c) 2012-2013 Maciej Stêpieñ
    Version: pre-beta
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdio>
#include <fstream>
#include "gra.h"

sf::Event event;
sf::RenderWindow window(sf::VideoMode(640, 480, 32), "2DRacing", sf::Style::Close);
const sf::Input& wejscie = window.GetInput();
Gra gra1(window);

int main()
{
    gra1.Main(wejscie, event, window);
    return 0;
}

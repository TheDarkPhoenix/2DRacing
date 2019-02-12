#include "gra.h"
#include "si.h"
#include <SFML/Graphics.hpp>
#include <cstdio>
#include <string>
#include <cmath>
#include <fstream>

using namespace sf;
using namespace std;

Gra::Gra(RenderWindow& window) : world(b2Vec2(0, 0))
{
    window.SetFramerateLimit(70);

    Gra::NazwyTras.open("Trasy/trasy.txt");
    if (Gra::NazwyTras.good())
    {
        Gra::NazwyTras >> Gra::nLiczbaTras;
    }
    Gra::tTrasy = new mapa[Gra::nLiczbaTras];
    Gra::imgTrasy = new Image[Gra::nLiczbaTras];
    for (int i = 0; i < Gra::nLiczbaTras; ++i)
    {
        Gra::NazwyTras >> tTrasy[i].chNazwaTxt >> tTrasy[i].strNazwaPng;
        Gra::imgTrasy[i].LoadFromFile(tTrasy[i].strNazwaPng);
        tTrasy[i].trasa.SetImage(imgTrasy[i]);
        tTrasy[i].trasa.Resize(window.GetWidth()/2, window.GetWidth()/2);
        tTrasy[i].trasa.SetCenter(imgTrasy[i].GetWidth()/2, imgTrasy[i].GetHeight()/2);
        tTrasy[i].trasa.SetPosition(window.GetWidth()/2, window.GetHeight()/2);
    }
    if (Gra::NazwyTras.good())
        Gra::NazwyTras >> Gra::nLiczbaSamochodow;
    Gra::samochody = new samochod[Gra::nLiczbaSamochodow];
    Gra::imgSamochody = new Image[Gra::nLiczbaSamochodow];
    for (int i = 0; i < Gra::nLiczbaSamochodow; ++i)
    {
        Gra::NazwyTras >> Gra::samochody[i].strNazwaPng;
        Gra::imgSamochody[i].LoadFromFile(Gra::samochody[i].strNazwaPng);
        samochody[i].car.SetImage(imgSamochody[i]);
        samochody[i].car.Resize(imgSamochody[i].GetWidth()/1.5, imgSamochody[i].GetHeight()/1.5);
        samochody[i].car.SetCenter(imgSamochody[i].GetWidth()/2, imgSamochody[i].GetHeight()/2+50);
        samochody[i].car.SetPosition(window.GetWidth()/2, window.GetHeight()/2);
    }
    Gra::background.LoadFromFile("Obrazy/background.png");
    Gra::whiteBackground.Create(640, 480, Color::White);
    t1[0] = window.GetWidth() *0.9;
    t1[1] = window.GetHeight()/2 - 15;
    t1[2] = window.GetWidth() - 15;
    t1[3] = window.GetHeight()/2+15;

    t2[0] = window.GetWidth() / 10;
    t2[1] = window.GetHeight()/2 - 15;
    t2[2] = 15;
    t2[3] = window.GetHeight()/2+15;

    PrzyciskP.AddPoint(t1[0], t1[1]);
    PrzyciskP.AddPoint(t1[0], t1[1]+30);
    PrzyciskP.AddPoint(t1[2], t1[3]-15);

    PrzyciskL.AddPoint(t2[0], t2[1]);
    PrzyciskL.AddPoint(t2[2], t2[1]+15);
    PrzyciskL.AddPoint(t2[0], t2[3]);

    PrzyciskP.SetColor(Color::Red);
    PrzyciskL.SetColor(Color::Red);
    KoniecGry = false;
    Dalej = false;

    b2BodyDef bodyDef;
    b2Body* m_groundBody = world.CreateBody( &bodyDef );
    b2PolygonShape polygonShape;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polygonShape;
    fixtureDef.isSensor = true;

    polygonShape.SetAsBox(75, 105, b2Vec2(0,0), 0 );
    b2Fixture* groundAreaFixture = m_groundBody->CreateFixture(&fixtureDef);
    groundAreaFixture->SetUserData( new GroundAreaFUD( 0.5f, false ) );
}

Gra::~Gra()
{
    if (tTrasy)
        delete[] tTrasy;
    if (imgTrasy)
        delete[] imgTrasy;
    if (samochody)
        delete[] samochody;
    if (imgSamochody)
        delete[] imgSamochody;
}

bool Gra::Menu(const Input& wejscie, Event event, RenderWindow& window)
{
    Gra::sBackground.SetImage(Gra::background);
    String tekst1;
    String tekst2;
    String tekst3;
    tekst1.SetSize(20);
    tekst2.SetSize(20);
    tekst3.SetSize(20);
    tekst1.SetText("Start gry");
    tekst2.SetText("Start gry");
    tekst3.SetText("Wyjscie");
    tekst1.SetCenter(tekst1.GetRect().Right/2, tekst1.GetRect().Bottom/2);
    tekst2.SetCenter(tekst2.GetRect().Right/2, tekst2.GetRect().Bottom/2);
    tekst3.SetCenter(tekst3.GetRect().Right/2, tekst3.GetRect().Bottom/2);
    tekst1.SetPosition(window.GetWidth()/2, window.GetHeight()/3+25);
    tekst2.SetPosition(window.GetWidth()/2, window.GetHeight()/3 + window.GetHeight()/9 + 100);
    tekst3.SetPosition(window.GetWidth()/2, window.GetHeight()/3+ (window.GetHeight()/9)*2 +80+25);
    float p1[4] = {tekst1.GetRect().Left, tekst1.GetRect().Top,  tekst1.GetRect().Right, tekst1.GetRect().Bottom};
    float p2[4] = {tekst2.GetRect().Left, tekst2.GetRect().Top,  tekst2.GetRect().Right, tekst2.GetRect().Bottom};
    float p3[4] = {tekst3.GetRect().Left, tekst3.GetRect().Top,  tekst3.GetRect().Right, tekst3.GetRect().Bottom};
    while (window.IsOpened())
    {
        while (window.GetEvent(event))
        {
            if (event.Type == Event::Closed)
            {
                window.Close();
                return false;
            }
            if (wejscie.IsKeyDown(Key::Escape))
            {
                window.Close();
                return false;
            }
            if (wejscie.IsMouseButtonDown(Mouse::Left))
            {
                if (wejscie.GetMouseX() >= p3[0] && wejscie.GetMouseX() <= p3[2] && wejscie.GetMouseY() >= p3[1] && wejscie.GetMouseY() <= p3 [3])
                {
                    window.Close();
                    return false;
                }
                if (wejscie.GetMouseX() >= p2[0] && wejscie.GetMouseX() <= p2[2] && wejscie.GetMouseY() >= p2[1] && wejscie.GetMouseY() <= p2 [3]);
                {
                    window.Clear();
                    return true;
                }
            }
        }
        if (wejscie.GetMouseX() >= p2[0] && wejscie.GetMouseX() <= p2[2] && wejscie.GetMouseY() >= p2[1] && wejscie.GetMouseY() <= p2 [3])
            tekst2.SetColor(Color::Blue);
        else
            tekst2.SetColor(Color::Black);
        if (wejscie.GetMouseX() >= p3[0] && wejscie.GetMouseX() <= p3[2] && wejscie.GetMouseY() >= p3[1] && wejscie.GetMouseY() <= p3 [3])
            tekst3.SetColor(Color::Blue);
        else
            tekst3.SetColor(Color::Black);
        window.Draw(Gra::sBackground);
        window.Draw(tekst2);
        window.Draw(tekst3);
        window.Display();
        window.Clear();
    }
    return true;
}

bool Gra::WyborTrasy(const Input& wejscie, Event event, RenderWindow& window)
{
    Gra::sBackground.SetImage(Gra::whiteBackground);
    int AktualnaTrasa = 3;
    String str;
    str.SetText("Wybierz");
    str.SetCenter(str.GetRect().Right/2, str.GetRect().Bottom/2);
    str.SetPosition(window.GetWidth()/2, window.GetHeight()*0.9);
    float p1[4] = {str.GetRect().Left, str.GetRect().Top,  str.GetRect().Right, str.GetRect().Bottom};
    while (window.IsOpened())
    {
        while (window.GetEvent(event))
        {
             if (event.Type == Event::Closed)
            {
                window.Close();
                return false;
            }
            if (wejscie.IsKeyDown(Key::Escape))
            {
                window.Close();
                return false;
            }
            if (wejscie.IsMouseButtonDown(Mouse::Left))
            {
                if (wejscie.GetMouseX() >= t1[0] && wejscie.GetMouseX() <= t1[2] && wejscie.GetMouseY() >= t1[1] && wejscie.GetMouseY() <= t1[3])
                {
                     window.Clear();
                     if (AktualnaTrasa + 1 < Gra::nLiczbaTras)
                        ++AktualnaTrasa;
                    else
                        AktualnaTrasa = 0;
                 }
                else if (wejscie.GetMouseX() >= t2[2] && wejscie.GetMouseX() <= t2[0] && wejscie.GetMouseY() >= t2[1] && wejscie.GetMouseY() <= t2[3])
                {
                    window.Clear();
                    if (AktualnaTrasa - 1 >= 0)
                        --AktualnaTrasa;
                    else
                        AktualnaTrasa = Gra::nLiczbaTras-1;
                }
                else if (wejscie.GetMouseX() >= p1[0] && wejscie.GetMouseX() <= p1[2] && wejscie.GetMouseY() >= p1[1] && wejscie.GetMouseY() <= p1[3])
                {
                    window.Clear();
                    nTrasa = AktualnaTrasa;
                    return true;
                }
            }
            if (wejscie.GetMouseX() >= t1[0] && wejscie.GetMouseX() <= t1[2] && wejscie.GetMouseY() >= t1[1] && wejscie.GetMouseY() <= t1[3])
                PrzyciskP.SetColor(Color::Green);
            else
                PrzyciskP.SetColor(Color::Red);
            if (wejscie.GetMouseX() >= t2[2] && wejscie.GetMouseX() <= t2[0] && wejscie.GetMouseY() >= t2[1] && wejscie.GetMouseY() <= t2[3])
                PrzyciskL.SetColor(Color::Green);
            else
                PrzyciskL.SetColor(Color::Red);
            if (wejscie.GetMouseX() >= p1[0] && wejscie.GetMouseX() <= p1[2] && wejscie.GetMouseY() >= p1[1] && wejscie.GetMouseY() <= p1[3])
                str.SetColor(Color::Blue);
            else
                str.SetColor(Color::Black);
            window.Draw(Gra::sBackground);
            window.Draw(tTrasy[AktualnaTrasa].trasa);
            window.Draw(str);
            window.Draw(PrzyciskL);
            window.Draw(PrzyciskP);
            window.Display();
            window.Clear();
        }
    }
    return true;
}

bool Gra::WyborSamochodu(const Input& wejscie, Event event, RenderWindow& window)
{
    Gra::sBackground.SetImage(Gra::whiteBackground);
    int AktualnySamochod = 0;
    String str;
    str.SetText("Wybierz");
    str.SetCenter(str.GetRect().Right/2, str.GetRect().Bottom/2);
    str.SetPosition(window.GetWidth()/2, window.GetHeight()*0.9);
    float p1[4] = {str.GetRect().Left, str.GetRect().Top,  str.GetRect().Right, str.GetRect().Bottom};
    while (window.IsOpened())
    {
        while (window.GetEvent(event))
        {
             if (event.Type == Event::Closed)
            {
                window.Close();
                return false;
            }
            if (wejscie.IsKeyDown(Key::Escape))
            {
                window.Close();
                return false;
            }
            if (wejscie.IsMouseButtonDown(Mouse::Left))
            {
                if (wejscie.GetMouseX() >= t1[0] && wejscie.GetMouseX() <= t1[2] && wejscie.GetMouseY() >= t1[1] && wejscie.GetMouseY() <= t1[3])
                {
                     window.Clear();
                     if (AktualnySamochod + 1 < nLiczbaSamochodow)
                        ++AktualnySamochod;
                    else
                        AktualnySamochod = 0;
                 }
                else if (wejscie.GetMouseX() >= t2[2] && wejscie.GetMouseX() <= t2[0] && wejscie.GetMouseY() >= t2[1] && wejscie.GetMouseY() <= t2[3])
                {
                    window.Clear();
                    if (AktualnySamochod - 1 >= 0)
                        --AktualnySamochod;
                    else
                        AktualnySamochod = nLiczbaSamochodow-1;
                }
                else if (wejscie.GetMouseX() >= p1[0] && wejscie.GetMouseX() <= p1[2] && wejscie.GetMouseY() >= p1[1] && wejscie.GetMouseY() <= p1[3])
                {
                    window.Clear();
                    nCar = AktualnySamochod;
                    return true;
                }
            }
            if (wejscie.GetMouseX() >= t1[0] && wejscie.GetMouseX() <= t1[2] && wejscie.GetMouseY() >= t1[1] && wejscie.GetMouseY() <= t1[3])
                PrzyciskP.SetColor(Color::Green);
            else
                PrzyciskP.SetColor(Color::Red);
            if (wejscie.GetMouseX() >= t2[2] && wejscie.GetMouseX() <= t2[0] && wejscie.GetMouseY() >= t2[1] && wejscie.GetMouseY() <= t2[3])
                PrzyciskL.SetColor(Color::Green);
            else
                PrzyciskL.SetColor(Color::Red);
            if (wejscie.GetMouseX() >= p1[0] && wejscie.GetMouseX() <= p1[2] && wejscie.GetMouseY() >= p1[1] && wejscie.GetMouseY() <= p1[3])
                str.SetColor(Color::Blue);
            else
                str.SetColor(Color::Black);
            window.Draw(Gra::sBackground);
            window.Draw(samochody[AktualnySamochod].car);
            window.Draw(PrzyciskL);
            window.Draw(PrzyciskP);
            window.Draw(str);
            window.Display();
            window.Clear();
        }
    }
    return true;
}

bool Gra::Okrazenia(const Input& wejscie, Event event, RenderWindow& window)
{
    char ch[50];
    nOkrazenia = 1;
    Gra::sBackground.SetImage(Gra::whiteBackground);
    String str;
    String str2;
    String str3;
    sprintf(ch, "%i", nOkrazenia);
    str2.SetText(ch);
    str.SetText("Wybierz");
    str3.SetText("Liczba okrazen");
    str.SetCenter(str.GetRect().Right/2, str.GetRect().Bottom/2);
    str2.SetCenter(str2.GetRect().Right/2, str2.GetRect().Bottom/2);
    str3.SetCenter(str3.GetRect().Right/2, str3.GetRect().Bottom/2);
    str.SetPosition(window.GetWidth()/2, window.GetHeight()*0.9);
    str2.SetPosition(window.GetWidth()/2, window.GetHeight()/2);
    str3.SetPosition(window.GetWidth()/2, window.GetHeight()/10);
    str2.SetColor(Color::Black);
    str3.SetColor(Color::Black);
    float p1[4] = {str.GetRect().Left, str.GetRect().Top,  str.GetRect().Right, str.GetRect().Bottom};
    while (window.IsOpened())
    {
        while (window.GetEvent(event))
        {
             if (event.Type == Event::Closed)
            {
                window.Close();
                return false;
            }
            if (wejscie.IsKeyDown(Key::Escape))
            {
                window.Close();
                return false;
            }
            if (wejscie.IsMouseButtonDown(Mouse::Left))
            {
                if (wejscie.GetMouseX() >= t1[0] && wejscie.GetMouseX() <= t1[2] && wejscie.GetMouseY() >= t1[1] && wejscie.GetMouseY() <= t1[3])
                {
                    window.Clear();
                    ++nOkrazenia;
                    sprintf(ch, "%i", nOkrazenia);
                    str2.SetText(ch);
                 }
                else if (wejscie.GetMouseX() >= t2[2] && wejscie.GetMouseX() <= t2[0] && wejscie.GetMouseY() >= t2[1] && wejscie.GetMouseY() <= t2[3])
                {
                    window.Clear();
                    if (nOkrazenia - 1 > 1)
                        --nOkrazenia;
                    else
                        nOkrazenia = 1;
                    sprintf(ch, "%i", nOkrazenia);
                    str2.SetText(ch);
                }
                else if (wejscie.GetMouseX() >= p1[0] && wejscie.GetMouseX() <= p1[2] && wejscie.GetMouseY() >= p1[1] && wejscie.GetMouseY() <= p1[3])
                {
                    window.Clear();
                    return true;
                }
            }
            if (wejscie.GetMouseX() >= t1[0] && wejscie.GetMouseX() <= t1[2] && wejscie.GetMouseY() >= t1[1] && wejscie.GetMouseY() <= t1[3])
                PrzyciskP.SetColor(Color::Green);
            else
                PrzyciskP.SetColor(Color::Red);
            if (wejscie.GetMouseX() >= t2[2] && wejscie.GetMouseX() <= t2[0] && wejscie.GetMouseY() >= t2[1] && wejscie.GetMouseY() <= t2[3])
                PrzyciskL.SetColor(Color::Green);
            else
                PrzyciskL.SetColor(Color::Red);
            if (wejscie.GetMouseX() >= p1[0] && wejscie.GetMouseX() <= p1[2] && wejscie.GetMouseY() >= p1[1] && wejscie.GetMouseY() <= p1[3])
                str.SetColor(Color::Blue);
            else
                str.SetColor(Color::Black);
            window.Draw(Gra::sBackground);
            window.Draw(PrzyciskL);
            window.Draw(PrzyciskP);
            window.Draw(str);
            window.Draw(str2);
            window.Draw(str3);
            window.Display();
            window.Clear();
        }
    }
    return true;
}

bool Gra::LiczbaPrzeciwnikow(const Input& wejscie, Event event, RenderWindow& window)
{
    char ch[50];
    int nPrzeciwnicy = 2;
    Gra::sBackground.SetImage(Gra::whiteBackground);
    String str;
    String str2;
    String str3;
    sprintf(ch, "%i", nPrzeciwnicy);
    str2.SetText(ch);
    str.SetText("Wybierz");
    str3.SetText("Liczba przeciwnikow");
    str.SetCenter(str.GetRect().Right/2, str.GetRect().Bottom/2);
    str2.SetCenter(str2.GetRect().Right/2, str2.GetRect().Bottom/2);
    str3.SetCenter(str3.GetRect().Right/2, str3.GetRect().Bottom/2);
    str.SetPosition(window.GetWidth()/2, window.GetHeight()*0.9);
    str2.SetPosition(window.GetWidth()/2, window.GetHeight()/2);
    str3.SetPosition(window.GetWidth()/2, window.GetHeight()/10);
    str2.SetColor(Color::Black);
    str3.SetColor(Color::Black);
    float p1[4] = {str.GetRect().Left, str.GetRect().Top,  str.GetRect().Right, str.GetRect().Bottom};
    while (window.IsOpened())
    {
        while (window.GetEvent(event))
        {
             if (event.Type == Event::Closed)
            {
                window.Close();
                return false;
            }
            if (wejscie.IsKeyDown(Key::Escape))
            {
                window.Close();
                return false;
            }
            if (wejscie.IsMouseButtonDown(Mouse::Left))
            {
                if (wejscie.GetMouseX() >= t1[0] && wejscie.GetMouseX() <= t1[2] && wejscie.GetMouseY() >= t1[1] && wejscie.GetMouseY() <= t1[3])
                {
                    window.Clear();
                    ++nPrzeciwnicy;
                    sprintf(ch, "%i", nPrzeciwnicy);
                    str2.SetText(ch);
                 }
                else if (wejscie.GetMouseX() >= t2[2] && wejscie.GetMouseX() <= t2[0] && wejscie.GetMouseY() >= t2[1] && wejscie.GetMouseY() <= t2[3])
                {
                    window.Clear();
                    if (nPrzeciwnicy - 1 > 0)
                        --nPrzeciwnicy;
                    else
                        nPrzeciwnicy = 0;
                    sprintf(ch, "%i", nPrzeciwnicy);
                    str2.SetText(ch);
                }
                else if (wejscie.GetMouseX() >= p1[0] && wejscie.GetMouseX() <= p1[2] && wejscie.GetMouseY() >= p1[1] && wejscie.GetMouseY() <= p1[3])
                {
                    window.Clear();
                    nLiczbaPrzeciwnikow = nPrzeciwnicy;
                    return true;
                }
            }
            if (wejscie.GetMouseX() >= t1[0] && wejscie.GetMouseX() <= t1[2] && wejscie.GetMouseY() >= t1[1] && wejscie.GetMouseY() <= t1[3])
                PrzyciskP.SetColor(Color::Green);
            else
                PrzyciskP.SetColor(Color::Red);
            if (wejscie.GetMouseX() >= t2[2] && wejscie.GetMouseX() <= t2[0] && wejscie.GetMouseY() >= t2[1] && wejscie.GetMouseY() <= t2[3])
                PrzyciskL.SetColor(Color::Green);
            else
                PrzyciskL.SetColor(Color::Red);
            if (wejscie.GetMouseX() >= p1[0] && wejscie.GetMouseX() <= p1[2] && wejscie.GetMouseY() >= p1[1] && wejscie.GetMouseY() <= p1[3])
                str.SetColor(Color::Blue);
            else
                str.SetColor(Color::Black);
            window.Draw(Gra::sBackground);
            window.Draw(PrzyciskL);
            window.Draw(PrzyciskP);
            window.Draw(str);
            window.Draw(str2);
            window.Draw(str3);
            window.Display();
            window.Clear();
        }
    }
    return true;
}

bool Gra::Koniec(float LacznyCzas, float NajlepszeOkrazenie, const Input& wejscie, Event event, RenderWindow& window)
{
    Gra::sBackground.SetImage(Gra::whiteBackground);
    char ch[50];
    String str;
    String str2;
    String str3;
    String str4;
    String str5;
    sprintf(ch, "%f", LacznyCzas);
    str.SetText("Laczny czas");
    str2.SetText("Najlepsze okrazenie");
    str3.SetText("Dalej");
    str4.SetText(ch);
    sprintf(ch, "%f", NajlepszeOkrazenie);
    str5.SetText(ch);
    str.SetCenter(str.GetRect().Right/2, str.GetRect().Bottom/2);
    str2.SetCenter(str2.GetRect().Right/2, str2.GetRect().Bottom/2);
    str3.SetCenter(str3.GetRect().Right/2, str3.GetRect().Bottom/2);
    str4.SetCenter(str4.GetRect().Right/2, str4.GetRect().Bottom/2);
    str5.SetCenter(str5.GetRect().Right/2, str5.GetRect().Bottom/2);
    str.SetPosition(window.GetWidth()/2, window.GetHeight()*0.25);
    str2.SetPosition(window.GetWidth()/2, window.GetHeight()*0.50);
    str3.SetPosition(window.GetWidth()/2, window.GetHeight()*0.75);
    str4.SetPosition(window.GetWidth()/2, window.GetHeight()*0.35);
    str5.SetPosition(window.GetWidth()/2, window.GetHeight()*0.60);
    float p3[4] = {str3.GetRect().Left, str3.GetRect().Top,  str3.GetRect().Right, str3.GetRect().Bottom};
    str.SetColor(Color::Black);
    str2.SetColor(Color::Black);
    str3.SetColor(Color::Black);
    str4.SetColor(Color::Black);
    str5.SetColor(Color::Black);
    while (window.IsOpened())
    {
        while (window.GetEvent(event))
        {
             if (event.Type == Event::Closed)
            {
                window.Close();
                return false;
            }
            if (wejscie.IsKeyDown(Key::Escape))
            {
                window.Close();
                return false;
            }
            if (wejscie.IsMouseButtonDown(Mouse::Left))
            {
                if (wejscie.GetMouseX() >= p3[0] && wejscie.GetMouseX() <= p3[2] && wejscie.GetMouseY() >= p3[1] && wejscie.GetMouseY() <= p3[3])
                {
                    Dalej = true;
                    return true;
                }
            }
        }
            if (wejscie.GetMouseX() >= p3[0] && wejscie.GetMouseX() <= p3[2] && wejscie.GetMouseY() >= p3[1] && wejscie.GetMouseY() <= p3[3])
                str3.SetColor(Color::Blue);
            else
                str3.SetColor(Color::Black);
            window.Draw(Gra::sBackground);
            window.Draw(str);
            window.Draw(str2);
            window.Draw(str3);
            window.Draw(str4);
            window.Draw(str5);
            window.Display();
            window.Clear();
        }
    return true;
}

bool Gra::Main(const Input& wejscie, Event event, RenderWindow& window)
{
    bool b = true;
    for(;;)
    {
        b = Menu(wejscie, event, window);
        if (!b)
            break;
        b = WyborTrasy(wejscie, event, window);
        if (!b)
            break;
        b = WyborSamochodu(wejscie, event, window);
        if (!b)
            break;
        b = Okrazenia(wejscie, event, window);
        if (!b)
            break;
        b = LiczbaPrzeciwnikow(wejscie, event, window);
        if (!b)
            break;
        b = game(wejscie, event, window);
        if (!b)
            break;
    }
    return true;
}

bool Gra::game(const Input& wejscie, Event event, RenderWindow& window)
{
    game1 = new Game(world, nOkrazenia, nLiczbaPrzeciwnikow, imgSamochody[nCar], window, nTrasa, tTrasy[nTrasa].strNazwaPng, tTrasy[nTrasa].chNazwaTxt);
    float* u1 = new float;
    float* u2 = new float;
    int j = game1->Gra(world, wejscie, event, window, u1, u2);
    View& View = window.GetDefaultView();
    View.Move(-View.GetRect().Left, -View.GetRect().Top);
    if (j == 0)
    {
        delete game1;
        delete u1;
        delete u2;
        return true;
    }
    else if (j == -1)
    {
        delete game1;
        delete u1;
        delete u2;
        return false;
    }
    else if (j == 1)
        Koniec(*u1, *u2, wejscie, event, window);
    delete game1;
    delete u1;
    delete u2;
    return true;
}

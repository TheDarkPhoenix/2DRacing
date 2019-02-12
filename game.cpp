#include "game.h"
#include <iostream>

Game::Game(b2World& world, int okrazenia, int liczbaprzeciwnikow, Image& img, RenderWindow& window, int trasa, string strNazwaPng, char* chNazwaTxt) : view(window.GetDefaultView())
{
    double pospoc[2] = {0.0, 0.0};
    nOkrazenia = okrazenia;
    nLiczbaPrzeciwnikow = liczbaprzeciwnikow;
    pos[0] = 0;
    pos[1] = 0;
    indeks = 0;
    nOkrazenie = 0;
    fCzasyOkrazen = new float[nOkrazenia];
    NajlepszeOkrazenie = 0.0f;
    LacznyCzas = 0.0f;
    dt = 0;
    str.SetScale(0.5, 0.5);
    str2.SetScale(0.5, 0.5);
    str3.SetScale(0.5, 0.5);
    str4.SetScale(0.5, 0.5);
    str5.SetScale(0.5, 0.5);
    nTrasa = trasa;
    tTrasa.img.LoadFromFile(strNazwaPng);
    tTrasa.fTrasa.open(chNazwaTxt);
    if (tTrasa.fTrasa.good())
    {
        tTrasa.fTrasa >> tTrasa.nRozmiary[0] >> tTrasa.nRozmiary[1];
        tTrasa.fTrasa >> tTrasa.nRozmiaryFragmentu[0] >> tTrasa.nRozmiaryFragmentu[1];
        tTrasa.fTrasa >> tTrasa.nLiczbaPunktowKontrolnych;
        tTrasa.aPunktyKontrolne = new bool[tTrasa.nLiczbaPunktowKontrolnych];
        for (int i = 0; i < tTrasa.nLiczbaPunktowKontrolnych; ++i)
            tTrasa.aPunktyKontrolne[i] = false;
        tTrasa.zakrety.resize(tTrasa.nLiczbaPunktowKontrolnych);
        tTrasa.aTablicaFragmentow = new fragment[tTrasa.nRozmiary[0] * tTrasa.nRozmiary[1]];
        for (int i = 0; i < tTrasa.nRozmiary[0] * tTrasa.nRozmiary[1]; ++i)
            if (!tTrasa.fTrasa.eof())
            {
                tTrasa.fTrasa >> tTrasa.aTablicaFragmentow[i].nPozycja[0] >> tTrasa.aTablicaFragmentow[i].nPozycja[1] >> tTrasa.aTablicaFragmentow[i].nTyp >> tTrasa.aTablicaFragmentow[i].chInf;
                if (tTrasa.aTablicaFragmentow[i].chInf == 'P')
                {
                    tTrasa.fTrasa >> tTrasa.aTablicaFragmentow[i].nKtoryPunkt;
                    --tTrasa.aTablicaFragmentow[i].nKtoryPunkt;
                }
                else if (tTrasa.aTablicaFragmentow[i].chInf == 'M')
                {
                    pospoc[0] = tTrasa.aTablicaFragmentow[i].nPozycja[0]/10 + 4;
                    pospoc[1] = (tTrasa.aTablicaFragmentow[i].nPozycja[1]+tTrasa.nRozmiaryFragmentu[1])/10;
                }
                tTrasa.fTrasa >> tTrasa.aTablicaFragmentow[i].siInf;
                if (tTrasa.aTablicaFragmentow[i].chInf == 'P')
                    tTrasa.zakrety[tTrasa.aTablicaFragmentow[i].nKtoryPunkt] = zakret(pt::PointMake(tTrasa.aTablicaFragmentow[i].nPozycja[0]/tTrasa.nRozmiaryFragmentu[0], tTrasa.aTablicaFragmentow[i].nPozycja[1]/tTrasa.nRozmiaryFragmentu[1]), tTrasa.aTablicaFragmentow[i].siInf);

            }
    }
    str4.SetText("Najlepsze okrazenie");
    tTrasa.trasa.SetImage(tTrasa.img);
    SIcars = new SI*[nLiczbaPrzeciwnikow];
    car1 = new Car(img, &world, pt::Point(pospoc[0]+6, pospoc[1]));
    std::cout << pospoc[0] << ' ' << pospoc[1] << std::endl;
    for (int i = 0; i < nLiczbaPrzeciwnikow; ++i)
    {
        if (i % 2 == 0)
            SIcars[i] = new SI(img, &world, &tTrasa, pt::Point(pospoc[0], pospoc[1] + ((i+1)*3)));
        else
            SIcars[i] = new SI(img, &world, &tTrasa, pt::Point(pospoc[0]+6, pospoc[1] + ((i+1)*3)));
        //SIcars[i]->car.SetRotation(0.0f);
    }
    b2Vec2 vs[4];
    vs[0].Set(0, 0);
    vs[3].Set((tTrasa.nRozmiary[0]*tTrasa.nRozmiaryFragmentu[0])/10, 0);
    vs[2].Set((tTrasa.nRozmiary[0]*tTrasa.nRozmiaryFragmentu[0])/10, (tTrasa.nRozmiary[1]*tTrasa.nRozmiaryFragmentu[1])/10);
    vs[1].Set(0, (tTrasa.nRozmiary[1]*tTrasa.nRozmiaryFragmentu[1])/10);
    /*b2ChainShape chain;
    chain.CreateLoop(vs, 4);*/
    b2EdgeShape e1;
    e1.Set(vs[0], vs[1]);
    b2EdgeShape e2;
    e2.Set(vs[1], vs[2]);
    b2EdgeShape e3;
    e3.Set(vs[2], vs[3]);
    b2EdgeShape e4;
    e4.Set(vs[3], vs[0]);

    b2BodyDef bodyDef2;
    bodyDef2.type = b2_staticBody;
    m_body = world.CreateBody(&bodyDef2);
    b2Fixture* fixtur2 = m_body->CreateFixture(&e1, 1.0f);

    b2BodyDef bodyDef3;
    bodyDef3.type = b2_staticBody;
    m_body = world.CreateBody(&bodyDef3);
    b2Fixture* fixtur3 = m_body->CreateFixture(&e2, 1.0f);

    b2BodyDef bodyDef4;
    bodyDef4.type = b2_staticBody;
    m_body = world.CreateBody(&bodyDef4);
    b2Fixture* fixtur4 = m_body->CreateFixture(&e3, 1.0f);

    b2BodyDef bodyDef5;
    bodyDef5.type = b2_staticBody;
    m_body = world.CreateBody(&bodyDef5);
    b2Fixture* fixtur5 = m_body->CreateFixture(&e4, 1.0f);
    /*b2BodyDef bodyDef2;
    bodyDef2.type = b2_staticBody;
    m_body = world.CreateBody(&bodyDef2);
    b2PolygonShape polygonShape2;
    polygonShape2.SetAsBox((tTrasa.nRozmiary[0]*tTrasa.nRozmiaryFragmentu[0])/10, 0.1);
    b2Fixture* fixture = m_body->CreateFixture(&polygonShape2, 1.0f);
    m_body->SetTransform(b2Vec2(0, 0), 0);

    b2BodyDef bodyDef3;
    bodyDef3.type = b2_staticBody;
    m_body1 = world.CreateBody(&bodyDef3);
    b2PolygonShape polygonShape3;
    polygonShape3.SetAsBox(0.1, (tTrasa.nRozmiary[1]*tTrasa.nRozmiaryFragmentu[1])/10);
    b2Fixture* fixture1 = m_body1->CreateFixture(&polygonShape3, 1.0f);
    m_body1->SetTransform(b2Vec2(0, 0), 0);

    b2BodyDef bodyDef4;
    bodyDef4.type = b2_staticBody;
    m_body2 = world.CreateBody(&bodyDef4);
    b2PolygonShape polygonShape4;
    polygonShape4.SetAsBox((tTrasa.nRozmiary[0]*tTrasa.nRozmiaryFragmentu[0])/10, 0.1);
    b2Fixture* fixture2 = m_body2->CreateFixture(&polygonShape4, 1.0f);
    m_body2->SetTransform(b2Vec2(0, (tTrasa.nRozmiary[1]*tTrasa.nRozmiaryFragmentu[1])/10), 0);

    b2BodyDef bodyDef5;
    bodyDef5.type = b2_staticBody;
    m_body3 = world.CreateBody(&bodyDef5);
    b2PolygonShape polygonShape5;
    polygonShape5.SetAsBox(0.1, (tTrasa.nRozmiary[1]*tTrasa.nRozmiaryFragmentu[1])/10);
    b2Fixture* fixture3 = m_body3->CreateFixture(&polygonShape5, 1.0f);
    m_body3->SetTransform(b2Vec2((tTrasa.nRozmiary[0]*tTrasa.nRozmiaryFragmentu[0])/10, 0), 0);*/
}

Game::~Game()
{
    if (car1)
        delete car1;
    for (int i = 0; i < nLiczbaPrzeciwnikow; ++i)
        if (SIcars[i])
            delete SIcars[i];
    if (SIcars)
        delete[] SIcars;
    if (fCzasyOkrazen)
        delete[] fCzasyOkrazen;
    if (m_body)
        m_body->GetWorld()->DestroyBody(m_body);
    if (m_body1)
        m_body1->GetWorld()->DestroyBody(m_body1);
    if (m_body2)
        m_body2->GetWorld()->DestroyBody(m_body2);
    if (m_body3)
        m_body3->GetWorld()->DestroyBody(m_body3);
}

int Game::Gra(b2World& world, const Input& wejscie, Event event, RenderWindow& window, float* u1, float* u2)
{
    FloatRect rec;
    rec.left = (car1->GetPosition().x > 320 ? car1->GetPosition().x-320 : 0);
    rec.top = (car1->GetPosition().x > 240 ? car1->GetPosition().x-240 : 0);
    rec.width = 640;
    rec.height = 480;
    view.SetFromRect(rec);
    str.SetPosition(view.GetRect().Left, view.GetRect().Top);
    str2.SetPosition(view.GetRect().Left + 320, view.GetRect().Top);
    str3.SetPosition(view.GetRect().Right - 70, view.GetRect().Top);
    str4.SetPosition(view.GetRect().Right - str4.GetRect().Right, view.GetRect().Top + 20);
    str5.SetPosition(view.GetRect().Right - 70, view.GetRect().Top + 40);
    if (!Odliczanie(wejscie, event, window))
        return -1;
    czas.Reset();
    while (window.IsOpened())
    {
        dt = double(window.GetFrameTime());
        while (window.GetEvent(event))
        {
            if (event.Type == Event::Closed)
            {
                window.Close();
                return -1;
            }
            if (wejscie.IsKeyDown(Key::Escape))
            {
                window.Close();
                return -1;
            }
            if (wejscie.IsKeyDown(Key::Right) && wejscie.IsKeyDown(Key::Left))continue;
            if (wejscie.IsKeyDown(Key::P))
            {
                int r = MenuPauza(wejscie, event, window);
                if (r == 0)
                    return 0;
                else if (r == -1)
                    return -1;

            }
        }
        if (wejscie.IsKeyDown(Key::Down))
            car1->SetKey(0);
        if (wejscie.IsKeyDown(Key::Left))
            car1->SetKey(2);
        if (wejscie.IsKeyDown(Key::Right))
            car1->SetKey(3);
        if (wejscie.IsKeyDown(Key::Up))
            car1->SetKey(1);

        car1->Update();
        for (int i = 0; i < nLiczbaPrzeciwnikow; ++i)
        {
            SIcars[i]->AktualizujPozycje(&tTrasa);
        }

        if (view.GetRect().Top + car1->GetShift().y >= 0 && view.GetRect().Bottom + car1->GetShift().y <= tTrasa.nRozmiary[1] * tTrasa.nRozmiaryFragmentu[1])
            if ((car1->GetShift().y > 0 && view.GetRect().Bottom - car1->GetSprPos().y <= 240) || (car1->GetShift().y < 0 && car1->GetSprPos().y - view.GetRect().Top <= 240))
            {
                view.Move(0, car1->GetShift().y);
                str.Move(0, car1->GetShift().y);
                str2.Move(0, car1->GetShift().y);
                str3.Move(0, car1->GetShift().y);
                str4.Move(0, car1->GetShift().y);
                str5.Move(0, car1->GetShift().y);
            }
        if (view.GetRect().Left + car1->GetShift().x >= 0 && view.GetRect().Right + car1->GetShift().x <= tTrasa.nRozmiary[0] * tTrasa.nRozmiaryFragmentu[0])
            if ((car1->GetShift().x > 0 && view.GetRect().Right - car1->GetSprPos().x <= 320) || (car1->GetShift().x < 0 && car1->GetSprPos().x - view.GetRect().Left <= 320))
            {
                view.Move(car1->GetShift().x, 0);
                str.Move(car1->GetShift().x, 0);
                str2.Move(car1->GetShift().x, 0);
                str3.Move(car1->GetShift().x, 0);
                str4.Move(car1->GetShift().x, 0);
                str5.Move(car1->GetShift().x, 0);
            }
        if (car1->GetPosition().x > 0 && car1->GetPosition().x < tTrasa.trasa.GetSize().x && car1->GetPosition().y > 0 && car1->GetPosition().y < tTrasa.trasa.GetSize().y)
            {
                pos[0] = (car1->GetSprPos().x - (static_cast<int>(car1->GetSprPos().x) % tTrasa.nRozmiaryFragmentu[0])) / tTrasa.nRozmiaryFragmentu[0];
                pos[1] = (car1->GetSprPos().y - (static_cast<int>(car1->GetSprPos().y) % tTrasa.nRozmiaryFragmentu[1])) / tTrasa.nRozmiaryFragmentu[1];
                if (pos[1] == 1)
                    indeks = tTrasa.nRozmiary[0] + pos[0];
                else if (pos[1] > 1)
                    indeks = (pos[1] * tTrasa.nRozmiary[0]) + pos[0];
                else
                    indeks = pos[0];
                if (tTrasa.aTablicaFragmentow[indeks].chInf == 'P')
                    tTrasa.aPunktyKontrolne[tTrasa.aTablicaFragmentow[indeks].nKtoryPunkt] = true;
                if (tTrasa.aTablicaFragmentow[indeks].chInf == 'M')
                {
                    int n = 0;
                    for (int i = 0; i < tTrasa.nLiczbaPunktowKontrolnych; ++i)
                        if (tTrasa.aPunktyKontrolne[i] == true)
                            ++n;
                    if (n == tTrasa.nLiczbaPunktowKontrolnych)
                    {
                        fCzasyOkrazen[nOkrazenie] = czas.GetElapsedTime();
                        cout << fCzasyOkrazen[nOkrazenie] << endl;
                        if (nOkrazenie > 0)
                        {
                            if (fCzasyOkrazen[nOkrazenie] < fCzasyOkrazen[nOkrazenie-1])
                                NajlepszeOkrazenie = fCzasyOkrazen[nOkrazenie];

                        }
                        else
                            NajlepszeOkrazenie = fCzasyOkrazen[nOkrazenie];
                        LacznyCzas += fCzasyOkrazen[nOkrazenie];
                        ++nOkrazenie;
                        czas.Reset();
                        cout << NajlepszeOkrazenie << ' ' << LacznyCzas << endl;
                        if (nOkrazenie >= nOkrazenia)
                        {
                            *u1 = LacznyCzas;
                            *u2 = NajlepszeOkrazenie;
                            return 1;
                        }
                        else
                            for (int i = 0; i < tTrasa.nLiczbaPunktowKontrolnych; ++i)
                                tTrasa.aPunktyKontrolne[i] = false;
                    }
                }
            }
        sprintf(ch, "%f", czas.GetElapsedTime());
        str.SetText(ch);
        sprintf(ch, "%f", static_cast<int>(car1->GetSpeed()/2));
        str2.SetText(ch);
        sprintf(ch, "%i", nOkrazenie+1);
        str3.SetText(ch);
        sprintf(ch, "%f", NajlepszeOkrazenie);
        str5.SetText(ch);
        window.Draw(tTrasa.trasa);
        window.Draw(car1->GetCar());
        for (int i = 0; i < nLiczbaPrzeciwnikow; ++i)
            window.Draw(SIcars[i]->GetCar());
        window.Draw(str);
        window.Draw(str2);
        window.Draw(str3);
        window.Draw(str4);
        window.Draw(str5);
        window.Display();
        window.Clear();
        world.Step(static_cast<float>(dt), 6, 2);
    }
}


bool Game::Odliczanie(const Input& wejscie, Event event, RenderWindow& window)
{
    if (window.IsOpened())
    {
        Clock czas2;
        String str3;
        str3.SetColor(Color::White);
        int nOdliczanie = 4;
        while (nOdliczanie >= -1)
        {
            if (czas2.GetElapsedTime() >= 1)
            {
                --nOdliczanie;
                czas2.Reset();
            }
            str3.SetPosition(view.GetRect().Left + 320, view.GetRect().Top + 240);
            if (nOdliczanie >= 0)
            {
                 sprintf(ch, "%i", nOdliczanie);
                 str3.SetText(ch);
            }
            else
                str3.SetText("Go!");
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
            }
            car1->Update();
            window.Draw(tTrasa.trasa);
            window.Draw(car1->GetCar());
            for (int i = 0; i < nLiczbaPrzeciwnikow; ++i)
                window.Draw(SIcars[i]->GetCar());
            window.Draw(str3);
            window.Display();
            window.Clear();
        }
    }
    return true;
}

int Game::MenuPauza(const Input& wejscie, Event event, RenderWindow& window)
{
    int p[2] = {view.GetRect().Left, view.GetRect().Top};
    view.Move(-p[0], -p[1]);
    String str1;
    String str21;
    String str31;
    str1.SetText("Kontynuuj");
    str21.SetText("Menu");
    str31.SetText("Wyjscie");
    str1.SetCenter(str1.GetRect().Right/2, str1.GetRect().Bottom/2);
    str21.SetCenter(str21.GetRect().Right/2, str21.GetRect().Bottom/2);
    str31.SetCenter(str31.GetRect().Right/2, str31.GetRect().Bottom/2);
    str1.SetPosition(window.GetWidth()/2, window.GetHeight()*0.25);
    str21.SetPosition(window.GetWidth()/2, window.GetHeight()*0.50);
    str31.SetPosition(window.GetWidth()/2, window.GetHeight()*0.75);
    str1.SetColor(Color::Black);
    str21.SetColor(Color::Black);
    str31.SetColor(Color::Black);
    float p1[4] = {str1.GetRect().Left, str1.GetRect().Top,  str1.GetRect().Right, str1.GetRect().Bottom};
    float p2[4] = {str21.GetRect().Left, str21.GetRect().Top,  str21.GetRect().Right, str21.GetRect().Bottom};
    float p3[4] = {str31.GetRect().Left, str31.GetRect().Top,  str31.GetRect().Right, str31.GetRect().Bottom};
    while (window.IsOpened())
    {
        while (window.GetEvent(event))
        {
             if (event.Type == Event::Closed)
            {
                window.Close();
                return -1;
            }
            if (wejscie.IsKeyDown(Key::Escape))
            {
                window.Close();
                return -1;
            }
            if (wejscie.IsMouseButtonDown(Mouse::Left))
            {
                if (wejscie.GetMouseX() >= p1[0] && wejscie.GetMouseX() <= p1[2] && wejscie.GetMouseY() >= p1[1] && wejscie.GetMouseY() <= p1[3])
                {
                    window.Clear();
                    view.Move(p[0], p[1]);
                    return 1;
                }
                if (wejscie.GetMouseX() >= p2[0] && wejscie.GetMouseX() <= p2[2] && wejscie.GetMouseY() >= p2[1] && wejscie.GetMouseY() <= p2[3])
                {
                    return 0;
                }
                if (wejscie.GetMouseX() >= p3[0] && wejscie.GetMouseX() <= p3[2] && wejscie.GetMouseY() >= p3[1] && wejscie.GetMouseY() <= p3[3])
                {
                    window.Close();
                    return -1;
                }
            }
        }
            if (wejscie.GetMouseX() >= p1[0] && wejscie.GetMouseX() <= p1[2] && wejscie.GetMouseY() >= p1[1] && wejscie.GetMouseY() <= p1[3])
                str.SetColor(Color::Blue);
            else
                str.SetColor(Color::Black);
            if (wejscie.GetMouseX() >= p2[0] && wejscie.GetMouseX() <= p2[2] && wejscie.GetMouseY() >= p2[1] && wejscie.GetMouseY() <= p2[3])
                str2.SetColor(Color::Blue);
            else
                str2.SetColor(Color::Black);
            if (wejscie.GetMouseX() >= p3[0] && wejscie.GetMouseX() <= p3[2] && wejscie.GetMouseY() >= p3[1] && wejscie.GetMouseY() <= p3[3])
                str3.SetColor(Color::Blue);
            else
                str3.SetColor(Color::Black);
            window.Draw(str1);
            window.Draw(str21);
            window.Draw(str31);
            window.Display();
            window.Clear(Color::White);
        }
    return 0;
}

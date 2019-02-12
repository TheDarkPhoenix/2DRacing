/*
* Author: Chris Campbell - www.iforce2d.net
*
* Copyright (c) 2006-2011 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef IFORCE2D_TOPDOWN_CAR_H
#define IFORCE2D_TOPDOWN_CAR_H

#include <vector>
#include <set>
#include <Box2D/Box2D.h>
#include "point.h"
#include "types.h"

#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif

/*enum
{
    TDC_LEFT     = 0x1,
    TDC_RIGHT    = 0x2,
    TDC_UP       = 0x4,
    TDC_DOWN     = 0x8
};*/

//types of fixture user data
enum fixtureUserDataType
{
    FUD_CAR_TIRE,
    FUD_GROUND_AREA
};

//a class to allow subclassing of different fixture user data
class FixtureUserData
{
    private:
        fixtureUserDataType m_type;

    protected:
        FixtureUserData(fixtureUserDataType type) : m_type(type) {}

    public:
        virtual fixtureUserDataType getType() { return m_type; }
        virtual ~FixtureUserData() {}
};

//class to allow marking a fixture as a car tire
class CarTireFUD : public FixtureUserData
{
    public:
        CarTireFUD() : FixtureUserData(FUD_CAR_TIRE) {}
};

//class to allow marking a fixture as a ground area
class GroundAreaFUD : public FixtureUserData
{
    public:
        float frictionModifier;
        bool outOfCourse;

        GroundAreaFUD(float fm, bool ooc) : FixtureUserData(FUD_GROUND_AREA)
        {
            frictionModifier = fm;
            outOfCourse = ooc;
        }
};

class TDTire
{
    public:
        b2Body* m_body;
        float m_maxForwardSpeed;
        float m_maxBackwardSpeed;
        float m_maxDriveForce;
        float m_maxLateralImpulse;
        std::set<GroundAreaFUD*> m_groundAreas;
        float m_currentTraction;

        TDTire(b2World* world);
        ~TDTire() { m_body->GetWorld()->DestroyBody(m_body); }

        void setCharacteristics(float maxForwardSpeed, float maxBackwardSpeed, float maxDriveForce, float maxLateralImpulse);
        void addGroundArea(GroundAreaFUD* ga) { m_groundAreas.insert(ga); updateTraction(); }
        void removeGroundArea(GroundAreaFUD* ga) { m_groundAreas.erase(ga); updateTraction(); }
        void updateTraction();
        b2Vec2 getLateralVelocity();
        b2Vec2 getForwardVelocity();
        void updateFriction();
        void updateDrive(bool b[4]);
        void updateTurn(bool b[4]);
};

class TDCar
{
    private:
        std::vector<TDTire*> m_tires;
        b2RevoluteJoint *flJoint, *frJoint;

    public:
        b2Body* m_body;

        TDCar(b2World* world, pt::Point pos);
        ~TDCar();

        void update(bool b[4], int steerlevel = 16);
        kolizja collision();
};

#endif

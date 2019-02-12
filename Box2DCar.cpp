#include "Box2DCar.h"
#include <iostream>

using namespace std;

///TDTire

TDTire::TDTire(b2World* world)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    m_body = world->CreateBody(&bodyDef);

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox( 0.3125f, 0.78125f );
    b2Fixture* fixture = m_body->CreateFixture(&polygonShape, 1);//shape, density
    fixture->SetUserData( new CarTireFUD() );

    m_body->SetUserData( this );

    m_currentTraction = 2;
}

void TDTire::setCharacteristics(float maxForwardSpeed, float maxBackwardSpeed, float maxDriveForce, float maxLateralImpulse)
{
    m_maxForwardSpeed = maxForwardSpeed;
    m_maxBackwardSpeed = maxBackwardSpeed;
    m_maxDriveForce = maxDriveForce;
    m_maxLateralImpulse = maxLateralImpulse;
}

void TDTire::updateTraction()
{
    if ( m_groundAreas.empty() )
        m_currentTraction = 1;
    else
    {
        //find area with highest traction
        m_currentTraction = 0;
        std::set<GroundAreaFUD*>::iterator it = m_groundAreas.begin();
        while (it != m_groundAreas.end())
        {
            GroundAreaFUD* ga = *it;
            if ( ga->frictionModifier > m_currentTraction )
                m_currentTraction = ga->frictionModifier;
            ++it;
        }
    }
}

b2Vec2 TDTire::getLateralVelocity()
{
    b2Vec2 currentRightNormal = m_body->GetWorldVector( b2Vec2(1,0) );
    return b2Dot( currentRightNormal, m_body->GetLinearVelocity() ) * currentRightNormal;
}

b2Vec2 TDTire::getForwardVelocity()
{
    b2Vec2 currentForwardNormal = m_body->GetWorldVector( b2Vec2(0,1) );
    return b2Dot( currentForwardNormal, m_body->GetLinearVelocity() ) * currentForwardNormal;
}

void TDTire::updateFriction()
{
    //lateral linear velocity
    b2Vec2 impulse = m_body->GetMass() * -getLateralVelocity();
    if ( impulse.Length() > m_maxLateralImpulse )
        impulse *= m_maxLateralImpulse / impulse.Length();
    m_body->ApplyLinearImpulse( m_currentTraction * impulse, m_body->GetWorldCenter() );

    //angular velocity
    m_body->ApplyAngularImpulse( m_currentTraction * 0.1f * m_body->GetInertia() * -m_body->GetAngularVelocity() );

    //forward linear velocity
    b2Vec2 currentForwardNormal = getForwardVelocity();
    float currentForwardSpeed = currentForwardNormal.Normalize();
    float dragForceMagnitude = -2 * currentForwardSpeed;
    m_body->ApplyForce( m_currentTraction * dragForceMagnitude * currentForwardNormal, m_body->GetWorldCenter() );
}

void TDTire::updateDrive(bool b[4])
{
    //find desired speed
    float desiredSpeed = 0;

    if (b[0])
        desiredSpeed = m_maxForwardSpeed;
    else if(b[1])
        desiredSpeed = m_maxBackwardSpeed;
    else
        return;

    //find current speed in forward direction
    b2Vec2 currentForwardNormal = m_body->GetWorldVector( b2Vec2(0,1) );
    float currentSpeed = b2Dot( getForwardVelocity(), currentForwardNormal );

    //apply necessary force
    float force = 0;
    if ( desiredSpeed > currentSpeed )
        force = m_maxDriveForce;
    else if ( desiredSpeed < currentSpeed )
        force = -m_maxDriveForce;
    else
        return;
    m_body->ApplyForce( m_currentTraction * force * currentForwardNormal, m_body->GetWorldCenter() );
}

void TDTire::updateTurn(bool b[4])
{
    float desiredTorque = 0;
    if (b[2])
        desiredTorque = 15;
    if (b[3])
        desiredTorque = -15;
    m_body->ApplyTorque( desiredTorque );
}

///TDCar

TDCar::TDCar(b2World* world, pt::Point pos)
{
    //create car body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    m_body = world->CreateBody(&bodyDef);
    m_body->SetAngularDamping(3);

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(0.875, 1.8125);
    b2Fixture* fixture = m_body->CreateFixture(&polygonShape, 0.4f);//shape, density
    b2AABB ab;
    ab.lowerBound.Set(fixture->GetAABB(0).GetCenter().x - 0.5, fixture->GetAABB(0).GetCenter().y - 0.5);
    ab.upperBound.Set(fixture->GetAABB(0).GetCenter().x + 0.5, fixture->GetAABB(0).GetCenter().y + 0.5);
    //fixture->GetAABB()->
    m_body->SetTransform(b2Vec2(pos.x, pos.y), 0);

    //prepare common joint parameters
    b2RevoluteJointDef jointDef;
    jointDef.bodyA = m_body;
    jointDef.enableLimit = true;
    jointDef.lowerAngle = 0;
    jointDef.upperAngle = 0;
    jointDef.localAnchorB.SetZero();//center of tire

    float maxForwardSpeed = 25;
    float maxBackwardSpeed = -40;
    float backTireMaxDriveForce = 300;
    float frontTireMaxDriveForce = 500;
    float backTireMaxLateralImpulse = 8.5;
    float frontTireMaxLateralImpulse = 7.5;

    //back left tire
    TDTire* tire = new TDTire(world);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
    jointDef.bodyB = tire->m_body;
    jointDef.localAnchorA.Set(-0.375, 0.3125);
    tire->m_body->SetTransform(b2Vec2(m_body->GetPosition().x-0.375, m_body->GetPosition().y+0.59375), 0);
    world->CreateJoint( &jointDef );
    m_tires.push_back(tire);

    //back right tire
    tire = new TDTire(world);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
    jointDef.bodyB = tire->m_body;
    jointDef.localAnchorA.Set(0.375, 0.3125);
    tire->m_body->SetTransform(b2Vec2(m_body->GetPosition().x+0.375, m_body->GetPosition().y+0.59375), 0);
    world->CreateJoint( &jointDef );
    m_tires.push_back(tire);

    //front left tire
    tire = new TDTire(world);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
    jointDef.bodyB = tire->m_body;
    jointDef.localAnchorA.Set(-0.375, 1.5);
    tire->m_body->SetTransform(b2Vec2(m_body->GetPosition().x-0.375, m_body->GetPosition().y-0.59375), 0);
    flJoint = (b2RevoluteJoint*)world->CreateJoint( &jointDef );
    m_tires.push_back(tire);

    //front right tire
    tire = new TDTire(world);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
    jointDef.bodyB = tire->m_body;
    jointDef.localAnchorA.Set(0.375, 1.5);
    tire->m_body->SetTransform(b2Vec2(m_body->GetPosition().x+0.375, m_body->GetPosition().y-0.59375), 0);
    frJoint = (b2RevoluteJoint*)world->CreateJoint( &jointDef );
    m_tires.push_back(tire);
}

TDCar::~TDCar()
{
    for (int i = 0; i < m_tires.size(); i++)
        delete m_tires[i];
}

void TDCar::update(bool b[4], int steerlevel)
{
    for (int i = 0; i < m_tires.size(); i++)
        m_tires[i]->updateFriction();
    for (int i = 0; i < m_tires.size(); i++)
        m_tires[i]->updateDrive(b);

    //control steering
    float lockAngle = steerlevel * DEGTORAD;
    float turnSpeedPerSec = 120 * DEGTORAD;//from lock to lock in 0.5 sec
    float turnPerTimeStep = turnSpeedPerSec / 60.0f;
    float desiredAngle = 0;
    if(b[2])
        desiredAngle = lockAngle;
    if(b[3])
        desiredAngle = -lockAngle;
    float angleNow = flJoint->GetJointAngle();
    float angleToTurn = desiredAngle - angleNow;
    angleToTurn = b2Clamp( angleToTurn, -turnPerTimeStep, turnPerTimeStep );
    float newAngle = angleNow + angleToTurn;
    flJoint->SetLimits( newAngle, newAngle );
    frJoint->SetLimits( newAngle, newAngle );
}

kolizja TDCar::collision()
{
    kolizja col = {0,0,0,0,0,0,0,0,0};
    b2WorldManifold worldManifold;
    for( b2ContactEdge * ce = m_body->GetContactList(); ce; ce = ce->next )
    {
        if( ce->contact != NULL )
        {
            b2Contact * c = ce->contact;
            if( c != NULL )
            {
                c->GetWorldManifold(&worldManifold);
                //cout << worldManifold.points[0].x << ' ' << worldManifold.points[0].y << worldManifold.points[1].x << ' ' << worldManifold.points[1].y << endl;
                if (c->GetFixtureA()->GetBody()->GetPosition().x > m_body->GetPosition().x)
                {
                    col.Rb = true;
                    if (c->GetFixtureA()->GetType() == b2_dynamicBody)
                        col.Rt = 2;
                    else
                        col.Rt = 1;
                }
                if (c->GetFixtureA()->GetBody()->GetPosition().x < m_body->GetPosition().x)
                {
                    col.Lb = true;
                    if (c->GetFixtureA()->GetType() == b2_dynamicBody)
                        col.Lt = 2;
                    else
                        col.Lt = 1;
                }
                if (c->GetFixtureA()->GetBody()->GetPosition().y > m_body->GetPosition().x)
                {
                    col.Bb = true;
                    if (c->GetFixtureA()->GetType() == b2_dynamicBody)
                        col.Bt = 2;
                    else
                        col.Bt = 1;
                }
                if (c->GetFixtureA()->GetBody()->GetPosition().y < m_body->GetPosition().x)
                {
                    col.Fb = true;
                    if (c->GetFixtureA()->GetType() == b2_dynamicBody)
                        col.Ft = 2;
                    else
                        col.Ft = 1;
                }
            }
        }
    }
    if (col.Rb || col.Lb || col.Bb || col.Fb)
        col.C = true;
    return col;
}

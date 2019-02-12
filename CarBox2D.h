#include <Box2D/Box2D.h>

#define PI 3.14159265359

class CarBox2D
{
    private:

        static const float MAX_STEER_ANGLE = PI/3;
        static const float STEER_SPEED = 1.5;
        static const int SIDEWAYS_FRICTION_FORCE = 10;
        static const int HORSEPOWERS = 40;
        const b2Vec2 CAR_STARTING_POS;

        const b2Vec2 leftRearWheelPosition;
        const b2Vec2 rightRearWheelPosition;
        const b2Vec2 leftFrontWheelPosition;
        const b2Vec2 rightFrontWheelPosition;

        int engineSpeed;
        int steeringAngle;

        b2AABB* worldBox;

        b2World* myWorld;

        b2BodyDef* staticDef;
        b2Polygon* staticBox;

        b2BodyDef* bodyDef;

        b2Body* body;

        b2BodyDef* leftWheelDef;
        b2Body* leftWheel;

        b2BodyDef* rightWheelDef;
        b2Body* rightWheel;

        b2BodyDef* leftRearWheelDef;
        b2Body* leftRearWheel;

        b2BodyDef* rightRearWheelDef;
        b2Body* rightRearWheel;

        // define our shapes
        b2PolygonDef* boxDef;

        //Left Wheel shape
        b2PolygonDef* leftWheelShapeDef;

        //Right Wheel shape
        b2PolygonDef* rightWheelShapeDef;

        //Left Wheel shape
        b2PolygonDef* leftRearWheelShapeDef;

        //Right Wheel shape
        b2PolygonDef* rightRearWheelShapeDef;


        b2RevoluteJointDef* leftJointDef;


        b2RevoluteJointDef* rightJointDef;


        b2RevoluteJointDef* leftJoint;
        b2RevoluteJointDef* rightJoint;

        b2PrismaticJointDef* leftRearJointDef;


        b2PrismaticJointDef* rightRearJointDef;

    public:

        CarBox2D();
        ~CarBox2D(){};

        void killOrthogonalVelocity(b2Body& targetBody);
        void keyPressed_handler(int key);
        void keyReleased_handler(int key);
        void Update();

}

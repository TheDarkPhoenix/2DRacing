#include "CarBox2D.h"

CarBox2D::CarBox2D() : CAR_STARTING_POS(10,10), leftRearWheelPosition(-1.5,1.90), rightRearWheelPosition(1.5,1.9), leftFrontWheelPosition(-1.5,-1.9), rightFrontWheelPosition(1.5,-1.9)
{
    engineSpeed = 0;
    steeringAngle = 0;

    worldBox = new b2AABB();
    worldBox->lowerBound.Set(-100,-100);
    worldBox->upperBound.Set(100,100);

    myWorld = new b2World(b2Vec2(0,0));

    staticDef = new b2BodyDef();
    staticDef->position.Set(5,20);

    staticBox = new b2PolygonDef();
    staticBox->SetAsBox(5,5);

    myWorld->CreateBody(staticDef).CreateShape(staticBox);
    staticDef->position.x = 25;

    myWorld->CreateBody(staticDef).CreateShape(staticBox);
    staticDef->position.Set(15, 24);

    myWorld->CreateBody(staticDef).CreateShape(staticBox);

    bodyDef = new b2BodyDef();
    bodyDef.linearDamping = 1;
    bodyDef.angularDamping = 1;
    bodyDef.position = CAR_STARTING_POS.Copy()

    body = myWorld.CreateBody(bodyDef);
    body.SetMassFromShapes();

    leftWheelDef = new b2BodyDef();
    leftWheelDef.position = CAR_STARTING_POS.Copy();
    leftWheelDef.position.Add(leftFrontWheelPosition);
    leftWheel = myWorld.CreateBody(leftWheelDef);

    rightWheelDef = new b2BodyDef();
    rightWheelDef.position = CAR_STARTING_POS.Copy();
    rightWheelDef.position.Add(rightFrontWheelPosition);
    rightWheel = myWorld.CreateBody(rightWheelDef);

    leftRearWheelDef = new b2BodyDef();
    leftRearWheelDef.position = CAR_STARTING_POS.Copy();
    leftRearWheelDef.position.Add(leftRearWheelPosition);
    leftRearWheel = myWorld.CreateBody(leftRearWheelDef);

    rightRearWheelDef = new b2BodyDef();
    rightRearWheelDef.position = CAR_STARTING_POS.Copy();
    rightRearWheelDef.position.Add(rightRearWheelPosition);
    rightRearWheel = myWorld.CreateBody(rightRearWheelDef);

    // define our shapes
    boxDef = new b2PolygonDef();
    boxDef.SetAsBox(1.5,2.5);
    boxDef.density = 1;
    body.CreateShape(boxDef);

    //Left Wheel shape
    leftWheelShapeDef = new b2PolygonDef();
    leftWheelShapeDef.SetAsBox(0.2,0.5);
    leftWheelShapeDef.density = 1;
    leftWheel.CreateShape(leftWheelShapeDef);

    //Right Wheel shape
    rightWheelShapeDef = new b2PolygonDef();
    rightWheelShapeDef.SetAsBox(0.2,0.5);
    rightWheelShapeDef.density = 1;
    rightWheel.CreateShape(rightWheelShapeDef);

    //Left Wheel shape
    leftRearWheelShapeDef = new b2PolygonDef();
    leftRearWheelShapeDef.SetAsBox(0.2,0.5);
    leftRearWheelShapeDef.density = 1;
    leftRearWheel.CreateShape(leftRearWheelShapeDef);

    //Right Wheel shape
    rightRearWheelShapeDef = new b2PolygonDef();
    rightRearWheelShapeDef.SetAsBox(0.2,0.5);
    rightRearWheelShapeDef.density = 1;
    rightRearWheel.CreateShape(rightRearWheelShapeDef);

    body.SetMassFromShapes();
    leftWheel.SetMassFromShapes();
    rightWheel.SetMassFromShapes();
    leftRearWheel.SetMassFromShapes();
    rightRearWheel.SetMassFromShapes();

    leftJointDef = new b2RevoluteJointDef();
    leftJointDef.Initialize(body, leftWheel, leftWheel.GetWorldCenter());
    leftJointDef.enableMotor = true;
    leftJointDef.maxMotorTorque = 100;

    rightJointDef = new b2RevoluteJointDef();
    rightJointDef.Initialize(body, rightWheel, rightWheel.GetWorldCenter());
    rightJointDef.enableMotor = true;
    rightJointDef.maxMotorTorque = 100;

    leftJoint = b2RevoluteJoint(myWorld.CreateJoint(leftJointDef));
    rightJoint = b2RevoluteJoint(myWorld.CreateJoint(rightJointDef));

    leftRearJointDef = new b2PrismaticJointDef();
    leftRearJointDef.Initialize(body, leftRearWheel, leftRearWheel.GetWorldCenter(), new b2Vec2(1,0));
    leftRearJointDef.enableLimit = true;
    leftRearJointDef.lowerTranslation = leftRearJointDef.upperTranslation = 0;

    rightRearJointDef = new b2PrismaticJointDef();
    rightRearJointDef.Initialize(body, rightRearWheel, rightRearWheel.GetWorldCenter(), new b2Vec2(1,0));
    rightRearJointDef.enableLimit = true;
    rightRearJointDef.lowerTranslation = rightRearJointDef.upperTranslation = 0;

    myWorld.CreateJoint(leftRearJointDef);
    myWorld.CreateJoint(rightRearJointDef);
}

void CarBox2D::killOrthogonalVelocity(b2Body& targetBody)
{
	b2Vec2* localPoint = new b2Vec2(0,0);
	b2Vec2* velocity = targetBody.GetLinearVelocityFromLocalPoint(localPoint);

	b2Vec2* sidewaysAxis = targetBody.GetXForm().R.col2.Copy();
	sidewaysAxis.Multiply(b2Math.b2Dot(velocity,sidewaysAxis))

	targetBody.SetLinearVelocity(sidewaysAxis);//targetBody.GetWorldPoint(localPoint));
}

void CarBox2D::keyPressed_handler(int key)
{
	if(key == 0)
	{
		body.WakeUp();
		engineSpeed = -HORSEPOWERS;
	}
	if(key == 1)
	{
		engineSpeed = HORSEPOWERS;
	}
	if(key == 2)
	{
		steeringAngle = MAX_STEER_ANGLE
	}
	if(ekey == 3)
	{
		steeringAngle = -MAX_STEER_ANGLE
	}
}

void CarBox2D::keyReleased_handler(int key)
{
	if(key == 0 || key == 1)
	{
		engineSpeed = 0;
	}
	if(key == 2 || key == 3)
	{
		steeringAngle = 0;
	}
}

void CarBox2D::Update()
{
	myWorld.Step(1/30, 8);
	killOrthogonalVelocity(leftWheel);
	killOrthogonalVelocity(rightWheel);
	killOrthogonalVelocity(leftRearWheel);
	killOrthogonalVelocity(rightRearWheel);

	//Driving
	var ldirection = leftWheel.GetXForm().R.col2.Copy();
	ldirection.Multiply(engineSpeed);
	var rdirection = rightWheel.GetXForm().R.col2.Copy()
	rdirection.Multiply(engineSpeed);
	leftWheel.ApplyForce(ldirection, leftWheel.GetPosition());
	rightWheel.ApplyForce(rdirection, rightWheel.GetPosition());

	//Steering
	float mspeed;
	mspeed = steeringAngle - leftJoint.GetJointAngle();
	leftJoint.SetMotorSpeed(mspeed * STEER_SPEED);
	mspeed = steeringAngle - rightJoint.GetJointAngle();
	rightJoint.SetMotorSpeed(mspeed * STEER_SPEED);
}

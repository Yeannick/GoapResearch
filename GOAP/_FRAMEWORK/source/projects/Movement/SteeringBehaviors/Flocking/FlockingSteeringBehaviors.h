#pragma once
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
class Flock;

//COHESION - FLOCKING
//*******************
class Cohesion : public Seek
{
public:
	Cohesion(Flock* pFlock) :m_pFlock(pFlock) {};

	//Cohesion Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

private:
	Flock* m_pFlock = nullptr;
};


//SEPARATION - FLOCKING
//*********************
class Separation : public Seek
{
public:
	Separation(Flock* pFlock) : m_pFlock(pFlock) {};

	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
private:
	Flock* m_pFlock;
};

//VELOCITY MATCH - FLOCKING
//************************
class Alignment : public Seek
{
public:
	Alignment(Flock* pFlock) : m_pFlock(pFlock) {};
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

private:
	Flock* m_pFlock;
};
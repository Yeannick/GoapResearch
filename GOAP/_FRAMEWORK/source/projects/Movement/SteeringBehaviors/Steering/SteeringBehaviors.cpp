//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "SteeringBehaviors.h"
#include "../SteeringAgent.h"
#include "../Obstacle.h"
#include "framework\EliteMath\EMatrix2x3.h"
#include "framework/EliteMath/EMath.h"

//SEEK
//****
SteeringOutput Seek::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	steering.LinearVelocity = (m_Target.Position - pAgent->GetPosition());
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, {0,1,0,1});
	}
	return steering;
}

//FLEE
//****
SteeringOutput Flee::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	steering.LinearVelocity = (m_Target.Position - pAgent->GetPosition());
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= -pAgent->GetMaxLinearSpeed();

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, { 0,1,0,1 });
	}


	return steering;
}

SteeringOutput Arrive::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{

	SteeringOutput steering = {};

	//const float arrivalRadius = 1.f;
	const float slowRadius = 10.f;

	Elite::Vector2 toTarget = m_Target.Position - pAgent->GetPosition();
	const float distance = toTarget.Magnitude();

	steering.LinearVelocity = (m_Target.Position - pAgent->GetPosition());
	steering.LinearVelocity.Normalize();
	if (distance < slowRadius)
	{
		steering.LinearVelocity = steering.LinearVelocity * distance / slowRadius;
	}
	else
	{
		steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();
	}


	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, { 0,1,0,1 });
		DEBUGRENDERER2D->DrawCircle(pAgent->GetPosition(), slowRadius, { 0,0,1,1 },0.f);
	}
	return steering;
}

SteeringOutput Face::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{

	SteeringOutput steering = {};

	pAgent->SetRotation(atan2(m_Target.Position.y, m_Target.Position.x) + M_PI / 2);

	pAgent->SetAutoOrient(false);
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, { 0,1,0,1 });
	}
	return steering;
}

SteeringOutput Pursuit::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};



	Elite::Vector2 Futureloc;

	Futureloc = m_Target.Position + m_Target.LinearVelocity ;



	steering.LinearVelocity = Futureloc - pAgent->GetPosition();
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();


	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 }, 0.4f);
	}
	return steering;
}

SteeringOutput Wander::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput wandering{};
	SetWanderRadius(4.f);
	SetWanderOffset(6.f);
	SetMaxAngleChange(45.f);

	m_WanderAngle += Elite::ToRadians(Elite::randomFloat(-m_AngleChange, m_AngleChange));

	m_Target.Position.x = (pAgent->GetPosition() + pAgent->GetDirection() * m_Offset).x + (m_Radius * cos(m_WanderAngle));
	m_Target.Position.y = (pAgent->GetPosition() + pAgent->GetDirection() * m_Offset).y + (m_Radius * sin(m_WanderAngle));

	wandering.LinearVelocity = (m_Target).Position - pAgent->GetPosition();
	wandering.LinearVelocity.Normalize();
	wandering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawCircle(pAgent->GetPosition() + pAgent->GetDirection() * m_Offset, 4.f, { 0,0,1 }, 0.f);
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), wandering.LinearVelocity, 6, { 0,1,0 }, 0.4f);

	}
	return Seek::CalculateSteering(deltaT, pAgent);
}

SteeringOutput Evade::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};


	Elite::Vector2 Futureloc;
	float distanceToTarget = Elite::Distance(pAgent->GetPosition(), m_Target.Position);
	if (distanceToTarget > m_EvadeRadius)
	{
		return SteeringOutput(Elite::ZeroVector2, 0.f, false);
	}
	Futureloc = m_Target.Position + m_Target.LinearVelocity ;

	steering.LinearVelocity = -(Futureloc) - pAgent->GetPosition();
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= -pAgent->GetMaxLinearSpeed();


	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 }, 0.4f);
	}
	return steering;
}

#include "stdafx.h"
#include "SandboxAgent.h"

using namespace Elite;

SandboxAgent::SandboxAgent(): BaseAgent()
{
	m_Target = GetPosition();
}

void SandboxAgent::Update(float dt)
{
	//Orientation
	const auto angle = Orientation(GetRotation(), -GetLinearVelocity());
	m_pRigidBody->SetTransform(Transform(GetPosition(), Elite::Vector2(angle, angle)));

	// variables 

	const float maxSpeed = 50.f;
	const float arrivalRadius = 1.f;
	const float slowRadius = 15.f;

	Vector2 toTarget = m_Target - GetPosition();
	const float distance = toTarget.Magnitude();

	// if agent is in arrival radius 
	if (distance < arrivalRadius)
	{
		// set linear velocity to 0 
		SetLinearVelocity(ZeroVector2);
		return;
	}
	Vector2 Velocity = toTarget;
	Velocity.Normalize();

	if (distance < slowRadius)
	{
		// the closer the agent gets to the target the slower he will go
		Velocity *= maxSpeed * distance / slowRadius;
	}
	else
	{
		Velocity *= maxSpeed;
	}
	

	SetLinearVelocity(Velocity);
}

void SandboxAgent::Render(float dt)
{
	BaseAgent::Render(dt); //Default Agent Rendering
}

float SandboxAgent::Orientation(float currentOrientation, Elite::Vector2 velocity) const
{
	//Determine angle based on direction
	if (velocity.Magnitude() > 0)
	{
		velocity.Normalize();
		return atan2(-velocity.x, velocity.y);
	}
	return currentOrientation;
}
#include "stdafx.h"
#include "FlockingSteeringBehaviors.h"
#include "TheFlock.h"
#include "../SteeringAgent.h"
#include "../SteeringHelpers.h"

//*******************
//COHESION (FLOCKING)
SteeringOutput Cohesion::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	
	SteeringOutput steering;

	if (m_pFlock->GetNrOfNeighbors() != 0)
	{
		
		pAgent->SetAutoOrient(true);
		

		Elite::Vector2 ToTarget = m_pFlock->GetAverageNeighborPos() - pAgent->GetPosition();
		steering.LinearVelocity = ToTarget;
		steering.LinearVelocity.Normalize();
		steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();
		if (pAgent->CanRenderBehavior())
		{
			DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, Elite::Color{ 0.f,0.f,1.f,1.f }, 0);
		}
		
	}
	
	return steering;
	
	
}


//*********************
//SEPARATION (FLOCKING)
SteeringOutput Separation::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering;
	pAgent->SetAutoOrient(true);
	if (m_pFlock->GetNrOfNeighbors() != 0 )
	{
		for (auto neighbour : m_pFlock->GetNeighbors())
		{
			steering.LinearVelocity += neighbour->GetPosition() - pAgent->GetPosition();
		}
		steering.LinearVelocity *= -1;
		steering.LinearVelocity.Normalize();
		steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

		if (pAgent->CanRenderBehavior())
		{
			DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, Elite::Color{ 0.f,0.f,1.f,1.f }, 0);
		}
		
	}
	
	return steering;
}

//*************************
//VELOCITY MATCH (FLOCKING)

SteeringOutput Alignment::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering;
	if (m_pFlock->GetNrOfNeighbors() != 0 )
	{
		steering.LinearVelocity = m_pFlock->GetAverageNeighborVelocity();
		steering.LinearVelocity.Normalize();
		steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

		if (pAgent->CanRenderBehavior())
		{
			DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, Elite::Color{ 0.f,0.f,1.f,1.f }, 0);
		}

		
	}
	
	return steering;
}

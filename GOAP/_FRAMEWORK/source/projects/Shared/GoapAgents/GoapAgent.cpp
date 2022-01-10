#include "stdafx.h"
#include "GoapAgent.h"

GoapAgent::GoapAgent(Elite::Vector2 pos):
	GoapAgent(pos, Elite::Color{ 0.8f,0.8f,0.8f })
{


	
}

GoapAgent::GoapAgent(Elite::Vector2 pos, Elite::Color color):
	SteeringAgent(2.0f)
{
	m_BodyColor = color;
	SetPosition(pos);
}

GoapAgent::~GoapAgent()
{
	SAFE_DELETE(m_DecisionMaking);
}

void GoapAgent::Update(float dt)
{
	//m_DecisionMaking->Update(dt);
	SteeringAgent::Update(dt);
}

void GoapAgent::Render(float dt)
{
	SteeringAgent::Render(dt);
}

void GoapAgent::SetDecisionMaking(Elite::IDecisionMaking* decisionmakingStructure)
{
	m_DecisionMaking = decisionmakingStructure;
}

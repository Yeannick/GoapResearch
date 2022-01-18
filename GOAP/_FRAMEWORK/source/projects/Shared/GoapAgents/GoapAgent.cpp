#include "stdafx.h"
#include "GoapAgent.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
//#include <framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPAction.h>
GoapAgent::GoapAgent(Elite::Vector2 pos):
	GoapAgent(pos, Elite::Color{ 0.8f,0.8f,0.8f })
{

	m_pSeek = new Seek();
	
}

GoapAgent::GoapAgent(Elite::Vector2 pos, Elite::Color color):
	SteeringAgent(2.0f)
{
	m_pSeek = new Seek();
	m_BodyColor = color;
	SetPosition(pos);
}

GoapAgent::~GoapAgent()
{
	SAFE_DELETE(m_DecisionMaking);
}

void GoapAgent::Update(float dt)
{
	m_DecisionMaking->Update(dt);
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

void GoapAgent::SetToSeek(Elite::Vector2 seekPos)
{
	this->SetMaxLinearSpeed(m_MovementSpeed*2);
	m_pSeek->SetTarget(seekPos);
	SetSteeringBehavior(m_pSeek);
}

void GoapAgent::SetActions(std::vector<GOAP::Action*> actions)
{
	m_Actions = actions;
}

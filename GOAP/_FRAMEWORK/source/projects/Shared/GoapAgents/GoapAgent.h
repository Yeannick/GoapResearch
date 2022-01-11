#pragma once
#include "projects/Movement/SteeringBehaviors/SteeringAgent.h"
class GoapAgent : public SteeringAgent
{
public :
	GoapAgent(Elite::Vector2 pos);
	GoapAgent(Elite::Vector2 pos, Elite::Color color);
	virtual ~GoapAgent();

	// Agent functions
	virtual void Update(float dt) override;
	virtual void Render(float dt) override;

	// GoapFunctions

	void SetDecisionMaking(Elite::IDecisionMaking* decisionmakingStructure);
	void SetToSeek(Elite::Vector2 seekPos);
	
private :
	Elite::IDecisionMaking* m_DecisionMaking = nullptr;
	float m_MovementSpeed = 25.f;
	ISteeringBehavior* m_pSeek = nullptr;
};


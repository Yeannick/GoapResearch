#pragma once
#include "projects/Movement/SteeringBehaviors/SteeringAgent.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPAction.h"

	class GoapAgent : public SteeringAgent
	{
	public:
		GoapAgent(Elite::Vector2 pos);
		GoapAgent(Elite::Vector2 pos, Elite::Color color);
		virtual ~GoapAgent();

		// Agent functions
		virtual void Update(float dt) override;
		virtual void Render(float dt) override;

		// GoapFunctions

		void SetDecisionMaking(Elite::IDecisionMaking* decisionmakingStructure);
		void SetToSeek(Elite::Vector2 seekPos);
		void SetActions(std::vector<GOAP::Action*> actions);
		Elite::Vector2 GetTarget() { return m_Target; }
		std::string GetStateName() { return m_StateName; }
		void SetStateName(std::string name) { m_StateName = name; }
	private:
		Elite::Vector2 m_Target;
		std::vector<GOAP::Action*> m_Actions;
		Elite::IDecisionMaking* m_DecisionMaking = nullptr;
		float m_MovementSpeed = 25.f;
		ISteeringBehavior* m_pSeek = nullptr;

		// debug variables
		std::string m_StateName;
	};


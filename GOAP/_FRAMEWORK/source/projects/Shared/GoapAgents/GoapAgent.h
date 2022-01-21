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

		

		// Setter for DecisionMaking of the FSM
		void SetDecisionMaking(Elite::IDecisionMaking* decisionmakingStructure);

		// Setters for SteeringBehaviours used in MOVETO state of FSM
		void SetToSeek(Elite::Vector2 seekPos);
		void SetToArrival(Elite::Vector2 seekPos);
		void SetToStill();

		// Setter for all of the possible Actions
		void SetActions(std::vector<GOAP::Action*> actions);
	
		// Debug Getter and setter used to display state in IMGUI UI
		std::string GetStateName() { return m_StateName; }
		void SetStateName(std::string name) { m_StateName = name; }

		// Actual Path of action Getter and Setter
		std::vector<GOAP::Action*> GetActionPath() { return m_ActionPath; }
		void SetActionPath(std::vector<GOAP::Action*> path) { m_ActionPath = path; }
	private:
		// All possible actions 
		std::vector<GOAP::Action*> m_Actions;

		// DecisionMaking 
		Elite::IDecisionMaking* m_DecisionMaking = nullptr;

		// MovementSpeed variable of the agent
		float m_MovementSpeed = 25.f;

		// SteeringBehaviors
		ISteeringBehavior* m_pSeek = nullptr;
		ISteeringBehavior* m_pArrival = nullptr;

		// Actual path of actions
		std::vector<GOAP::Action*> m_ActionPath;

		// debug variables
		std::string m_StateName;
	};


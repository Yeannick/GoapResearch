#pragma once
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "framework/EliteAI/EliteData/EBlackboard.h"
#include "projects/Shared/GoapAgents/GoapAgent.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPAction.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPPlanner.h"



class Idle : public Elite::FSMState
{
public:
	virtual void OnEnter(Elite::Blackboard* pBlackboard)override
	{
		// Calculate a path plan
		GoapAgent* pAgent = nullptr;
		bool GetAgent = pBlackboard->GetData("Agent", pAgent);

		GOAP::WorldState currentState;
		bool GetWorldState = pBlackboard->GetData("WorldState", currentState);

		GOAP::WorldState goalState;
		bool GetGoalState = pBlackboard->GetData("GoalState", goalState);

		std::vector<GOAP::Action*> actions;
		bool GetActions = pBlackboard->GetData("Actions", actions);

		GOAP::Planner Plan;
		std::vector<GOAP::Action*> PlannedActions;
		try {
			PlannedActions = Plan.Plan(currentState, goalState, actions);
			std::cout << "\n";
			std::cout << "Found a path!\n";


			std::vector<GOAP::Action*> reversePath = PlannedActions;
			std::reverse(reversePath.begin(), reversePath.end());

			for (auto a : reversePath)
			{
				std::cout << a->GetName() << "\n";
			}

		}
		catch (const std::exception&) {
			std::cout << "Sorry, could not find a path!\n";
		}
		if (PlannedActions.empty())return;
		// check if action can be performed
		std::reverse(PlannedActions.begin(), PlannedActions.end());
		auto precon = PlannedActions.front()->GetPreconditons();
	
		pBlackboard->ChangeData("Plan", PlannedActions);
		pBlackboard->AddData("Action", PlannedActions[0]);

		pAgent->SetStateName(name);
		// else transition to move to 
	}
	virtual std::string GetName()  override
	{ 
		return name;
	}
	int ID = 1;
	std::string name = "Idle";

	


};
class MoveTo : public Elite::FSMState
{
public:
	virtual void OnEnter(Elite::Blackboard* pBlackboard)override
	{
		GoapAgent* pAgent = nullptr;
		bool GetAgent = pBlackboard->GetData("Agent", pAgent);

		GoapAgent* pTarget;
		bool GetEnemies = pBlackboard->GetData("Target", pTarget);

		if (GetAgent && GetEnemies)
		{
			
			pAgent->SetToSeek(pTarget->GetPosition());
		}
		pAgent->SetStateName(name);
	}
	virtual std::string GetName()  override
	{
		return name;
	}
	std::string name = "Move To";
};
class PerformAction : public Elite::FSMState
{
public:
	virtual void OnEnter(Elite::Blackboard* pBlackboard)override
	{
		GoapAgent* pAgent = nullptr;
		bool GetAgent = pBlackboard->GetData("Agent", pAgent);

		std::vector<GOAP::Action*> plan;
		bool getPlan = pBlackboard->GetData("Plan", plan);

		GOAP::WorldState ws;
		bool GetWorldState = pBlackboard->GetData("WorldState", ws);

		if (getPlan && GetWorldState)
		{
			bool performed = plan.front()->Perform(pBlackboard);
			if (performed)
			{
				ws = plan.front()->ActOnWorldState(ws);
				plan.erase(plan.begin());

				if (plan.front()->CanOperateWorldState(ws))
				{
					std::cout << " next action : " << plan.front()->GetName() << "\n";
				}
			}
		}
		pAgent->SetStateName(name);
	}
	virtual std::string GetName()  override
	{
		return name;
	}
	int ID = 3;
	std::string name = "Perform Action";
};



class RunningAction : public GOAP::Action
{
public:
	RunningAction();

	virtual bool Perform(Elite::Blackboard* pBlackboard) override;
	virtual std::string GetName() const override { return m_Name; }
	bool RequireRange() override;
	void SetRange(float range) { m_Range = range; }
	float GetRange() const { return m_Range; }
	void SetTarget(SteeringAgent* agent);
	void SetTarget(Elite::Vector2 pos);
	virtual Elite::Vector2 GetTarget() { return m_Target; }

	float m_Range = 0.f;
	bool m_InRange = false;
	SteeringAgent* m_pTarget;
	Elite::Vector2 m_Target;

	std::string m_Name = "Running";
	
};
class ApproachClose : public GOAP::Action
{
public:
	ApproachClose();

	virtual bool Perform(Elite::Blackboard* pBlackboard) override;
	virtual std::string GetName() const override { return m_Name; }
	bool RequireRange() override;

	void SetRange(float range) { m_Range = range; }
	float GetRange() const { return m_Range; }
	void SetTarget(SteeringAgent* agent) { m_Target = agent->GetPosition(); }
	void SetTarget(Elite::Vector2 pos) { m_Target = pos; }
	virtual Elite::Vector2 GetTarget() { return m_Target; }

	float m_Range = 20.f;
	bool m_InRange = false;
	SteeringAgent* m_pTarget;
	Elite::Vector2 m_Target;
	
	std::string m_Name = "ApproachClose";
	
};
class DrawKnife : public GOAP::Action
{
public:
	DrawKnife();

	virtual bool Perform(Elite::Blackboard* pBlackboard) override;
	virtual std::string GetName() const override { return m_Name; }
	bool RequireRange() override;
	void SetRange(float range) { m_Range = range; }
	float GetRange() const { return m_Range; }
	void SetTarget(SteeringAgent* agent) { m_Target = agent->GetPosition(); }
	void SetTarget(Elite::Vector2 pos) { m_Target = pos; }
	virtual Elite::Vector2 GetTarget() { return m_Target; }
	
	bool m_InRange = false;
	SteeringAgent* m_pTarget;
	Elite::Vector2 m_Target;
	float m_Range = 0.f;
	std::string m_Name = "DrawKnife";
};
class SheathKnife : public GOAP::Action
{
public:
	SheathKnife();

	virtual bool Perform(Elite::Blackboard* pBlackboard) override;
	virtual std::string GetName() const override { return m_Name; }
	bool RequireRange() override;
	void SetRange(float range) { m_Range = range; }
	float GetRange() const { return m_Range; }
	void SetTarget(SteeringAgent* agent) { m_Target = agent->GetPosition(); }
	void SetTarget(Elite::Vector2 pos) { m_Target = pos; }
	virtual Elite::Vector2 GetTarget() { return m_Target; }
	
	bool m_InRange = false;
	SteeringAgent* m_pTarget;
	Elite::Vector2 m_Target;
	float m_Range = 0.f;
	std::string m_Name = "SheathKnife";
	
};
class Stab : public GOAP::Action
{
public:
	Stab();

	virtual bool Perform(Elite::Blackboard* pBlackboard) override;
	virtual std::string GetName() const override { return m_Name; }
	bool RequireRange() override;
	virtual void SetRange(float range) override { m_Range = range; }
	virtual float GetRange() const override { return m_Range; }
	virtual void SetTarget(SteeringAgent* agent)override { m_Target = agent->GetPosition(); }
	virtual void SetTarget(Elite::Vector2 pos)override { m_Target = pos; }
	virtual Elite::Vector2 GetTarget() { return m_Target; }
	
	bool m_InRange = false;
	SteeringAgent* m_pTarget;
	Elite::Vector2 m_Target;
	std::string m_Name = "Stab";
	float m_Range = 5.f;
	
};


/////////////////


class TransitionPerformAction : public Elite::FSMTransition
{
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const override
	{
		std::vector<GOAP::Action*> plan;
		GoapAgent* pAgent;
		bool GetAgent = pBlackboard->GetData("Agent", pAgent);
		bool getPlan = pBlackboard->GetData("Plan", plan);
		if (getPlan && GetAgent)
		{
			if (plan.front()->RequireRange())
			{
				if (plan.front()->GetRange() > Elite::Distance(pAgent->GetPosition(), plan.front()->GetTarget()))
				{
					std::cout << " go to PerformAction \n";
					return true;
				}
			}
			else
			{
				return false;
			}
			
		}
		return false;
	}
};

class Transition : public Elite::FSMTransition
{

	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const override
	{
		std::vector<GOAP::Action*> plan;

		bool getPlan = pBlackboard->GetData("Plan", plan);

		//ApproachClose CurrentAction;

		//bool GetAction = pBlackboard->GetData("Action", CurrentAction);

		if (getPlan )
		{

			if (plan.front()->RequireRange())
			{
				std::cout << "Going to move to\n";
				return true;

			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
};
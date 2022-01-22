#pragma once
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "framework/EliteAI/EliteData/EBlackboard.h"
#include "projects/Shared/GoapAgents/GoapAgent.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPAction.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPPlanner.h"
#include "projects/Shared/GoapAgents/Weapon.h"
#include "projects/Shared/GoapAgents/Knife.h"
#include "projects/Shared/GoapAgents/Gun.h"



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
		pAgent->SetActionPath(PlannedActions);
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

		// Get Agent
		GoapAgent* pAgent = nullptr;
		bool GetAgent = pBlackboard->GetData("Agent", pAgent);
		// Get Target from action 
		if (GetAgent)
		{
			auto Path = pAgent->GetActionPath();
			Elite::Vector2 target;
			if (!Path.empty())
			{
				target = Path.front()->GetTarget();
			}
			if (GetAgent)
			{
				// set steering
				pAgent->SetToArrival(target);
			}

			pAgent->SetStateName(name);
		}
		
		
	}
	virtual void OnExit(Elite::Blackboard* pBlackboard)override
	{
		// disable steering
		GoapAgent* pAgent = nullptr;
		bool GetAgent = pBlackboard->GetData("Agent", pAgent);

		if (GetAgent)
		{
			pAgent->SetToStill();
		}
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

		// GetAgent 
		
		GetAgent = pBlackboard->GetData("Agent", pAgent);

		
		GetWorldSate = pBlackboard->GetData("WorldState", ws);

		performed = false;
		// get agents actions
		if (GetAgent && GetWorldSate)
		{
			
			pAgent->SetStateName(name); 
		}
	}
	virtual void Update(Elite::Blackboard* pBlackboard, float deltaTime)
	{
		if (GetAgent && GetWorldSate)
		{
			auto path = pAgent->GetActionPath();
			if (!path.empty())
			{
				if (path.front()->CanOperateWorldState(ws))
				{
					//action perform
					performed = path.front()->Perform(pBlackboard);
				}
				// if action performed , Set effects , remove action from list 
				if (performed)
				{
					ws = path.front()->ActOnWorldState(ws);
					path.erase(path.begin());
					pBlackboard->ChangeData("WorldState", ws);
					pAgent->SetActionPath(path);
				}
			}
		}
	}
	virtual std::string GetName()  override
	{
		return name;
	}
	GOAP::WorldState ws;
	GoapAgent* pAgent = nullptr;
	bool GetAgent = false;
	bool GetWorldSate = false;
	bool performed = false;
	std::string name = "Perform Action";
};





class Stabbing : public GOAP::Action
{
public :
	Stabbing();

	virtual bool Perform(Elite::Blackboard* pBlackboard) override;

	bool RequireRange()override
	{
		if (m_Range > 0.f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetRange(float range) { m_Range = range; }
	float GetRange() const { return m_Range; }

	void SetTarget(Elite::Vector2 position) { m_Target = position; }
	virtual Elite::Vector2 GetTarget() { return m_Target; }
	std::string GetName()const override { return m_Name; }

private:
	float m_Range;
	Elite::Vector2 m_Target;

	std::string m_Name = "Stabbing";
};
class Shooting : public GOAP::Action
{
public:
	Shooting();

	virtual bool Perform(Elite::Blackboard* pBlackboard) override;

	bool RequireRange()override
	{
		if (m_Range > 0.f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetRange(float range) { m_Range = range; }
	float GetRange() const { return m_Range; }

	void SetTarget(Elite::Vector2 position) { m_Target = position; }
	virtual Elite::Vector2 GetTarget() { return m_Target; }

	std::string GetName()const override { return m_Name; }
private:
	float m_Range;
	Elite::Vector2 m_Target;

	std::string m_Name = "Shooting";
};
class SelfDestruct : public GOAP::Action
{
public:
	SelfDestruct();

	virtual bool Perform(Elite::Blackboard* pBlackboard) override;

	bool RequireRange()override
	{
		if (m_Range > 0.f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetRange(float range) { m_Range = range; }
	float GetRange() const { return m_Range; }

	void SetTarget(Elite::Vector2 position) { m_Target = position; }
	virtual Elite::Vector2 GetTarget() { return m_Target; }
	std::string GetName()const override { return m_Name; }
private:
	float m_Range;
	Elite::Vector2 m_Target;

	std::string m_Name = "SelfDestruct";
};
class PickUpKnife : public GOAP::Action
{
public:
	PickUpKnife();

	virtual bool Perform(Elite::Blackboard* pBlackboard) override;

	bool RequireRange()override
	{
		if (m_Range > 0.f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetRange(float range) { m_Range = range; }
	float GetRange() const { return m_Range; }

	void SetTarget(Elite::Vector2 position) { m_Target = position; }
	virtual Elite::Vector2 GetTarget() { return m_Target; }
	std::string GetName()const override { return m_Name; }
private:
	float m_Range;
	Elite::Vector2 m_Target;

	std::string m_Name = "PickUpKnife";
};
class PickUpGun : public GOAP::Action
{
public:
	PickUpGun();

	virtual bool Perform(Elite::Blackboard* pBlackboard) override;

	bool RequireRange()override
	{
		if (m_Range > 0.f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetRange(float range) { m_Range = range; }
	float GetRange() const { return m_Range; }

	void SetTarget(Elite::Vector2 position) { m_Target = position; }
	virtual Elite::Vector2 GetTarget() { return m_Target; }
	std::string GetName()const override { return m_Name; }
private:
	float m_Range;
	Elite::Vector2 m_Target;

	std::string m_Name = "PickUpGun";
};
class PickUpBomb : public GOAP::Action
{
public:
	PickUpBomb();

	virtual bool Perform(Elite::Blackboard* pBlackboard) override;

	bool RequireRange()override
	{
		if (m_Range > 0.f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetRange(float range) { m_Range = range; }
	float GetRange() const { return m_Range; }

	void SetTarget(Elite::Vector2 position) { m_Target = position; }
	virtual Elite::Vector2 GetTarget() { return m_Target; }
	std::string GetName()const override { return m_Name; }
private:
	float m_Range;
	Elite::Vector2 m_Target;

	std::string m_Name = "PickUpBomb";
};
class DrawKnife : public GOAP::Action
{
public:
	DrawKnife();

	virtual bool Perform(Elite::Blackboard* pBlackboard) override;

	bool RequireRange()override
	{
		if (m_Range > 0.f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetRange(float range) { m_Range = range; }
	float GetRange() const { return m_Range; }

	void SetTarget(Elite::Vector2 position) { m_Target = position; }
	virtual Elite::Vector2 GetTarget() { return m_Target; }
	std::string GetName()const override { return m_Name; }
private:
	float m_Range;
	Elite::Vector2 m_Target;

	std::string m_Name = "DrawKnife";
};
class DrawGun : public GOAP::Action
{
public:
	DrawGun();

	virtual bool Perform(Elite::Blackboard* pBlackboard) override;

	bool RequireRange()override
	{
		if (m_Range > 0.f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetRange(float range) { m_Range = range; }
	float GetRange() const { return m_Range; }

	void SetTarget(Elite::Vector2 position) { m_Target = position; }
	virtual Elite::Vector2 GetTarget() { return m_Target; }
	std::string GetName()const override { return m_Name; }
private:
	float m_Range;
	Elite::Vector2 m_Target;

	std::string m_Name = "DrawGun";
};
class DrawBomb : public GOAP::Action
{
public:
	DrawBomb();

	virtual bool Perform(Elite::Blackboard* pBlackboard) override;

	bool RequireRange()override
	{
		if (m_Range > 0.f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetRange(float range) { m_Range = range; }
	float GetRange() const { return m_Range; }

	void SetTarget(Elite::Vector2 position) { m_Target = position; }
	virtual Elite::Vector2 GetTarget() { return m_Target; }
	std::string GetName()const override { return m_Name; }
private:
	float m_Range;
	Elite::Vector2 m_Target;

	std::string m_Name = "DrawBomb";
};
class SheatKnife : public GOAP::Action
{
public:
	SheatKnife();

	virtual bool Perform(Elite::Blackboard* pBlackboard) override;

	bool RequireRange()override
	{
		if (m_Range > 0.f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetRange(float range) { m_Range = range; }
	float GetRange() const { return m_Range; }

	void SetTarget(Elite::Vector2 position) { m_Target = position; }
	virtual Elite::Vector2 GetTarget() { return m_Target; }
	std::string GetName()const override { return m_Name; }
private:
	float m_Range;
	Elite::Vector2 m_Target;

	std::string m_Name = "SheatKnife";
};
class SheatGun : public GOAP::Action
{
public:
	SheatGun();

	virtual bool Perform(Elite::Blackboard* pBlackboard) override;

	bool RequireRange()override 
	{
		if (m_Range > 0.f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetRange(float range) { m_Range = range; }
	float GetRange() const { return m_Range; }

	void SetTarget(Elite::Vector2 position) { m_Target = position; }
	virtual Elite::Vector2 GetTarget() { return m_Target; }
	std::string GetName()const override { return m_Name; }
private:
	float m_Range;
	Elite::Vector2 m_Target;

	std::string m_Name = "SheatGun";
};
/////////////////


class TransitionPerformAction : public Elite::FSMTransition
{
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const override
	{
		std::vector<GOAP::Action*> plan;
		GoapAgent* pAgent;
		bool GetAgent = pBlackboard->GetData("Agent", pAgent);
		plan = pAgent->GetActionPath();
		if ( GetAgent)
		{
			if (plan.front()->RequireRange())
			{
				if (plan.front()->GetRange() > Elite::Distance(pAgent->GetPosition(), plan.front()->GetTarget()))
				{
					std::cout << " go to PerformAction \n";
					return true;
				}
				return false;
			}
			else if((!plan.front()->RequireRange()))
			{
				if (!plan.front()->GetRange() > 0.f)
				{
					std::cout << " go to PerformAction \n";
						return true;
				}
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
class TransitionMoveTo : public Elite::FSMTransition
{
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const override
	{
		std::vector<GOAP::Action*> Plan;
		GoapAgent* pAgent;

		bool GetAgent = pBlackboard->GetData("Agent", pAgent);
		
		Plan = pAgent->GetActionPath();

		if (GetAgent && !Plan.empty())
		{
			if (Plan.front()->RequireRange())
			{
				if (Plan.front()->GetRange() < Elite::Distance(pAgent->GetPosition(), Plan.front()->GetTarget()))
				{
					std::cout << " from performAction to MoveTo \n";
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
		return false;
	}
};
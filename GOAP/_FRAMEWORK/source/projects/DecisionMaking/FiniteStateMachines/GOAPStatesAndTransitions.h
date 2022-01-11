#pragma once
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "framework/EliteAI/EliteData/EBlackboard.h"
#include "projects/Shared/GoapAgents/GoapAgent.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPAction.h"


class Idle : public Elite::FSMState
{
public:
	virtual void OnEnter(Elite::Blackboard* pBlackboard)override
	{
		std::vector<GOAP::Action> actions;

		bool GetActions = pBlackboard->GetData("Actions", actions);

		/*if (GetActions)
		{
			for (size_t i = 0; i < actions.size(); i++)
			{
				auto preCon = actions[i].GetPreConditions();
				for (auto p : preCon)
				{
				  GetConditionState(p.first).
				}
				GetConditionState();
			}
		}*/
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
		
		
	}
	virtual std::string GetName()  override
	{
		return name;
	}
	int ID = 2;
	std::string name = "Move To";
};
class PerformAction : public Elite::FSMState
{
public:
	virtual void OnEnter(Elite::Blackboard* pBlackboard)override
	{

	}
	virtual std::string GetName()  override
	{
		return name;
	}
	int ID = 3;
	std::string name = "Perform Action";
};


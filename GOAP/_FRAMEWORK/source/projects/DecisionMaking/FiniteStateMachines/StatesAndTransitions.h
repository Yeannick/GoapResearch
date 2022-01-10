/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
/*=============================================================================*/
// StatesAndTransitions.h: Implementation of the state/transition classes
/*=============================================================================*/
#ifndef ELITE_APPLICATION_FSM_STATES_TRANSITIONS
#define ELITE_APPLICATION_FSM_STATES_TRANSITIONS

#include "projects/Shared/Agario/AgarioAgent.h"
#include "projects/Shared/Agario/AgarioFood.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "framework/EliteAI/EliteData/EBlackboard.h"

//------------
//---STATES---
//------------

// state that makes the agent wander
class WanderState : public Elite::FSMState
{
public:
	virtual void OnEnter(Elite::Blackboard* pBlackboard)override
	{
		//std::cout << "WanderState" << std::endl;
		AgarioAgent* pAgent = nullptr;
		bool Succes = pBlackboard->GetData("Agent", pAgent);
		
		if (!Succes)
		{
			return;
		}
		pAgent->SetToWander();
	};
private :

};
// state that makes the agent seek for food

class SeekForFood : public Elite::FSMState
{
public :
	virtual void OnEnter(Elite::Blackboard* pBlackboard) override
	{
		std::cout << " Seeking for food State " << std::endl;
		AgarioAgent* pAgent = nullptr;
		bool Succes = pBlackboard->GetData("Agent", pAgent);
		AgarioFood* targetFood = nullptr;
		bool foodTarget = pBlackboard->GetData("FoodTarget", targetFood);

		if (foodTarget && Succes)
		{
			if (targetFood)
			{
				pAgent->SetToSeek(targetFood->GetPosition());
			}
			
		}
		
	}
};
// Evade bigger agents state 
class EvadeState : public Elite::FSMState
{
	virtual void OnEnter(Elite::Blackboard* pBlackboard) override
	{
		std::cout << "Evading State " << std::endl;
		const float radius = 15.f;
		AgarioAgent* pAgent = nullptr;
		AgarioAgent* pFleeTarget = nullptr;
		const bool succesGettingAgent = pBlackboard->GetData("Agent", pAgent);
		const bool SuccesGettingFleeTarget = pBlackboard->GetData("AgentFleeTarget", pFleeTarget);
		
		pAgent->SetToFlee(pFleeTarget->GetPosition());

	}
};
// Pursuit smaller agents state
class PursuitState : public Elite::FSMState
{
	virtual void OnEnter(Elite::Blackboard* pBlackboard) override
	{
		std::cout << "Pursuit State " << std::endl;
		const float radius = 15.f;
		AgarioAgent* pAgent = nullptr;
		std::vector<AgarioAgent*>* pAgents;
		const bool succesGettingAgent = pBlackboard->GetData("Agent", pAgent);
		const bool succesGettingOtherAgents = pBlackboard->GetData("AgentsVec", pAgents);
		if (succesGettingAgent && succesGettingOtherAgents)
		{
			for (auto a : *pAgents)
			{
				const float distance = Elite::Distance(pAgent->GetPosition(), a->GetPosition());
				if (distance < radius)
				{
					if (a->GetRadius() < pAgent->GetRadius())
					{
						pAgent->SetToSeek(a->GetPosition());
					}

				}
			}
		}
	}
};
// move From border state
class EvadeBorder : public Elite::FSMState
{
	virtual void OnEnter(Elite::Blackboard* pBlackboard) override
	{
		AgarioAgent* pAgent = nullptr;
		const bool succesGettingAgent = pBlackboard->GetData("Agent", pAgent);
		float TrimWorldSize = 0.f;
		bool succes = pBlackboard->GetData("WorldSize", TrimWorldSize);
		pAgent->SetToSeek(Elite::randomVector2(0, TrimWorldSize));
		
	}
};
//-----------------
//---TRANSITIONS---
//-----------------

class TransitionToSeekFood : public Elite::FSMTransition
{
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const override
	{
		// get Agent
		AgarioAgent* pAgent = nullptr;
		const bool succesGettingAgent = pBlackboard->GetData("Agent", pAgent);
		// get FoodVector
		std::vector<AgarioFood*>* food;
		const bool succes = pBlackboard->GetData("FoodVec", food);
		// if both are succes 
		if (succesGettingAgent && succes)
		{
			// loop over food
			for (auto f : *food)
			{
				// calculate distance and if food is nearby
				if (Elite::Distance(pAgent->GetPosition(), f->GetPosition()) < 15.f)
				{
					// create targetFood and changeData in the blackboard
					AgarioFood* targetFood = f;
					pBlackboard->ChangeData("FoodTarget", targetFood);
						
					return true;
				}
			}
			
		}
		return false;
	}
};
class TransitionToEvade : public Elite::FSMTransition
{
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const override
	{
		AgarioAgent* pAgent = nullptr;
		std::vector<AgarioAgent*>* pAgents;
		const bool succesGettingAgent = pBlackboard->GetData("Agent", pAgent);
		const bool succesGettingOtherAgents = pBlackboard->GetData("AgentsVec", pAgents);
		const float evadeRadius = 25.f;

		if (succesGettingAgent && succesGettingOtherAgents)
		{
			for (auto a : *pAgents)
			{
				const float distance = Elite::Distance(pAgent->GetPosition(), a->GetPosition());
				if (distance < evadeRadius)
				{
					if (a->GetRadius()-1 > pAgent->GetRadius())
					{
						pBlackboard->ChangeData("AgentFleeTarget", a);
						return true;
					}

				}
			}
		}
		return false;
	}
};
class TransitionToPursuit : public Elite::FSMTransition
{
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const override
	{
		AgarioAgent* pAgent = nullptr;
		std::vector<AgarioAgent*> *pAgents;
		const bool succesGettingAgent = pBlackboard->GetData("Agent", pAgent);
		const bool succesGettingOtherAgents = pBlackboard->GetData("AgentsVec", pAgents);

		if (succesGettingAgent && succesGettingOtherAgents)
		{
			for (auto a :* pAgents)
			{
				const float distance = Elite::Distance(pAgent->GetPosition(), a->GetPosition());
				if (distance < 15.f)
				{
					if (a->GetRadius()+1 < pAgent->GetRadius())
					{
						return true;
					}

				}
			}
		}
		return false;
	}
};
class TransitionWander : public Elite::FSMTransition
{
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const override
	{
		AgarioAgent* pAgent = nullptr;
		const bool succesGettingAgent = pBlackboard->GetData("Agent", pAgent);
		std::vector<AgarioFood*>* food;
		const bool succes = pBlackboard->GetData("FoodVec", food);
		if (succesGettingAgent && succes)
		{
			for (auto f : *food)
			{
				if (Elite::Distance(pAgent->GetPosition(), f->GetPosition()) > 15.f)
				{
					std::cout << "to wander State" << std::endl;
					return true;
				}
			}

		}
		return false;
	}
};
class TransitionToWanderFromEvade : public Elite::FSMTransition
{
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const override
	{
		AgarioAgent* pAgent = nullptr;
		std::vector<AgarioAgent*>* pAgents;
		const bool succesGettingAgent = pBlackboard->GetData("Agent", pAgent);
		const bool succesGettingOtherAgents = pBlackboard->GetData("AgentsVec", pAgents);
		const float evadeRadius = 25.f;

		if (succesGettingAgent && succesGettingOtherAgents)
		{
			for (auto a : *pAgents)
			{
				if (a->GetRadius() - 1 > pAgent->GetRadius())
				{
					const float distance = Elite::Distance(pAgent->GetPosition(), a->GetPosition());
					if (distance > evadeRadius)
					{
						return true;
					}

				}
			}
		}
		return false;
	}
};
class TransitionToWanderFromPursuit : public Elite::FSMTransition
{
	virtual bool ToTransition(Elite::Blackboard* pBlackboard) const override
	{
		AgarioAgent* pAgent = nullptr;
		std::vector<AgarioAgent*>* pAgents;
		const bool succesGettingAgent = pBlackboard->GetData("Agent", pAgent);
		const bool succesGettingOtherAgents = pBlackboard->GetData("AgentsVec", pAgents);
		const float PursuitRadius = 15.f;
		if (succesGettingAgent && succesGettingOtherAgents)
		{
			for (auto a : *pAgents)
			{
				if (a->GetRadius() + 1 < pAgent->GetRadius())
				{
					const float distance = Elite::Distance(pAgent->GetPosition(), a->GetPosition());
					if (distance > PursuitRadius)
					{
						return true;
					}

				}
			}
		}
		return false;
	}
};
class TransitionBorderEvade : public Elite::FSMTransition
{
	
	bool ToTransition(Elite::Blackboard* pBlackboard) const override
	{
		AgarioAgent* pAgent = nullptr;
		const bool succesGettingAgent = pBlackboard->GetData("Agent", pAgent);
		float TrimWorldSize = 0.f;
		bool succes = pBlackboard->GetData("WorldSize", TrimWorldSize);
		const float radius = 10.f;
		if (succes && succesGettingAgent)
		{
			if (pAgent->GetPosition().x + radius > TrimWorldSize || pAgent->GetPosition().x - radius < 0)
			{
				return true;
			}
			if (pAgent->GetPosition().y + radius > TrimWorldSize || pAgent->GetPosition().y - radius < 0)
			{
				return true;
			}
		}
		return false;
	}
};
#endif
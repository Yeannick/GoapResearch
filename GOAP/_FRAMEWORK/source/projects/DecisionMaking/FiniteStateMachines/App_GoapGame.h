#pragma once
#include "framework/EliteInterfaces/EIApp.h"
#include "projects/Shared/GoapAgents/GoapAgent.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPAction.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPNode.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPPlanner.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPWorldState.h"
#include "projects/Shared/GoapAgents/Weapon.h"
#include "projects/Shared/GoapAgents/Knife.h"
#include "projects/Shared/GoapAgents/Gun.h"
#include "projects/Shared/GoapAgents/Bomb.h"

#include "GOAPStatesAndTransitions.h"

class App_GoapGame final : public IApp
{
public :
	App_GoapGame();
	~App_GoapGame();

	void Start()override;
	void Update(float deltaTime) override;
	void Render(float deltaTime) const override;

	std::vector<GOAP::Action*> GetActions();
	void SetStateName(std::string name) { m_StateName = name; }
private:
	float m_TrimWorldSize = 100.f;;
	
	// Action will be used as states in FSM to make AI do stuff
	std::vector<Elite::FSMState*> m_pStates{};
	std::vector<GoapAgent*> m_GoapAgents;
	std::vector<GoapAgent*> m_pEnemies;
	bool GetConditionState(int conditionID);
	std::vector<GOAP::Action*> m_Actions;
	std::string m_StateName;


	std::vector<Weapon*> m_Weapons;
	std::vector<GOAP::WorldState> m_GoalStates;
	Elite::Blackboard* pBlackBoard = nullptr;
	bool IsGoalKnife = true;
	bool IsGoalShoot = false;
	bool IsGoalBomb = false;
	void UpdateImGui();
	// worldstate

	GOAP::WorldState m_WorldState;

	// Actions 

	Stabbing* m_pStabbing = nullptr;
	Shooting* m_pShooting = nullptr;
	SelfDestruct* m_pSelfDestruct = nullptr;
	PickUpKnife* m_pPickUpKnife = nullptr;
	PickUpGun* m_pPickUpGun = nullptr;
	PickUpBomb* m_pPickUpBomb = nullptr;
	DrawKnife* m_pDrawKnife = nullptr;
	DrawGun* m_pDrawGun = nullptr;
	DrawBomb* m_pDrawBomb = nullptr;
	SheatKnife* m_pSheatKnife = nullptr;
	SheatGun* m_pSheatGun = nullptr;
	SheatBomb* m_pSheatBomb = nullptr;
};


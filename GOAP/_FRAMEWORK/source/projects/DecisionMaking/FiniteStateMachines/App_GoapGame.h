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
	void UpdateImGui();
};


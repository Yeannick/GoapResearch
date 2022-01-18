#include "stdafx.h"
#include "App_GoapGame.h"
#include "GOAPStatesAndTransitions.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPAction.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteFiniteStateMachine/EFiniteStateMachine.h"
using namespace Elite;
App_GoapGame::App_GoapGame()
{

}

App_GoapGame::~App_GoapGame()
{
	for (auto& s : m_pStates)
	{
		SAFE_DELETE(s);
	}
}

void App_GoapGame::Start()
{
	auto pAgent = new GoapAgent({ 50.f,20.f }, Elite::Color{ 0.f,1.f,0.f });

	auto pEnemy = new GoapAgent({ 80.f,80.f }, Elite::Color{ 1.f,1.f,0.f });

	

	m_pEnemies.push_back(pEnemy);
	RunningAction* pRunning = new RunningAction();

	pRunning->AddPrecondition("EnemySighted", false);
	pRunning->AddPrecondition("WeaponInHand", true);
	pRunning->AddEffect("EnemySighted", true);
	pRunning->SetCost(150);
	pRunning->SetName(pRunning->GetName());
	pRunning->SetTarget(pEnemy);

	m_Actions.push_back(pRunning);

	ApproachClose* pApproachClose = new ApproachClose();
	
	pApproachClose->AddPrecondition("EnemySighted", true);
	pApproachClose->AddPrecondition("EnemyDead", false);
	pApproachClose->AddPrecondition("EnemyInCloseRange", false);
	pApproachClose->AddEffect("EnemyInCloseRange", true);
	pApproachClose->AddEffect("EnemySighted", true);
	pApproachClose->SetCost(4);
	pApproachClose->SetName(pApproachClose->GetName());
	pApproachClose->SetTarget(pEnemy);
	m_Actions.push_back(pApproachClose);

	DrawKnife* pDrawKnife = new DrawKnife();

	pDrawKnife->AddPrecondition("InventoryKnife" , true);
	pDrawKnife->AddPrecondition("WeaponInHand" , false);
	pDrawKnife->AddPrecondition("KnifeDrawn" , false);
	pDrawKnife->AddEffect("KnifeDrawn",true);
	pDrawKnife->AddEffect("WeaponInHand" , true);
	pDrawKnife->SetCost(1);
	pDrawKnife->SetName(pDrawKnife->GetName());
	m_Actions.push_back(pDrawKnife);

	SheathKnife* pSheathKnife = new SheathKnife();

	pSheathKnife->AddPrecondition("WeaponInHand", true);
	pSheathKnife->AddPrecondition("KnifeDrawn", true);
	pSheathKnife->AddEffect("KnifeDrawn", false);
	pSheathKnife->AddEffect("WeaponInHand", false);
	pSheathKnife->SetCost(1);
	pSheathKnife->SetName(pSheathKnife->GetName());
	m_Actions.push_back(pSheathKnife);
	Stab* pStab = new Stab();
	
	pStab->AddPrecondition("EnemySighted", true);
	pStab->AddPrecondition("EnemyDead", false);
	pStab->AddPrecondition("KnifeDrawn", true);
	pStab->AddPrecondition("EnemyInCloseRange", true);
	pStab->AddEffect("EnemyDead", true);
	pStab->SetCost(3);
	pStab->SetName(pStab->GetName());
	pStab->SetTarget(pEnemy);
	m_Actions.push_back(pStab);

    // Now establish some goal states and an initial state
    GOAP::WorldState goal_win;
    goal_win.SetVariable("EnemyDead", true);
    goal_win.SetVariable("MeDead", false);
    goal_win.SetVariable("WeaponInHand", false);
    
    goal_win.priority = 100;

    // You can tweak these (e.g. have_ammo, the inventory items) to
    // elicit different plans from the AI.
    GOAP::WorldState initial_state;
    initial_state.SetVariable("EnemyDead", false);
    initial_state.SetVariable("EnemySighted", false);
    initial_state.SetVariable("EnemyInRange", false);
    initial_state.SetVariable("EnemyInCloseRange", false);
    initial_state.SetVariable("GunLoaded", false);
    initial_state.SetVariable("GunDrawn", false);
    initial_state.SetVariable("KnifeDrawn", true);
    initial_state.SetVariable("WeaponInHand", true);
    initial_state.SetVariable("MeDead", false);
    initial_state.SetVariable("HaveAmmo", false);
    initial_state.SetVariable("InventoryKnife", true);
    initial_state.SetVariable("InventoryGun", true);

    GOAP::WorldState currentWorldState;

    
    // Fire up the A* planner
   
    
    Elite::Blackboard* pBlackBoard = new Elite::Blackboard();
    Idle* pIdleState = new Idle();
	MoveTo *pMoveToState = new MoveTo();
	PerformAction* pPerformActionState = new PerformAction();
    m_pStates.push_back(pIdleState);
	m_pStates.push_back(pMoveToState);
	m_pStates.push_back(pPerformActionState);

   
   
   
	pAgent->SetActions(m_Actions);


	
	Transition* TransitionMoveTo = new Transition();
	TransitionPerformAction* TransitionPerform = new TransitionPerformAction();

	

    pBlackBoard->AddData("Agent", pAgent);
    pBlackBoard->AddData("Enemies", m_pEnemies);
    pBlackBoard->AddData("Actions", m_Actions );
    pBlackBoard->AddData("WorldState", initial_state);
    pBlackBoard->AddData("GoalState", goal_win);
    pBlackBoard->AddData("Plan", std::vector<GOAP::Action*>());
	pBlackBoard->AddData("Target", pEnemy);

    FiniteStateMachine* pFSM = new FiniteStateMachine(m_pStates[0], pBlackBoard);
	pFSM->AddTransition(pIdleState, pMoveToState, TransitionMoveTo);
	pFSM->AddTransition(pMoveToState, pPerformActionState, TransitionPerform);
    pAgent->SetDecisionMaking(pFSM);

	

    m_GoapAgents.push_back(pAgent);
}

void App_GoapGame::Update(float deltaTime)
{
	UpdateImGui();

	for (auto agent : m_GoapAgents)
	{
		agent->Update(deltaTime);
		agent->LimitToWorld(m_TrimWorldSize);
		m_StateName = agent->GetStateName();
	}
	for (auto e : m_pEnemies)
	{
		//e->Update(deltaTime);
		e->LimitToWorld(m_TrimWorldSize);
	}
}

void App_GoapGame::Render(float deltaTime) const
{
	std::vector<Elite::Vector2> points =
	{
		{ -m_TrimWorldSize, m_TrimWorldSize },
		{ m_TrimWorldSize, m_TrimWorldSize },
		{ m_TrimWorldSize, -m_TrimWorldSize },
		{ -m_TrimWorldSize, -m_TrimWorldSize }
	};
	DEBUGRENDERER2D->DrawPolygon(&points[0], 4, { 1,0,0,1 }, 0.4f);

	for (auto agent : m_GoapAgents)
	{
		agent->Render(deltaTime);
	}
}

bool App_GoapGame::GetConditionState(int conditionID)
{
    return false;

}

void App_GoapGame::UpdateImGui()
{
	//------- UI --------
#ifdef PLATFORM_WINDOWS
#pragma region UI
	{
		//Setup
		int menuWidth = 150;
		int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
		int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
		bool windowActive = true;
		ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 10, 10));
		ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height - 90));
		ImGui::Begin("GOAP", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::PushAllowKeyboardFocus(false);
		ImGui::SetWindowFocus();
		ImGui::PushItemWidth(70);
		//Elements
		ImGui::Text("CONTROLS");
		ImGui::Indent();
		ImGui::Unindent();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("STATS");
		ImGui::Indent();
		ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Unindent();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Agent Info");
		ImGui::Text("State: ");
      
         ImGui::Text(m_StateName.c_str());
        
      
		ImGui::Text("Survive Time: %.1f", TIMER->GetTotal());

		//End
		ImGui::PopAllowKeyboardFocus();
		ImGui::End();
	}
#endif
}

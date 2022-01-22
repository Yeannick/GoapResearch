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

	auto pEnemy = new GoapAgent({ 50.f,-30.f }, Elite::Color{ 1.f,1.f,0.f });

	Gun* pGun = new Gun({ -40.f, 30.f },2.f);
	m_Weapons.push_back(pGun);
	pAgent->SetRenderBehavior(true);

	m_pEnemies.push_back(pEnemy);
	
	Stabbing* pStabbing = new Stabbing();
	pStabbing->SetCost(4);
	Shooting* pShooting = new Shooting();
	pShooting->SetCost(3);
	pShooting->SetTarget(pEnemy->GetPosition());
	SelfDestruct* pSelfDestruct = new SelfDestruct();
	pSelfDestruct->SetCost(6);
	PickUpKnife* pPickUpKnife = new PickUpKnife();
	pPickUpKnife->SetCost(2);
	PickUpGun* pPickUpGun = new PickUpGun();
	pPickUpGun->SetCost(2);
	pPickUpGun->SetTarget(pGun->GetPosition());
	PickUpBomb* pPickUpBomb = new PickUpBomb();
	pPickUpBomb->SetCost(2);
	DrawKnife* pDrawKnife = new DrawKnife();
	pDrawKnife->SetCost(1);
	DrawGun* pDrawGun = new DrawGun();
	pDrawGun->SetCost(2);
	DrawBomb* pDrawBomb = new DrawBomb();
	pDrawBomb->SetCost(3);
	SheatKnife* pSheatKnife = new SheatKnife();
	pSheatKnife->SetCost(1);
	SheatGun* pSheatGun = new SheatGun();
	pSheatGun->SetCost(2);
	
	m_Actions.push_back(pStabbing);
	m_Actions.push_back(pShooting);
	m_Actions.push_back(pSelfDestruct);
	m_Actions.push_back(pPickUpKnife);
	m_Actions.push_back(pPickUpGun);
	m_Actions.push_back(pPickUpBomb);
	m_Actions.push_back(pDrawKnife);
	m_Actions.push_back(pDrawGun);
	m_Actions.push_back(pDrawBomb);
	m_Actions.push_back(pSheatKnife);
	m_Actions.push_back(pSheatGun);
	
    // Now establish some goal states and an initial state
    GOAP::WorldState goal_win;
    goal_win.SetVariable("EnemyDead", true);
    goal_win.SetVariable("MeDead", false);
  //  goal_win.SetVariable("WeaponInHand", false);
    
    goal_win.priority = 100;

    // You can tweak these (e.g. have_ammo, the inventory items) to
    // elicit different plans from the AI.
    GOAP::WorldState initial_state;
    initial_state.SetVariable("EnemyDead", false);
    initial_state.SetVariable("KnifeAvailable", true);
    initial_state.SetVariable("KnifeInInventory", false);
    initial_state.SetVariable("KnifeInHand", false);
    initial_state.SetVariable("GunAvailable", true);
    initial_state.SetVariable("GunInInventory", false);
    initial_state.SetVariable("GunInHand", false);
    initial_state.SetVariable("WeaponInHand", false);
    initial_state.SetVariable("BombAvailable", true);
    initial_state.SetVariable("BombInInventory", false);
    initial_state.SetVariable("BombInHand", false);
	initial_state.SetVariable("MeDead", false);

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


	
	Transition* MoveToTransition = new Transition();
	TransitionPerformAction* TransitionPerform = new TransitionPerformAction();
	TransitionMoveTo* TransitionPerformMoveTo = new TransitionMoveTo();
	

    pBlackBoard->AddData("Agent", pAgent);
    pBlackBoard->AddData("Enemies", m_pEnemies);
    pBlackBoard->AddData("Actions", m_Actions );
    pBlackBoard->AddData("WorldState", initial_state);
    pBlackBoard->AddData("GoalState", goal_win);
    pBlackBoard->AddData("Plan", std::vector<GOAP::Action*>());
	pBlackBoard->AddData("Target", pEnemy);
	pBlackBoard->AddData("Weapons", &m_Weapons);

    FiniteStateMachine* pFSM = new FiniteStateMachine(m_pStates[0], pBlackBoard);
	pFSM->AddTransition(pIdleState, pMoveToState, MoveToTransition);
	pFSM->AddTransition(pMoveToState, pPerformActionState, TransitionPerform);
	pFSM->AddTransition(pPerformActionState,pMoveToState,TransitionPerformMoveTo);
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
	for (auto w : m_Weapons)
	{
		w->Update(deltaTime);
		if (w->IsOverlapping(m_GoapAgents[0]))
		{
		//	std::cout << "overlap\n";
		}
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
	for (auto w : m_Weapons )
	{
		if (!w->IsPickedUp())
		{
			w->Render(deltaTime);
		}
		
	}
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
		ImGui::Text("State : ");
      
        ImGui::Text(m_StateName.c_str());

		ImGui::Text("Action : ");
		if (!m_GoapAgents[0]->GetActionPath().empty())
		{
			ImGui::Text(m_GoapAgents[0]->GetActionPath().front()->GetName().c_str());
		}
		ImGui::Text("Inventory :");
		for (size_t i = 0; i < m_GoapAgents[0]->GetInventory().size(); i++)
		{
			ImGui::Text(m_GoapAgents[0]->GetInventory()[i]->GetName().c_str());
		}
		
      
		ImGui::Text("Survive Time: %.1f", TIMER->GetTotal());

		//End
		ImGui::PopAllowKeyboardFocus();
		ImGui::End();
	}
#endif
}

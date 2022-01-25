#include "stdafx.h"
#include "App_GoapGame.h"

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

	

	Gun* pGun = new Gun({ -40.f, 30.f },2.f);
	Knife* pKnife = new Knife({ 40.f,50.f }, 2.f);
	Bomb* pBomb = new Bomb({ 0,70.f }, 2.f);
	m_Weapons.push_back(pGun);
	m_Weapons.push_back(pKnife);
	m_Weapons.push_back(pBomb);
	pAgent->SetRenderBehavior(true);

	m_pEnemies.push_back(new GoapAgent({ 50.f,-30.f }, Elite::Color{ 1.f,1.f,0.f }));
	
	m_pStabbing = new Stabbing();
	m_pStabbing->SetCost(4);
	m_pStabbing->SetTarget(m_pEnemies[0]->GetPosition());

	m_pShooting = new Shooting();
	m_pShooting->SetCost(3);
	m_pShooting->SetTarget(m_pEnemies[0]->GetPosition());

	m_pSelfDestruct = new SelfDestruct();
	m_pSelfDestruct->SetCost(6);
	m_pSelfDestruct->SetTarget(m_pEnemies[0]->GetPosition());

	m_pPickUpKnife = new PickUpKnife();
	m_pPickUpKnife->SetCost(2);
	m_pPickUpKnife->SetTarget(pKnife->GetPosition());

	m_pPickUpGun = new PickUpGun();
	m_pPickUpGun->SetCost(2);
	m_pPickUpGun->SetTarget(pGun->GetPosition());

	m_pPickUpBomb = new PickUpBomb();
	m_pPickUpBomb->SetTarget(pBomb->GetPosition());
	m_pPickUpBomb->SetCost(2);

	m_pDrawKnife = new DrawKnife();
	m_pDrawKnife->SetCost(1);

	m_pDrawGun = new DrawGun();
	m_pDrawGun->SetCost(2);

	m_pDrawBomb = new DrawBomb();
	m_pDrawBomb->SetCost(3);

	m_pSheatKnife = new SheatKnife();
	m_pSheatKnife->SetCost(1);

	m_pSheatGun = new SheatGun();
	m_pSheatGun->SetCost(2);

	m_pSheatBomb = new SheatBomb();
	m_pSheatBomb->SetCost(2);
	
	m_Actions.push_back(m_pStabbing);
	m_Actions.push_back(m_pShooting);
	m_Actions.push_back(m_pSelfDestruct);
	m_Actions.push_back(m_pPickUpKnife);
	m_Actions.push_back(m_pPickUpGun);
	m_Actions.push_back(m_pPickUpBomb);
	m_Actions.push_back(m_pDrawKnife);
	m_Actions.push_back(m_pDrawGun);
	m_Actions.push_back(m_pDrawBomb);
	m_Actions.push_back(m_pSheatKnife);
	m_Actions.push_back(m_pSheatGun);
	m_Actions.push_back(m_pSheatBomb);
	
    // Now establish some goal states and an initial state
    GOAP::WorldState goalKnife;
	goalKnife.SetVariable("EnemyDead", true);
	goalKnife.SetVariable("KnifeInHand", true);
	goalKnife.SetVariable("MeDead", false);
  //  goal_win.SetVariable("WeaponInHand", false);
    
	goalKnife.priority = 100;

	m_GoalStates.push_back(goalKnife);

	GOAP::WorldState goalShoot;
	goalShoot.SetVariable("EnemyDead", true);
	goalShoot.SetVariable("MeDead", false);
	goalShoot.SetVariable("GunInHand", true);
	//  goal_win.SetVariable("WeaponInHand", false);

	goalShoot.priority = 100;

	m_GoalStates.push_back(goalShoot);

	GOAP::WorldState goalSelfdestruct;
	goalSelfdestruct.SetVariable("EnemyDead", true);
	goalSelfdestruct.SetVariable("MeDead", true);
	goalSelfdestruct.SetVariable("BombInHand", true);
	m_GoalStates.push_back(goalSelfdestruct);
    // You can tweak these (e.g. have_ammo, the inventory items) to
    // elicit different plans from the AI.
   
	m_WorldState.SetVariable("EnemyDead", false);
	m_WorldState.SetVariable("KnifeAvailable", true);
	m_WorldState.SetVariable("KnifeInInventory", false);
	m_WorldState.SetVariable("KnifeInHand", false);
	m_WorldState.SetVariable("GunAvailable", true);
	m_WorldState.SetVariable("GunInInventory", false);
	m_WorldState.SetVariable("GunInHand", false);
	m_WorldState.SetVariable("WeaponInHand", false);
	m_WorldState.SetVariable("BombAvailable", true);
	m_WorldState.SetVariable("BombInInventory", false);
	m_WorldState.SetVariable("BombInHand", false);
	m_WorldState.SetVariable("MeDead", false);

   

    
    // Fire up the A* planner
   
    
    pBlackBoard = new Elite::Blackboard();
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
	TransitionIdle* TransitionToIdle = new TransitionIdle();

    pBlackBoard->AddData("Agent", pAgent);
    pBlackBoard->AddData("Enemies", m_pEnemies);
    pBlackBoard->AddData("Actions", m_Actions );
    pBlackBoard->AddData("WorldState", m_WorldState);
    pBlackBoard->AddData("GoalState", goalKnife);
    pBlackBoard->AddData("Plan", std::vector<GOAP::Action*>());
	pBlackBoard->AddData("Target", m_pEnemies[0]);
	pBlackBoard->AddData("Weapons", &m_Weapons);

    FiniteStateMachine* pFSM = new FiniteStateMachine(m_pStates[0], pBlackBoard);
	pFSM->AddTransition(pIdleState, pMoveToState, MoveToTransition);
	pFSM->AddTransition(pMoveToState, pPerformActionState, TransitionPerform);
	pFSM->AddTransition(pPerformActionState,pMoveToState,TransitionPerformMoveTo);
	pFSM->AddTransition(pPerformActionState, pIdleState, TransitionToIdle);
	pFSM->AddTransition(pIdleState, pPerformActionState, TransitionPerform);
    pAgent->SetDecisionMaking(pFSM);

	

    m_GoapAgents.push_back(pAgent);
}

void App_GoapGame::Update(float deltaTime)
{
	UpdateImGui();
	if (IsGoalKnife)
	{
		pBlackBoard->ChangeData("GoalState", m_GoalStates[0]);
		m_WorldState.SetVariable("MeDead", false);
	}
	else if (IsGoalShoot)
	{
		pBlackBoard->ChangeData("GoalState", m_GoalStates[1]);
		m_WorldState.SetVariable("MeDead", false);
	}
	else if (IsGoalBomb)
	{
		pBlackBoard->ChangeData("GoalState", m_GoalStates[2]);
		m_WorldState.SetVariable("MeDead", false);
	}
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
	if (m_pEnemies[0]->GetIsKilled())
	{
		delete m_pEnemies[0];
		m_pEnemies.erase(m_pEnemies.begin());
		
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
	for (auto e : m_pEnemies)
	{
		//e->Update(deltaTime);
		e->Render(deltaTime);

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
		static int e = 0;
		ImGui::RadioButton("Knife",&e, 0);
		ImGui::RadioButton("Gun", &e, 1);
		ImGui::RadioButton("Bomb", &e, 2);

		if (e == 0)
		{
			IsGoalKnife = true;
			IsGoalShoot = false;
			IsGoalBomb = false;
			if (m_pEnemies.empty())
			{
				m_pEnemies.push_back( new GoapAgent({ Elite::randomFloat(50),Elite::randomFloat(50) }, Elite::Color{ 1.f,1.f,0.f }));
				pBlackBoard->GetData("WorldState", m_WorldState);
				m_WorldState.SetVariable("EnemyDead", false);
				m_WorldState.SetVariable("MeDead", false);
				m_pStabbing->SetTarget(m_pEnemies[0]->GetPosition());
				m_pShooting->SetTarget(m_pEnemies[0]->GetPosition());
				m_pSelfDestruct->SetTarget(m_pEnemies[0]->GetPosition());
				pBlackBoard->ChangeData("WorldState", m_WorldState);
				pBlackBoard->ChangeData("Target", m_pEnemies[0]);
			}
		}
		if (e == 1)
		{
			IsGoalKnife = false;
			IsGoalShoot = true;
			IsGoalBomb = false;
			if (m_pEnemies.empty())
			{
				m_pEnemies.push_back(new GoapAgent({ Elite::randomFloat(50),Elite::randomFloat(50) }, Elite::Color{ 1.f,1.f,0.f }));
				pBlackBoard->GetData("WorldState", m_WorldState);
				m_WorldState.SetVariable("EnemyDead", false);
				m_WorldState.SetVariable("MeDead", false);
				m_pStabbing->SetTarget(m_pEnemies[0]->GetPosition());
				m_pShooting->SetTarget(m_pEnemies[0]->GetPosition());
				m_pSelfDestruct->SetTarget(m_pEnemies[0]->GetPosition());
				pBlackBoard->ChangeData("WorldState", m_WorldState);
				pBlackBoard->ChangeData("Target", m_pEnemies[0]);
			}
		}
		if (e == 2)
		{
			IsGoalKnife = false;
			IsGoalShoot = false;
			IsGoalBomb = true;
			if (m_pEnemies.empty())
			{
				m_pEnemies.push_back(new GoapAgent({ Elite::randomFloat(50),Elite::randomFloat(50) }, Elite::Color{ 1.f,1.f,0.f }));
				pBlackBoard->GetData("WorldState", m_WorldState);
				m_WorldState.SetVariable("EnemyDead", false);
				m_WorldState.SetVariable("MeDead", false);

				m_pStabbing->SetTarget(m_pEnemies[0]->GetPosition());
				m_pShooting->SetTarget(m_pEnemies[0]->GetPosition());
				m_pSelfDestruct->SetTarget(m_pEnemies[0]->GetPosition());
				pBlackBoard->ChangeData("WorldState", m_WorldState);
				pBlackBoard->ChangeData("Target", m_pEnemies[0]);
				
			}
		}
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

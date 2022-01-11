#include "stdafx.h"
#include "App_GoapGame.h"
#include "GOAPStatesAndTransitions.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPAction.h"
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
	const int enemy_sighted = 1;
	const int enemy_dead = 2;
	const int enemy_in_range = 3;
	const int enemy_in_close_range = 4;
	const int inventory_knife = 5;
	const int inventory_gun = 6;
	const int gun_drawn = 7;;
	const int gun_loaded = 8;
	const int have_ammo = 9;
	const int knife_drawn = 10;
	const int weapon_in_hand = 11;
	const int me_dead = 12;

   
    GOAP::Action scout("scoutStealthily", 250,1);
    scout.SetPrecondition(enemy_sighted, false);
    scout.SetPrecondition(weapon_in_hand, true);
    scout.SetEffect(enemy_sighted, true);
    m_Actions.push_back(scout);

    GOAP::Action run("scoutRunning", 150,2);
    run.SetPrecondition(enemy_sighted, false);
    run.SetPrecondition(weapon_in_hand, true);
    run.SetEffect(enemy_sighted, true);
    m_Actions.push_back(run);

    GOAP::Action approach("closeToGunRange", 2,3);
    approach.SetPrecondition(enemy_sighted, true);
    approach.SetPrecondition(enemy_dead, false);
    approach.SetPrecondition(enemy_in_range, false);
    approach.SetPrecondition(gun_loaded, true);
    approach.SetEffect(enemy_in_range, true);
    m_Actions.push_back(approach);

    GOAP::Action approachClose("closeToKnifeRange", 4,4);
    approachClose.SetPrecondition(enemy_sighted, true);
    approachClose.SetPrecondition(enemy_dead, false);
    approachClose.SetPrecondition(enemy_in_close_range, false);
    approachClose.SetEffect(enemy_in_close_range, true);
    m_Actions.push_back(approachClose);

    GOAP::Action load("loadGun", 2,5);
    load.SetPrecondition(have_ammo, true);
    load.SetPrecondition(gun_loaded, false);
    load.SetPrecondition(gun_drawn, true);
    load.SetEffect(gun_loaded, true);
    load.SetEffect(have_ammo, false);
    m_Actions.push_back(load);

    GOAP::Action draw("drawGun", 1,6);
    draw.SetPrecondition(inventory_gun, true);
    draw.SetPrecondition(weapon_in_hand, false);
    draw.SetPrecondition(gun_drawn, false);
    draw.SetEffect(gun_drawn, true);
    draw.SetEffect(weapon_in_hand, true);
    m_Actions.push_back(draw);

    GOAP::Action holster("holsterGun", 1,7);
    holster.SetPrecondition(weapon_in_hand, true);
    holster.SetPrecondition(gun_drawn, true);
    holster.SetEffect(gun_drawn, false);
    holster.SetEffect(weapon_in_hand, false);
    m_Actions.push_back(holster);

    GOAP::Action drawKnife("drawKnife", 1,8);
    drawKnife.SetPrecondition(inventory_knife, true);
    drawKnife.SetPrecondition(weapon_in_hand, false);
    drawKnife.SetPrecondition(knife_drawn, false);
    drawKnife.SetEffect(knife_drawn, true);
    drawKnife.SetEffect(weapon_in_hand, true);
    m_Actions.push_back(drawKnife);

    GOAP::Action sheath("sheathKnife", 1,9);
    sheath.SetPrecondition(weapon_in_hand, true);
    sheath.SetPrecondition(knife_drawn, true);
    sheath.SetEffect(knife_drawn, false);
    sheath.SetEffect(weapon_in_hand, false);
    m_Actions.push_back(sheath);

    GOAP::Action shoot("shootEnemy", 3,10);
    shoot.SetPrecondition(enemy_sighted, true);
    shoot.SetPrecondition(enemy_dead, false);
    shoot.SetPrecondition(gun_drawn, true);
    shoot.SetPrecondition(gun_loaded, true);
    shoot.SetPrecondition(enemy_in_range, true);
    shoot.SetEffect(enemy_dead, true);
    m_Actions.push_back(shoot);

    GOAP::Action knife("knifeEnemy", 3,11);
    knife.SetPrecondition(enemy_sighted, true);
    knife.SetPrecondition(enemy_dead, false);
    knife.SetPrecondition(knife_drawn, true);
    knife.SetPrecondition(enemy_in_close_range, true);
    knife.SetEffect(enemy_dead, true);
    m_Actions.push_back(knife);

    GOAP::Action destruct("selfDestruct", 30,12);
    destruct.SetPrecondition(enemy_sighted, true);
    destruct.SetPrecondition(enemy_dead, false);
    destruct.SetPrecondition(enemy_in_range, true);
    destruct.SetEffect(enemy_dead, true);
    destruct.SetEffect(me_dead, true);
    m_Actions.push_back(destruct);

    // Now establish some goal states and an initial state
    GOAP::WorldState goal_win;
    goal_win.SetVariable(enemy_dead, true);
    goal_win.SetVariable(me_dead, false);
    goal_win.SetVariable(weapon_in_hand, false);
    
    goal_win.priority = 100;

    // You can tweak these (e.g. have_ammo, the inventory items) to
    // elicit different plans from the AI.
    GOAP::WorldState initial_state;
    initial_state.SetVariable(enemy_dead, false);
    initial_state.SetVariable(enemy_sighted, false);
    initial_state.SetVariable(enemy_in_range, false);
    initial_state.SetVariable(enemy_in_close_range, false);
    initial_state.SetVariable(gun_loaded, false);
    initial_state.SetVariable(gun_drawn, false);
    initial_state.SetVariable(knife_drawn, false);
    initial_state.SetVariable(weapon_in_hand, false);
    initial_state.SetVariable(me_dead, false);
    initial_state.SetVariable(have_ammo, true);
    initial_state.SetVariable(inventory_knife, true);
    initial_state.SetVariable(inventory_gun, true);

    // Fire up the A* planner
    GOAP::Planner as;
    std::vector<GOAP::Action> the_plan;
    try {
        the_plan = as.Plan(initial_state, goal_win, m_Actions);
        std::cout << "\n";
        std::cout << "Found a path!\n";
        for (std::vector<GOAP::Action>::reverse_iterator rit = the_plan.rbegin(); rit != the_plan.rend(); ++rit) {
            std::cout << rit->GetName() << std::endl;
        }
    }
    catch (const std::exception&) {
        std::cout << "Sorry, could not find a path!\n";
    }
    
    Elite::Blackboard* pBlackBoard = new Elite::Blackboard();
   
  

    for (auto p : the_plan)
    {
       
      
    }
   
   
    auto pAgent = new GoapAgent({ 50.f,20.f }, Elite::Color{ 0.f,1.f,0.f });
    auto pEnemy = new GoapAgent({ 100.f,80.f }, Elite::Color{ 1.f,1.f,0.f });

    std::vector<GoapAgent*> pEnemies;

    pEnemies.push_back(pEnemy);

    pBlackBoard->AddData("Agent", pAgent);
    pBlackBoard->AddData("Enemies", pEnemies);
    pBlackBoard->AddData("Actions", m_Actions );
    //FiniteStateMachine* pFSM = new FiniteStateMachine(m_pStates[0], pBlackBoard);
    //pAgent->SetDecisionMaking(pFSM);
    m_GoapAgents.push_back(pAgent);
}

void App_GoapGame::Update(float deltaTime)
{
	UpdateImGui();

	for (auto agent : m_GoapAgents)
	{
		agent->Update(deltaTime);
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
        for (size_t i = 0; i < m_pStates.size(); i++)
        {
            ImGui::Text(m_pStates[i]->GetName().c_str());
        }
      
		ImGui::Text("Survive Time: %.1f", TIMER->GetTotal());

		//End
		ImGui::PopAllowKeyboardFocus();
		ImGui::End();
	}
#endif
}

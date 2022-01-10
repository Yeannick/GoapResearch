#pragma once
#include "framework/EliteInterfaces/EIApp.h"
#include "projects/Shared/GoapAgents/GoapAgent.h"
class App_GoapGame final : public IApp
{
public :
	App_GoapGame();
	~App_GoapGame();

	void Start()override;
	void Update(float deltaTime) override;
	void Render(float deltaTime) const override;

private:
	float m_TrimWorldSize = 100.f;;
	
	// Action will be used as states in FSM to make AI do stuff
	std::vector<Elite::FSMState*> m_pStates{};
	std::vector<GoapAgent*> m_GoapAgents;
	void UpdateImGui();
};


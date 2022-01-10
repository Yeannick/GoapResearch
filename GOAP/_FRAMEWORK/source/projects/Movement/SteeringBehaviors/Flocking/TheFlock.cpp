#include "stdafx.h"
#include "TheFlock.h"

#include "../SteeringAgent.h"
#include "../Steering/SteeringBehaviors.h"
#include "../CombinedSteering/CombinedSteeringBehaviors.h"

//Constructor & Destructor
Flock::Flock(
	int flockSize /*= 50*/, 
	float worldSize /*= 100.f*/, 
	SteeringAgent* pAgentToEvade /*= nullptr*/, 
	bool trimWorld /*= false*/)

	: m_WorldSize{ worldSize }
	, m_FlockSize{ flockSize }
	, m_TrimWorld { trimWorld }
	, m_pAgentToEvade{pAgentToEvade}
	, m_NeighborhoodRadius{ 10 }
	, m_NrOfNeighbors{0}
{
	m_Agents.resize(m_FlockSize);
	

	m_pSeparationBehavior = new Separation{ this };
	m_pCohesionBehavior = new Cohesion{ this };
	m_pAlignmenthBehavior = new Alignment{ this };
	m_pSeekBehavior = new Seek();
	m_pWanderBehavior = new Wander();
	std::vector<BlendedSteering::WeightedBehavior> weightedSteeringBehavior;
	weightedSteeringBehavior.push_back({ m_pCohesionBehavior,0.25f });
	weightedSteeringBehavior.push_back({ m_pSeparationBehavior,0.25f });
	weightedSteeringBehavior.push_back({ m_pAlignmenthBehavior,0.25f });
	weightedSteeringBehavior.push_back({ m_pSeekBehavior,0.0f });
	weightedSteeringBehavior.push_back({ m_pWanderBehavior,0.25f });
	// TODO: initialize the flock and the memory pool
	for (int i = 0; i < m_FlockSize; i++)
	{
		SteeringAgent* Agent = new SteeringAgent();
		Agent->SetPosition(Elite::Vector2{ Elite::randomInt(m_WorldSize) * 1.f,Elite::randomInt(m_WorldSize) * 1.f }); 
		

		m_pBlendedSteering = new BlendedSteering(weightedSteeringBehavior);
		Agent->SetSteeringBehavior(m_pBlendedSteering);
		Agent->SetRenderBehavior(false);
		m_Agents[i] = Agent ;

	}
	
	m_Agents[0]->SetRenderBehavior(true);
}

Flock::~Flock()
{
	// TODO: clean up any additional data


	SAFE_DELETE(m_pBlendedSteering);
	SAFE_DELETE(m_pPrioritySteering);
	SAFE_DELETE(m_pCohesionBehavior);
	SAFE_DELETE(m_pAlignmenthBehavior);
	SAFE_DELETE(m_pSeekBehavior);
	SAFE_DELETE(m_pWanderBehavior);

	for(auto pAgent: m_Agents)
	{
		SAFE_DELETE(pAgent);
	}
	m_Agents.clear();
}

void Flock::Update(float deltaT)
{
	// TODO: update the flock
	// loop over all the agents
	// 
	
	for (auto pAgent : m_Agents )
	{
		if (pAgent)
		{
			RegisterNeighbors(pAgent);
			
			pAgent->Update(deltaT);
			if (m_TrimWorld)
			{
				
				pAgent->TrimToWorld({ 0,0 }, { m_WorldSize,m_WorldSize });
			}
		}
		
	}
	m_Agents[0]->SetBodyColor(Elite::Color{ 1.f,1.f,1.f,1.f });
	RegisterNeighbors(m_Agents[0]);
	for (auto Neighbour : m_Neighbors)
	{
		Neighbour->SetBodyColor(Elite::Color{ 0.f,1.f,0.f,1.f });
	}
	if (m_Agents[0]->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawCircle(m_Agents[0]->GetPosition(), m_NeighborhoodRadius, Elite::Color{ 1.f,1.f,1.f,1.f }, 0);

		
	}
		// register its neighbors	(-> memory pool is filled with neighbors of the currently evaluated agent)
		// update it				(-> the behaviors can use the neighbors stored in the pool, next iteration they will be the next agent's neighbors)
		// trim it to the world
}

void Flock::Render(float deltaT)
{
	// TODO: render the flock
	
	for (auto pAgent : m_Agents)
	{
		if (pAgent != nullptr)
		{
			pAgent->Render(deltaT);
			
			
		}
	}
}

void Flock::RegisterNeighbors(SteeringAgent* pAgent)
{
	m_Neighbors.clear();
	//std::cout << m_Neighbors.size() << std::endl;
	for (auto OtherAgent : m_Agents)
	{
		if (OtherAgent != nullptr)
		{
			if (OtherAgent != pAgent)
			{
				/*if (OtherAgent->CanRenderBehavior())
				{
					DEBUGRENDERER2D->DrawCircle(pAgent->GetPosition(), m_NeighborhoodRadius, Elite::Color{ 1.f,1.f,1.f,1.f }, 0);
				}*/
				Elite::Vector2 ToTarget = OtherAgent->GetPosition() - pAgent->GetPosition();
				float distance = ToTarget.Magnitude();

				if (distance < m_NeighborhoodRadius)
				{
					m_Neighbors.push_back(OtherAgent);
					m_NrOfNeighbors = m_Neighbors.size();

					//std::cout << m_NrOfNeighbors << " // " << std::endl;
					
				}
			}
		}
		
	}
	
}

void Flock::UpdateAndRenderUI()
{
	//Setup
	int menuWidth = 235;
	int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
	int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
	bool windowActive = true;
	ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 10, 10));
	ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height - 20));
	ImGui::Begin("Gameplay Programming", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::PushAllowKeyboardFocus(false);

	//Elements
	ImGui::Text("CONTROLS");
	ImGui::Indent();
	ImGui::Text("LMB: place target");
	ImGui::Text("RMB: move cam.");
	ImGui::Text("Scrollwheel: zoom cam.");
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

	ImGui::Text("Flocking");
	ImGui::Spacing();

	ImGui::SliderFloat("Cohesion", &m_pBlendedSteering->GetWeightedBehaviorsRef()[0].weight, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("Separation", &m_pBlendedSteering->GetWeightedBehaviorsRef()[1].weight, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("Alignment", &m_pBlendedSteering->GetWeightedBehaviorsRef()[2].weight, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("Seek", &m_pBlendedSteering->GetWeightedBehaviorsRef()[3].weight, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("Wander", &m_pBlendedSteering->GetWeightedBehaviorsRef()[4].weight, 0.f, 1.f, "%.2");
	
	// TODO: Implement checkboxes for debug rendering and weight sliders here

	//End
	ImGui::PopAllowKeyboardFocus();
	ImGui::End();
	
}

Elite::Vector2 Flock::GetAverageNeighborPos() const
{
	Elite::Vector2 average{};
	if (m_NrOfNeighbors > 0)
	{

		for (auto neighbour : m_Neighbors)
		{
			average += neighbour->GetPosition();
		}

	
		average /= m_NrOfNeighbors;
	}
	//std::cout << average << std::endl;
	return average;
}

Elite::Vector2 Flock::GetAverageNeighborVelocity() const
{
	Elite::Vector2 average{};
	if (m_NrOfNeighbors > 0)
	{
		for (auto neighbour : m_Neighbors)
		{
			average += neighbour->GetLinearVelocity();
		}

		average /= m_NrOfNeighbors;
	}
	//std::cout << average << std::endl;
	return average;
}

void Flock::SetSeekTarget(TargetData target)
{
	// TODO: set target for Seek behavior
	m_pSeekBehavior->SetTarget(target);
}

float* Flock::GetWeight(ISteeringBehavior* pBehavior) 
{
	if (m_pBlendedSteering)
	{
		auto& weightedBehaviors = m_pBlendedSteering->GetWeightedBehaviorsRef();
		auto it = find_if(weightedBehaviors.begin(),
			weightedBehaviors.end(),
			[pBehavior](BlendedSteering::WeightedBehavior el)
			{
				return el.pBehavior == pBehavior;
			}
		);

		if(it!= weightedBehaviors.end())
			return &it->weight;
	}

	return nullptr;
}

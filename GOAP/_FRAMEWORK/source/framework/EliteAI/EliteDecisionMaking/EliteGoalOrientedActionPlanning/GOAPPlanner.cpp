#include "stdafx.h"
#include "GOAPPlanner.h"

GOAP::Planner::Planner()
{
}

void GOAP::Planner::PrintOpenList() const
{
	for (const auto& n : m_OpenList) {
		std::cout << n << std::endl;
	}
}

void GOAP::Planner::PrintClosedList() const
{
	for (const auto& n : m_ClosedList) {
		std::cout << n << std::endl;
	}
}



bool GOAP::Planner::OnClosedList(WorldState& worldState)
{
	if (std::find_if(m_ClosedList.begin(), m_ClosedList.end(), [&](const Node& n) { return n.Worldstate == worldState; }) == m_ClosedList.end()) 
	{
		return false;
	}
	return true;
}

std::vector<GOAP::Node>::iterator GOAP::Planner::OnOpenList(const WorldState& worldState)
{
	return std::find_if(m_OpenList.begin(), m_OpenList.end(), [&](const Node& n) { return n.Worldstate == worldState; });
}

GOAP::Node& GOAP::Planner::AddToClosed()
{
	assert(!m_OpenList.empty());
	m_ClosedList.push_back(std::move(m_OpenList.front()));
	m_OpenList.erase(m_OpenList.begin());

	return m_ClosedList.back();
}

void GOAP::Planner::AddToOpenList(Node& n)
{
	auto it = std::lower_bound(m_OpenList.begin(),m_OpenList.end(),n);
	m_OpenList.emplace(it, std::move(n));
}

int GOAP::Planner::CalculateHeuristic(const WorldState& current, const WorldState& goal) const
{
	return current.DistanceTo(goal);
}

std::vector<GOAP::Action*> GOAP::Planner::Plan(const WorldState& start, const WorldState& goal, const std::vector<Action*> actions)
{
	if (start.MeetsGoal(goal))
	{
		return std::vector<GOAP::Action*>();
	}
	m_OpenList.clear();
	m_ClosedList.clear();

	Node StartNode(start, 0, CalculateHeuristic(start, goal), 0, nullptr);
	

	m_OpenList.push_back(std::move(StartNode));

	while (m_OpenList.size() > 0)
	{
		Node& current(AddToClosed());

		if (current.Worldstate.MeetsGoal(goal))
		{
			std::vector<GOAP::Action*> Plan;
			do
			{
				Plan.push_back(current.action);
				//Plan->push_back(current.action);
				auto It = std::find_if(m_OpenList.begin(), m_OpenList.end(), [&](const Node& n)
					{
						return n.Id == current.ParentId;
					});
				if (It == m_OpenList.end()) {
					It = std::find_if(m_ClosedList.begin(), m_ClosedList.end(), [&](const Node& n) 
						{
							return n.Id == current.ParentId; 
						});
				}
				current = *It;
			} 
			while (current.ParentId != 0);
			if (current.action)
			{
				Plan.push_back(current.action);
			}
			
			return Plan;
		}
		for (const auto& Potential : actions)
		{
			if (Potential->CanOperateWorldState(current.Worldstate))
			{
				WorldState outcome = Potential->ActOnWorldState(current.Worldstate);

				if (OnClosedList(outcome))
				{
					continue;
				}

				auto outComeNode = OnOpenList(outcome);

				if (outComeNode == m_OpenList.end())
				{
					Node found(outcome, current.gCost + Potential->GetCost(), CalculateHeuristic(outcome, goal), current.Id, Potential);
					AddToOpenList(std::move(found));
				}
				else
				{
					if (current.gCost + Potential->GetCost() < outComeNode->gCost) {
						outComeNode->ParentId = current.Id;                  
						outComeNode->gCost = current.gCost + Potential->GetCost(); 
						outComeNode->hCost = CalculateHeuristic(outcome, goal);
						outComeNode->action = Potential;

						std::sort(m_OpenList.begin(), m_OpenList.end());
					}
				}
			}
		}
	}


	throw std::runtime_error("A* planner could not find a path from start to goal");
}
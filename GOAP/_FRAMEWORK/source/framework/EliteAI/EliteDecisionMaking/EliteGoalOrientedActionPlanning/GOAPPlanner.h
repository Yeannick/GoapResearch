#pragma once

#include "framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPAction.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPNode.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPWorldState.h"

#include <ostream>
#include <unordered_map>
#include <vector>

namespace GOAP
{
	class Planner
	{
	
	public:
		Planner();

		void PrintOpenList() const;
		void PrintClosedList() const;

		std::vector<Action*> Plan(const WorldState& start, const WorldState& goal, const std::vector<Action*> actions);
	private:
		// A* Open AND closedList
		std::vector<Node> m_OpenList;
		std::vector<Node> m_ClosedList;

		bool OnClosedList(WorldState& worldState);
		std::vector<GOAP::Node>::iterator OnOpenList(const WorldState& worldState);
		// removes node from open list , adds it to closed and returns a reference to the newly added node on the closed
		Node& AddToClosed();
		void AddToOpenList(Node& n);
		int CalculateHeuristic(const WorldState& current, const WorldState& goal) const;
	};
}



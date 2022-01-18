#pragma once
#include "framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPAction.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteGoalOrientedActionPlanning/GOAPWorldState.h"

namespace GOAP 
{
	struct Node
	{

		static int LastID; // a static that lets me assign incrementing, unique IDs to nodes
		WorldState Worldstate;
		int Id;
		int ParentId;
		int gCost;
		int hCost;
		Action* action;

		Node();
		Node(const WorldState state, int g, int h, int parentId, Action* action);

		// Fcost is g + h cost
		int GetFCost () const { return gCost + hCost; }
		friend std::ostream& operator<<(std::ostream& out, const Node& n);
	};

	bool operator<(const Node& lhs, const Node& rhs);

	inline std::ostream& operator<<(std::ostream& out, const Node& n) {
		out << "Node { id:" << n.Id << " parent:" << n.ParentId << " F:" << n.GetFCost() << " G:" << n.gCost << " H:" << n.hCost;
		out << ", " << n.Worldstate << "}";
		return out;
	}

	
	
}



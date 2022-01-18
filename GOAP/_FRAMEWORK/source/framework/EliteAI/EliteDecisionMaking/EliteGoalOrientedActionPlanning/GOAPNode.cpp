#include "stdafx.h"
#include "GOAPNode.h"

int GOAP::Node::LastID = 0;

GOAP::Node::Node(): gCost(0), hCost(0)
{
	Id = ++LastID;
}

GOAP::Node::Node(const WorldState state, int g, int h, int parentId,  Action* action):
	Worldstate(state),
	gCost(g),
	hCost(h),
	ParentId(parentId),
	action(action)
{
	Id = LastID++;
}
bool GOAP::operator<(const Node& lhs, const Node& rhs)
{
	return lhs.GetFCost() < rhs.GetFCost();
}
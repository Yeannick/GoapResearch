#include "stdafx.h"
#include "GOAPAction.h"

GOAP::Action::Action() : m_Cost(0)
{
}

GOAP::Action::Action(std::string name, int cost)
{
	m_Name = name;
	m_Cost = cost;
}

bool GOAP::Action::CanOperateWorldState(const GOAP::WorldState& worldstate) const
{
	return false;
}

GOAP::WorldState GOAP::Action::ActOnWorldState(const GOAP::WorldState& ws) const
{
	return GOAP::WorldState();
}

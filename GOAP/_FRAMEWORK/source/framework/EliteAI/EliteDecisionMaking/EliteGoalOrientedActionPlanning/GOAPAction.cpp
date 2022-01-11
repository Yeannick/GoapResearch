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

GOAP::Action::Action(std::string name, int cost, int id)
{
	m_Name = name;
	m_Cost = cost;
	m_ID = id;
}

bool GOAP::Action::CanOperateWorldState(const GOAP::WorldState& worldstate) const
{
	for (const auto& precond : m_PreConditions)
	{
		try
		{
			if (worldstate.Variables.at(precond.first)!= precond.second)
			{
				return false;
			}
		}
		catch (const std::out_of_range&)
		{
			return false;
		}
	}
	return true;
}

GOAP::WorldState GOAP::Action::ActOnWorldState(const GOAP::WorldState& ws) const
{
	GOAP::WorldState temp(ws);
	for ( const auto& effect : m_Effects)
	{
		temp.SetVariable(effect.first, effect.second);
	}
	return temp;
}

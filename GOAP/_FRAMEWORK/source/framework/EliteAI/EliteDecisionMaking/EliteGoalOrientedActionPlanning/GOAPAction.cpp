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


void GOAP::Action::AddPrecondition(std::string key, bool state)
{
	m_PreConditions.insert({ key,state });
}

void GOAP::Action::RemovePrecondition(std::string key)
{
	m_PreConditions.erase(key);
}

void GOAP::Action::AddEffect(std::string key, bool state)
{
	m_Effects.insert({ key, state });
}

void GOAP::Action::RemoveEffect(std::string key)
{
	m_Effects.erase(key);
}

bool GOAP::Action::RequireRange()
{
	return false;
}

std::unordered_map<std::string, bool> GOAP::Action::GetPreconditons()
{
	return m_PreConditions;
}

std::unordered_map<std::string, bool> GOAP::Action::GetEffect()
{
	return m_Effects;
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


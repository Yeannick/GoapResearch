#include "stdafx.h"
#include "GOAPWorldState.h"

GOAP::WorldState::WorldState(const std::string name):
	priority(0), name(name)
{
}

void GOAP::WorldState::SetVariable(const int varID, const bool value)
{
	Variables[varID] = value;
}

bool GOAP::WorldState::GetVariable(const int varID) const
{
	return Variables.at(varID);
}

bool GOAP::WorldState::MeetsGoal(const WorldState& goalState) const
{
	for ( const auto & value : goalState.Variables)
	{
		try 
		{
			if (Variables.at(value.first)!= value.second)
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

int GOAP::WorldState::DistanceTo(const WorldState& goalState) const
{
	int distance = 0;

	for (const auto& value : goalState.Variables)
	{
		auto It = Variables.find(value.first);
		if (It == Variables.end() || It->second != value.second)
		{
			distance++;
		}
	}
	return distance;
}

bool GOAP::WorldState::operator==(const WorldState& other) const
{
	return Variables == other.Variables;
}

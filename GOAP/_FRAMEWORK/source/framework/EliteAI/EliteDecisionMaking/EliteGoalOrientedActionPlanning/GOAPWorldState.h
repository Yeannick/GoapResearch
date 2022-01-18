#pragma once
#include <ostream>
#include <string>
#include <map>
namespace GOAP
{
	struct WorldState
	{
		float priority; // handy variable to have if this is a goal state, to distinguish from other possible goals

		std::string name;

		std::map<std::string, bool> Variables;

		WorldState(const std::string name = "");

		void SetVariable(const std::string key, const bool value);

		bool GetVariable(const std::string key) const;

		// does this state 'other' meet the requirement of this goal
		bool MeetsGoal(const WorldState& goalState) const;
		int DistanceTo(const WorldState& goalState) const;

		bool operator==(const WorldState& other) const;

		friend std::ostream& operator<<(std::ostream& out, const WorldState& n);

	};
	inline std::ostream& operator<<(std::ostream& out, const WorldState& n) {
		out << "WorldState { ";
		for (const auto& value : n.Variables) 
		{
			out << value.second << " ";
		}
		out << "}";
		return out;
	}
}



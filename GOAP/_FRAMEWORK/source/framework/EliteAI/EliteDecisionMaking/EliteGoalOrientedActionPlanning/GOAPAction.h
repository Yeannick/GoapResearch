#pragma once
namespace GOAP 
{
	struct WorldState;
	class Action
	{
	private:
		std::string m_Name;
		int m_Cost;
		std::unordered_map<int, bool> m_PreConditions;
		std::unordered_map<int, bool> m_Effects;
	public :
		Action();
		Action(std::string name, int cost);

		// can this Action Operate on the given WorldState
		bool CanOperateWorldState(const GOAP::WorldState& worldstate) const;

		GOAP::WorldState ActOnWorldState(const GOAP::WorldState& ws)const;

		void SetPrecondition(const int key , const bool value)
		{
			m_PreConditions[key] = value;
		}
		void SetEffect(const int key, const bool value)
		{
			m_Effects[key] = value;
		}

		int cost() const { return m_Cost; }
		std::string name() const { return m_Name; }


	};



}


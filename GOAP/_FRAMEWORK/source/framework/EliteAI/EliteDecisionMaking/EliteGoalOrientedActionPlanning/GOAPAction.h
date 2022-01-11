#pragma once
#include "GOAPWorldState.h"
namespace GOAP 
{

	struct Precondition
	{
		int conditionID;
		bool state;

	
	};
	//struct WorldState;
	class Action
	{
	private:
		std::string m_Name;
		int m_Cost;
		int m_ID;
		std::unordered_map<int, bool> m_PreConditions;
		std::unordered_map<int, bool> m_Effects;
	public :
		Action();
		Action(std::string name, int cost);
		Action(std::string name, int cost , int id);


		// can this Action Operate on the given WorldState
		//return true if this worldstate meets the preconditions
		bool CanOperateWorldState(const GOAP::WorldState& worldstate) const;

		GOAP::WorldState ActOnWorldState(const GOAP::WorldState& ws)const;

		void SetPrecondition(const int key , const bool value)
		{
			m_PreConditions[key] = value;
		}
		void SetPrecondition(Precondition condition)
		{
			m_PreConditions[condition.conditionID] = condition.state;
		}
		std::unordered_map<int, bool> GetPreConditions() { return m_PreConditions; }
		void SetEffect(const int key, const bool value)
		{
			m_Effects[key] = value;
		}

		int GetCost() const { return m_Cost; }
		std::string GetName() const { return m_Name; }


	};



}


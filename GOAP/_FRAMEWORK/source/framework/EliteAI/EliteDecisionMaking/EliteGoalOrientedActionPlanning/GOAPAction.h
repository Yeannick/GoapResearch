#pragma once
#include "GOAPWorldState.h"
//#include "projects/Shared/GoapAgents/GoapAgent.h"
#include "projects/Movement/SteeringBehaviors/SteeringAgent.h"
namespace GOAP 
{

	
	//struct WorldState;
	class Action
	{
	private:
		std::string m_Name;
		int m_Cost;
		

		std::unordered_map<std::string, bool> m_PreConditions;
		std::unordered_map<std::string, bool> m_Effects;
	public :
		Action();
		Action(std::string name, int cost);
		
		virtual float GetRange() const { return 0.f; }
		virtual void SetRange(float range) {}
		virtual void SetTarget(SteeringAgent* agent) {}
		virtual void SetTarget(Elite::Vector2 pos) {}
		virtual Elite::Vector2 GetTarget() { return Elite::Vector2{}; };

		void AddPrecondition(std::string key, bool state);
		void RemovePrecondition(std::string key);

		void AddEffect(std::string key, bool state);
		void RemoveEffect(std::string key);

		void SetCost(int cost) { m_Cost = cost; }
		virtual bool Perform(Elite::Blackboard* pBlackboard) { return false; };

		virtual bool RequireRange();
		std::unordered_map<std::string, bool>GetPreconditons();
		std::unordered_map<std::string, bool> GetEffect();


		// can this Action Operate on the given WorldState
		////return true if this worldstate meets the preconditions
		bool CanOperateWorldState(const GOAP::WorldState& worldstate) const;

		GOAP::WorldState ActOnWorldState(const GOAP::WorldState& ws)const;
		
		int GetCost() const { return m_Cost; }
		virtual std::string GetName() const { return m_Name; }
		void SetName(std::string name) { m_Name = name; }
		


	};



}


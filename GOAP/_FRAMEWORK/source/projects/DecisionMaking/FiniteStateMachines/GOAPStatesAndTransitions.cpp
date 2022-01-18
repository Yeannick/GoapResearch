#include "stdafx.h"
#include "GOAPStatesAndTransitions.h"

RunningAction::RunningAction()
{
	
}

bool RunningAction::Perform(Elite::Blackboard* pBlackboard)
{
	GoapAgent* pAgent = nullptr;

	bool GetAgent = pBlackboard->GetData("Agent", pAgent);

	GoapAgent* pTarget = nullptr;

	bool GetTarget = pBlackboard->GetData("Target", pTarget);

	if (GetAgent && GetTarget)
	{
		pAgent->SetToSeek(pTarget->GetPosition());

		return true;
	}
	else
	{
		return false;
	}
}

bool RunningAction::RequireRange()
{
	return true;
}

void RunningAction::SetTarget(SteeringAgent* agent)
{
	m_Target = agent->GetPosition();
}

void RunningAction::SetTarget(Elite::Vector2 pos)
{
	m_Target = pos;
}

ApproachClose::ApproachClose()
{
}

bool ApproachClose::Perform(Elite::Blackboard* pBlackboard)
{
	std::cout << "Approached close\n";
	return true;
}

bool ApproachClose::RequireRange()
{
	return true;
}

DrawKnife::DrawKnife()
{
}

bool DrawKnife::Perform(Elite::Blackboard* pBlackboard)
{
	std::cout << "Drawn Knife \n";
	return false;
}

bool DrawKnife::RequireRange()
{
	return false;
}

SheathKnife::SheathKnife()
{
}

bool SheathKnife::Perform(Elite::Blackboard* pBlackboard)
{
	std::cout << "sheated knife\n";
	return false;
}

bool SheathKnife::RequireRange()
{
	return false;
}

Stab::Stab()
{
}

bool Stab::Perform(Elite::Blackboard* pBlackboard)
{
	GoapAgent* pAgent;

	bool GetAgent = pBlackboard->GetData("Agent", pAgent);

	auto target = pAgent->GetTarget();
	if (Elite::Distance(pAgent->GetPosition(),target) < m_Range)
	{
		std::cout << "stabbed enemy\n";
	}

	return true;
}

bool Stab::RequireRange()
{
	return true;
}

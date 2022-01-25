#include "stdafx.h"
#include "GOAPStatesAndTransitions.h"

// Stabbing Class
Stabbing::Stabbing()
{
	AddPrecondition("KnifeInHand", true);
	AddPrecondition("EnemyDead", false);
	AddEffect("EnemyDead",true);
	m_Range = 5.f;
}

bool Stabbing::Perform(Elite::Blackboard* pBlackboard, float dt)
{
	std::cout << " Stabbing Enemy \n";
	GoapAgent* pAgent = nullptr;
	bool getAgent = pBlackboard->GetData("Agent", pAgent);
	GoapAgent* pEnemy = nullptr;
	bool GetEnemy = pBlackboard->GetData("Target", pEnemy);
	pEnemy->SetKilled();
	Elite::Vector2 target = GetTarget();

	Elite::Vector2 dir =  Elite::Vector2{(target.x + 5.f), target.y}- Elite::Vector2{ (target.x-5.f) ,target.y };
	Elite::Vector2 targetleft = { target.x-5.f  , target.y };
	DEBUGRENDERER2D->DrawDirection(targetleft, dir, dir.Magnitude(), Elite::Color{ 1,1,1,1 });
	
	
	return true;
}
// ------------------------------------------------------------------//

Shooting::Shooting()
{
	AddPrecondition("GunInHand", true);
	AddPrecondition("EnemyDead", false);
	AddEffect("EnemyDead", true);
	m_Range = 20.f;
}

bool Shooting::Perform(Elite::Blackboard* pBlackboard, float dt)
{
	std::cout << " Shooting Enemy \n";
	GoapAgent* pAgent = nullptr;
	bool getAgent = pBlackboard->GetData("Agent", pAgent);

	GoapAgent* pEnemy = nullptr;
	bool GetEnemy = pBlackboard->GetData("Target", pEnemy);
	pEnemy->SetKilled();

	Elite::Vector2 target = GetTarget();

	Elite::Vector2 dir = target - pAgent->GetPosition();
	DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), dir, dir.Magnitude(), Elite::Color{ 1,1,1,1 });
	return true;
}
// ------------------------------------------------------------------//

SelfDestruct::SelfDestruct()
{
	AddPrecondition("BombInHand", true);
	AddPrecondition("EnemyDead", false);
	AddEffect("EnemyDead", true);
	AddEffect("MeDead", true);
	m_Range = 2.f;
}

bool SelfDestruct::Perform(Elite::Blackboard* pBlackboard, float dt)
{
	
	m_Time += dt;
	std::cout << " SelfDestructing taking enemy with me \n";
	GoapAgent* pAgent = nullptr;
	bool getAgent = pBlackboard->GetData("Agent", pAgent);

	GoapAgent* pEnemy = nullptr;
	bool GetEnemy = pBlackboard->GetData("Target", pEnemy);
	pEnemy->SetKilled();

	if (getAgent)
	{
		if (m_Time <= 2.f)
		{
			DEBUGRENDERER2D->DrawSolidCircle(pAgent->GetPosition(), 7.5f, { 0,0 }, Elite::Color{ 1.f,0.8f,0.f,1.f });
			GoapAgent* pEnemy = nullptr;
			bool GetEnemy = pBlackboard->GetData("Target", pEnemy);
			pEnemy->SetKilled();
			return true;
		}
		if (m_Time >= 2.f)
		{
			m_Time = 0.f;
			GoapAgent* pEnemy = nullptr;
			bool GetEnemy = pBlackboard->GetData("Target", pEnemy);
			pEnemy->SetKilled();
			return true;
		}
		return false;
	}
	return false;

}
// ------------------------------------------------------------------//

PickUpKnife::PickUpKnife()
{
	AddPrecondition("KnifeAvailable", true);
	AddPrecondition("KnifeInInventory", false);
	AddEffect("KnifeInInventory", true);
	AddEffect("KnifeAvailable", false);
	m_Range = 1.f;
}

bool PickUpKnife::Perform(Elite::Blackboard* pBlackboard, float dt)
{
	std::cout << "Picked Up Knife \n";
	GoapAgent* pAgent = nullptr;
	bool GetAgent = pBlackboard->GetData("Agent", pAgent);
	std::vector<Weapon*>* weapons;
	bool GetGun = pBlackboard->GetData("Weapons", weapons);

	if (GetGun && GetAgent)
	{
		for (size_t i = 0; i < weapons->size(); i++)
		{

			if (weapons->at(i)->GetName() == "Knife")
			{

				pAgent->AddToInventory(weapons->at(i));
				weapons->at(i)->MarkForPickedUp();

			}
		}
	}
	pBlackboard->ChangeData("Weapons", weapons);
	return true;
}
// ------------------------------------------------------------------//

PickUpGun::PickUpGun()
{
	AddPrecondition("GunAvailable", true);
	AddPrecondition("GunInInventory", false);
	AddEffect("GunInInventory", true);
	m_Range = 1.f;
}

bool PickUpGun::Perform(Elite::Blackboard* pBlackboard, float dt)
{
	std::cout << " Picked up Gun \n";
	GoapAgent* pAgent = nullptr;
	bool GetAgent = pBlackboard->GetData("Agent", pAgent);
	std::vector<Weapon*>* weapons;
	bool GetGun = pBlackboard->GetData("Weapons", weapons);
	
	if (GetGun && GetAgent)
	{
		for (size_t i = 0 ; i  < weapons->size() ; i++)
		{
			
			if (weapons->at(i)->GetName() == "Gun")
			{

				pAgent->AddToInventory(weapons->at(i));
				weapons->at(i)->MarkForPickedUp();
				
			}
		}
	}
	pBlackboard->ChangeData("Weapons", weapons);
	return true;
}
// ------------------------------------------------------------------//
PickUpBomb::PickUpBomb()
{
	AddPrecondition("BombAvailable", true);
	AddPrecondition("BombInInventory", false);
	AddEffect("BombInInventory", true);
	m_Range = 1.f;
}

bool PickUpBomb::Perform(Elite::Blackboard* pBlackboard, float dt)
{
	std::cout << " Picked Up Bomb \n";
	GoapAgent* pAgent = nullptr;
	bool GetAgent = pBlackboard->GetData("Agent", pAgent);
	std::vector<Weapon*>* weapons;
	bool GetGun = pBlackboard->GetData("Weapons", weapons);
	if (GetGun && GetAgent)
	{
		for (size_t i = 0; i < weapons->size(); i++)
		{

			if (weapons->at(i)->GetName() == "Bomb")
			{

				pAgent->AddToInventory(weapons->at(i));
				weapons->at(i)->MarkForPickedUp();

			}
		}
	}
	return true;
}
// ------------------------------------------------------------------//

DrawKnife::DrawKnife()
{
	AddPrecondition("KnifeInInventory", true);
	AddPrecondition("KnifeInHand", false);
	AddPrecondition("WeaponInHand", false);
	AddEffect("WeaponInHand", true);
	AddEffect("KnifeInHand", true);

	m_Range = 0.f;
}

bool DrawKnife::Perform(Elite::Blackboard* pBlackboard, float dt)
{
	std::cout << " drawing my Knife \n";
	return true;
}
// ------------------------------------------------------------------//

DrawGun::DrawGun()
{
	AddPrecondition("GunInInventory", true);
	AddPrecondition("GunInHand", false);
	AddPrecondition("WeaponInHand", false);
	AddEffect("WeaponInHand", true);
	AddEffect("GunInHand", true);

	m_Range = 0.f;
}

bool DrawGun::Perform(Elite::Blackboard* pBlackboard, float dt)
{
	std::cout << " Drawing my Gun \n";
	return true;
}
// ------------------------------------------------------------------//

DrawBomb::DrawBomb()
{
	AddPrecondition("BombInInventory", true);
	AddPrecondition("BombInHand", false);
	AddPrecondition("WeaponInHand", false);
	AddEffect("WeaponInHand", true);
	AddEffect("BombInHand", true);

	m_Range = 0.f;
}

bool DrawBomb::Perform(Elite::Blackboard* pBlackboard, float dt)
{
	std::cout << "Drawing my bomb \n";
	return true;
}
// ------------------------------------------------------------------//

SheatKnife::SheatKnife()
{
	AddPrecondition("KnifeInHand", true);
	AddEffect("KnifeInHand", false);
	AddEffect("WeaponInHand", false);

	m_Range = 0.f;
}

bool SheatKnife::Perform(Elite::Blackboard* pBlackboard, float dt)
{
	std::cout << "Put away my Knife \n";

	return true;
}
// ------------------------------------------------------------------//

SheatGun::SheatGun()
{
	AddPrecondition("GunInHand", true);
	AddEffect("GunInHand", false);
	AddEffect("WeaponInHand", false);

	m_Range = 0.f;
}

bool SheatGun::Perform(Elite::Blackboard* pBlackboard, float dt)
{
	std::cout << "Put away my Gun \n";
	return true;
}

SheatBomb::SheatBomb()
{
	AddPrecondition("BombInHand", true);
	AddEffect("BombInHand", false);
	AddEffect("WeaponInHand", false);

	m_Range = 0.f;
}

bool SheatBomb::Perform(Elite::Blackboard* pBlackboard, float dt)
{
	std::cout << "Put away my Bomb \n";
	return true;
}

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

bool Stabbing::Perform(Elite::Blackboard* pBlackboard)
{
	std::cout << " Stabbing Enemy \n";
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

bool Shooting::Perform(Elite::Blackboard* pBlackboard)
{
	std::cout << " Shooting Enemy \n";
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

bool SelfDestruct::Perform(Elite::Blackboard* pBlackboard)
{
	std::cout << " SelfDestructing taking enemy with me \n";
	return true;
}
// ------------------------------------------------------------------//

PickUpKnife::PickUpKnife()
{
	AddPrecondition("KnifeAvailable", true);
	AddPrecondition("KnifeInInventory", false);
	AddEffect("KnifeInInventory", true);
	m_Range = 1.f;
}

bool PickUpKnife::Perform(Elite::Blackboard* pBlackboard)
{
	std::cout << "Picked Up Knife \n";
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

bool PickUpGun::Perform(Elite::Blackboard* pBlackboard)
{
	std::cout << " Picked up Gun \n";
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

bool PickUpBomb::Perform(Elite::Blackboard* pBlackboard)
{
	std::cout << " Picked Up Bomb \n";
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

bool DrawKnife::Perform(Elite::Blackboard* pBlackboard)
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

bool DrawGun::Perform(Elite::Blackboard* pBlackboard)
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

bool DrawBomb::Perform(Elite::Blackboard* pBlackboard)
{
	std::cout << "Drawing my bomb \n";
	return true;
}
// ------------------------------------------------------------------//

SheatKnife::SheatKnife()
{
	AddPrecondition("KnifeInHand", true);
	AddEffect("KnifeInHand", false);

	m_Range = 0.f;
}

bool SheatKnife::Perform(Elite::Blackboard* pBlackboard)
{
	std::cout << "Put away my Knife \n";

	return true;
}
// ------------------------------------------------------------------//

SheatGun::SheatGun()
{
	AddPrecondition("GunInHand", true);
	AddEffect("GunInHand", false);

	m_Range = 0.f;
}

bool SheatGun::Perform(Elite::Blackboard* pBlackboard)
{
	std::cout << "Put away my Gun \n";
	return true;
}

#include "stdafx.h"
#include "Weapon.h"

Weapon::Weapon(Elite::Vector2 pos , float radius):
	m_Position{pos},
	m_Radius{radius}
{
}
Weapon::~Weapon()
{
}

void Weapon::Update(float dt)
{
	
}

void Weapon::Render(float dt)
{
	DEBUGRENDERER2D->DrawSolidCircle(m_Position, m_Radius, { 0,0 },Elite::Color{ 1,0,0,1 });
}


void Weapon::MarkForPickedUp()
{
	m_PickedUp = true;
}

bool Weapon::IsPickedUp()
{
	return m_PickedUp;
}

bool Weapon::IsOverlapping(SteeringAgent* agent)
{
    float distance = Elite::Distance(m_Position, agent->GetPosition());

	if (distance < m_Radius + agent->GetRadius())
	{
		return true;
	}
    return false;
}

#include "stdafx.h"
#include "Knife.h"
Knife::Knife(Elite::Vector2 pos, float radius) :
	Weapon(pos, radius)
{

}
Knife::~Knife()
{
}

void Knife::Update(float dt)
{
	
}

void Knife::Render(float dt)
{
	DEBUGRENDERER2D->DrawSolidCircle(m_Position, m_Radius, { 0,0 }, Elite::Color{ 1,1,0,1 });
}

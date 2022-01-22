#include "stdafx.h"
#include "Gun.h"

Gun::Gun(Elite::Vector2 pos, float radius):
	Weapon(pos,radius)
{
}

Gun::~Gun()
{
}

void Gun::Update(float dt)
{
}

void Gun::Render(float dt)
{
	DEBUGRENDERER2D->DrawSolidCircle(m_Position, m_Radius, { 0,0 }, Elite::Color{ 1,0,1,1 });

}

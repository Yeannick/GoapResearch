#include "stdafx.h"
#include "Bomb.h"

Bomb::Bomb(Elite::Vector2 pos, float radius) :
	Weapon(pos, radius)
{

}
Bomb::~Bomb()
{

}
void Bomb::Update(float dt)
{

}
void Bomb::Render(float dt)
{
	DEBUGRENDERER2D->DrawSolidCircle(m_Position, m_Radius, { 0,0 }, Elite::Color{0,1,1,1});
}
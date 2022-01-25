#pragma once
#include "Weapon.h"
class Bomb : public Weapon
{
public:
	Bomb(Elite::Vector2 pos, float radius);
	~Bomb();
	std::string GetName()override { return m_Name; }

	void Update(float dt) override;
	void Render(float dt) override;

private:
	std::string m_Name = "Bomb";
};


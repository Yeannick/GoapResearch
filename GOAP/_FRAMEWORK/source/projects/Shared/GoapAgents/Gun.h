#pragma once
#include "Weapon.h"
class Gun : public Weapon
{
public:
	Gun(Elite::Vector2 pos, float radius);
	~Gun();
	std::string GetName()override { return m_Name; }
	void Update(float dt) override;
	void Render(float dt) override;


private:
	std::string m_Name = "Gun";
};


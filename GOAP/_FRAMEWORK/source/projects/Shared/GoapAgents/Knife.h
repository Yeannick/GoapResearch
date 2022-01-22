#pragma once
#include "Weapon.h"
class Knife : public Weapon
{
public:
	Knife(Elite::Vector2 pos, float radius);
	~Knife();

	void Update(float dt) override;
	void Render(float dt) override;
	std::string GetName()override { return m_Name; }
	
private:
	std::string m_Name = "Knife";
	
};


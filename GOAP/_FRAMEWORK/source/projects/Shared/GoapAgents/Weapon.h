#pragma once
#include "projects/Movement/SteeringBehaviors/SteeringAgent.h"
class Weapon
{

public:
	Weapon(Elite::Vector2 pos, float radius);
	~Weapon();

	virtual void Update(float dt) ;
	virtual void Render(float dt) ;

	virtual std::string GetName() { return m_Name; }


	void SetPosition(Elite::Vector2 pos) { m_Position = pos; }
	Elite::Vector2 GetPosition() { return m_Position; }
	void MarkForPickedUp();
	bool IsPickedUp();
	bool IsOverlapping(SteeringAgent* agent);
protected:
	std::string m_Name;
	Elite::Vector2 m_Position;
	float m_Radius;

	bool m_PickedUp = false;

};


#pragma once
#include "Entity.h"
class Projectile : public Entity
{
public:
	Projectile(bool isServerSide);
	~Projectile();
	void Update(float dt) override;
	void Init(XMFLOAT3 pos, XMFLOAT3 dir);
	void OnCollision(Entity* other) override;

private:
	float m_speed;
	cpu_mesh m_mesh;
	cpu_material m_material;

	float m_distMax = 100.0f;
	float m_distTraveled = 0.0f;


};


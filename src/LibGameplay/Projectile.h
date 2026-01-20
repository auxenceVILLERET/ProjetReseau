#pragma once
#include "Entity.h"
class Projectile : public Entity
{
	public:
	Projectile();
	~Projectile();
	void Update(float dt) override;
	void Init(cpu_transform& transform);
	void OnCollision(Entity* other) override;

	EntityType GetType() const override { return EntityType::Bullet; }
private:
	float m_speed;
	cpu_mesh m_mesh;
	cpu_material m_material;

	float m_distMax = 50.0f;
	float m_distTraveled = 0.0f;


};


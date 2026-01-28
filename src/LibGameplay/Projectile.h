#pragma once
#include "Entity.h"

class Player;

class Projectile : public Entity
{
public:
	Projectile(bool isServerSide);
	~Projectile();
	void Update(float dt) override;
	void Init(XMFLOAT3 pos, XMFLOAT3 dir);
	void OnCollision(Entity* other) override;

	void SetShooter(Player* pPlayer) { m_pShooter = pPlayer; }
	
private:
	float m_speed;
	cpu_mesh m_mesh;
	cpu_material m_material;

	Player* m_pShooter;
	
	float m_distMax;
	float m_distTraveled;

	friend class Player;
};


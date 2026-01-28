#pragma once
#include "Entity.h"

enum class PowerUpType
{
	HEALTH,
	SPEED,
	SHIELD

};

class PowerUp : public Entity
{
public:
	PowerUp(bool isServerSide);
	void Init(PowerUpType type);
	virtual ~PowerUp();
	virtual void OnCollision(Entity* other) override;
	PowerUpType GetPowerUpType() const { return m_powerUpType; }

private:
	PowerUpType m_powerUpType;
	cpu_mesh m_mesh;
	cpu_material m_material;

};


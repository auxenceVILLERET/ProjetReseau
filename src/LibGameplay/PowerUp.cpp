#include "pch.h"
#include "PowerUp.h"
#include "Player.h"


PowerUp::PowerUp(bool isServerSide) : Entity(isServerSide)
{
	if(isServerSide == false)
	{
		m_mesh.CreateCube(0.5f);
		m_material.color = cpu::ToColor(0, 0, 0);
		m_pCpuEntity->pMesh = &m_mesh;
		m_pCpuEntity->pMaterial = &m_material;
	}

	m_powerUpType = PowerUpType::HEALTH;
	m_collider.radius = 0.7f;
	m_type = EntityType::POWERUP;

}

void PowerUp::Init(PowerUpType type)
{
	m_powerUpType = type;

	if(m_powerUpType == PowerUpType::HEALTH)
	{
		m_material.color = cpu::ToColor(255, 0, 0);
	}
	else if(m_powerUpType == PowerUpType::SPEED)
	{
		m_material.color = cpu::ToColor(0, 255, 0);
	}
	else if(m_powerUpType == PowerUpType::SHIELD)
	{
		m_material.color = cpu::ToColor(0, 0, 255);
	}
}

PowerUp::~PowerUp()
{

}

void PowerUp::OnCollision(Entity* other)
{
	if (m_toDestroy == true) return;

	Player* player = dynamic_cast<Player*>(other);

	if (player)
	{
		switch (m_powerUpType)
		{
		case PowerUpType::HEALTH:
			player->Heal(50.0f); 
			break;
		case PowerUpType::SPEED:
			player->ActivateSpeedBoost(); 
			break;
		case PowerUpType::SHIELD:
			//player->ActivateShield(10.0f); 
			break;
		default:
			break;
		}
		this->Destroy();
	}
}

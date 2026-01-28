#include "pch.h"
#ifndef PLAYER_CPP_DEFINED
#define PLAYER_CPP_DEFINED
#include "Player.h"
#include "Projectile.h"
#include "GameManager.h"

std::vector<XMFLOAT3> Player::m_shipColor =
{
	{1, 1, 1},    // blanc
	{1, 0, 0},    // rouge
	{0, 1, 0},    // vert
	{0, 0, 1},    // bleu
	{1, 1, 0},    // jaune
	{1, 0, 1},    // magenta

};

std::vector<XMFLOAT3> Player::m_particleColor =
{
	{1, 1, 1},			// blanc
	{1, 0.5f, 0.5f},    // rouge
	{0.5f, 1, 0.5f},    // vert
	{0.5f, 0.5f, 1},    // bleu
	{1, 1, 0.5f},		// jaune
	{1, 0.5f, 1},		// magenta
};

Player::Player(bool isServerSide) : Entity(isServerSide)
{
	m_health = 100.0f;
	m_maxHealth = 100.0f;

	if (!m_isServerSide)
	{
		m_mesh.CreateSpaceship();
		m_material.color = cpu::ToColor(255, 128, 0);
		m_pCpuEntity->pMesh = &m_mesh;
		m_pCpuEntity->pMaterial = &m_material;
	}

	m_pEmitter = nullptr;
	
	m_speedRotation = 1.3f;
	m_speedMovement = 3.0f;
	m_maxSpeed = 10.0f;

	m_shootCooldown = 0.1f;
	m_shootTimer = 0.0f;

	m_collider.radius = 1.0f;
	m_type = EntityType::PLAYER;

	m_speedBoostActive = false;
	m_speedBoostDuration = 5.0f;
	m_speedBoostTimer = 0.0f;
	m_killCount = 0;
	m_deathCount = 0;
	m_score = 0;

	m_shipColorIndex = 0;
	m_particleColorIndex = 0;
}

Player::~Player()
{
	
}

void Player::Update(float dt)
{
	XMFLOAT3 dir = GetTransform().dir;
	Move(dt * m_speedMovement * dir.x, dt * m_speedMovement * dir.y, dt * m_speedMovement * dir.z);
	m_shootTimer += dt;
	SpeedBoost(dt, 10.0f);
}

void Player::UpdateCamera()
{
	XMFLOAT3 pos = GetTransform().pos;
	XMFLOAT3 camPos = pos;

	float speedMult = (m_speedMovement) / 10.0f + 1.0f; 
	
	camPos.x -= GetTransform().dir.x * 6.0f * speedMult - GetTransform().up.x * 2.0f;
	camPos.y -= GetTransform().dir.y * 6.0f * speedMult - GetTransform().up.y * 2.0f;
	camPos.z -= GetTransform().dir.z * 6.0f * speedMult - GetTransform().up.z * 2.0f;
	
	cpuEngine.GetCamera()->transform.pos = camPos;

	cpuEngine.GetCamera()->transform.SetRotation(GetTransform());
	cpuEngine.GetCamera()->transform.AddYPR(0.0f, 0.1f / speedMult, 0.0f);
}

bool Player::Shoot()
{
	if (m_shootTimer >= m_shootCooldown)
	{
		m_shootTimer = 0.0f;
		return true;
	}
	return false;
}

void Player::InitRenderElements()
{
	m_pEmitter = cpuEngine.CreateParticleEmitter();
	m_pEmitter->density = 1500.0f;
	m_pEmitter->colorMin = cpu::ToColor(156, 0, 250);
	m_pEmitter->colorMax = cpu::ToColor(213, 125, 255);
}

void Player::UpdateRenderElements(float dt)
{
	m_shootTimer += dt;
	if (m_pEmitter == nullptr) return;
	
	m_pEmitter->density = m_speedMovement * 100.0f;
	m_pEmitter->pos = GetTransform().pos;
	m_pEmitter->dir.x = -GetTransform().dir.x;
	m_pEmitter->dir.y = -GetTransform().dir.y;
	m_pEmitter->dir.z = -GetTransform().dir.z;
}

void Player::Render()
{
	
}

void Player::OnCollision(Entity* other)
{
	if (m_isAlive == false) return;
	
	if(other->GetType() == EntityType::ASTEROID)
	{
		TakeDamage(100.0f);
	}
	if(other->GetType() == EntityType::PROJECTILE)
	{
		TakeDamage(10.0f);
		Projectile* projectile = dynamic_cast<Projectile*>(other);
		if (projectile == nullptr) return;

		if (IsAlive() == false)
		{
			Player* shooter = projectile->m_pShooter;
			if (shooter == nullptr) return;

			shooter->m_killCount++;
			shooter->m_score += 50;
			shooter->SetDirtyFlag(OTHER);
		}
	}

	if (m_isAlive == true) return;
	
	m_deathCount++;
	m_score -= 20;
	SetDirtyFlag(OTHER);
}

bool Player::ActivateSpeedBoost()
{
	if(m_speedBoostActive == false)
	{
		m_speedBoostActive = true;
		return true;
	}
	return false;
}

void Player::SpeedBoost(float dt, float speedBoost)
{
	if(m_speedBoostActive == true)
	{
		m_speedBoostTimer += dt;
		m_speedMovement = m_maxSpeed + speedBoost;
		if(m_speedBoostTimer >= m_speedBoostDuration)
		{
			m_speedBoostActive = false;
			m_speedBoostTimer = 0.0f;
			m_speedMovement = m_maxSpeed;
		}
	}
}

void Player::SetActive()
{
	m_isActive = true;
	if (m_isServerSide == false)
	{
		m_pCpuEntity->visible = true;
	}
}

void Player::SetInactive()
{
	m_isActive = false;
	m_speedMovement = 1.0f;

	if (m_isServerSide == false)
	{
		m_pCpuEntity->visible = false;
		m_pEmitter->density = 0.0f;
	}
}

void Player::ChangeColorShip(int index)
{
	int size = static_cast<int>(m_shipColor.size());
	m_shipColorIndex = (m_shipColorIndex + index + size) % size;

	if (m_isServerSide == false)
	{
		XMFLOAT3 shipCol = m_shipColor[m_shipColorIndex];
		m_material.color = cpu::ToColor((shipCol.x * 255), (shipCol.y * 255), (shipCol.z * 255));
		SetDirtyFlag(DIRTY_TYPES::SHIP_COLOR);
	}
}

void Player::ChangeColorParticle(int index)
{
	int size = static_cast<int>(m_particleColor.size());
	m_particleColorIndex = (m_particleColorIndex + index + size) % size;

	if (m_isServerSide == false)
	{
		XMFLOAT3 partCol = m_particleColor[m_particleColorIndex];
		m_pEmitter->colorMin = cpu::ToColor((partCol.x * 255) / 2, (partCol.y * 255) / 2, (partCol.z * 255) / 2);
		m_pEmitter->colorMax = cpu::ToColor((partCol.x * 255), (partCol.y * 255), (partCol.z * 255));
		SetDirtyFlag(DIRTY_TYPES::PARTICLE_COLOR);
	}
}


#endif

#include "pch.h"
#ifndef PLAYER_CPP_DEFINED
#define PLAYER_CPP_DEFINED
#include "Player.h"
#include "Projectile.h"
#include "GameManager.h"

Player::Player(bool isServerSide) : Entity(isServerSide)
{
	m_health = 100.0f;

	if (!m_isServerSide)
	{
		m_mesh.CreateSpaceship();
		m_material.color = cpu::ToColor(255, 128, 0);
		m_pCpuEntity->pMesh = &m_mesh;
		m_pCpuEntity->pMaterial = &m_material;
	}
	
	m_speedRotation = 0.8f;
	m_speedMovement = 0.0f;
	m_maxSpeed = 4.0f;

	m_shootCooldown = 0.2f;
	m_shootTimer = 0.0f;

	m_type = EntityType::PLAYER;
}

Player::~Player()
{
	
}

void Player::Update(float dt)
{
	GetTransform().Move(dt * m_speedMovement);
}

void Player::Accelerate(float dt)
{
	m_speedMovement = std::min(m_speedMovement + dt * 2.0f, 4.0f);
}

void Player::Brake(float dt)
{
	m_speedMovement = std::max(m_speedMovement - dt * 8.0f, 1.0f);
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

void Player::Rotate(float x, float y,float z, float dt)
{
	GetTransform().AddYPR(x * dt * m_speedRotation, y * dt * m_speedRotation, z * dt * m_speedRotation * 1.5f);

}

void Player::Shoot()
{
	m_shootTimer += cpuTime.delta;
	if (m_shootTimer >= m_shootCooldown)
	{
		Projectile* pProjectile = GameManager::GetInstance()->CreateEntity<Projectile>();
		pProjectile->Init(GetTransform());
		m_shootTimer = 0.0f;
	}
}
#endif

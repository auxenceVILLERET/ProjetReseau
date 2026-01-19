#include "pch.h"
#ifndef PLAYER_CPP_DEFINED
#define PLAYER_CPP_DEFINED
#include "Player.h"
#include "Projectile.h"
#include "GameManager.h"

Player::Player()
{
	m_health = 100.0f;
	
	m_mesh.CreateSpaceship();
	m_material.color = cpu::ToColor(255, 128, 0);
	m_pCpuEntity->pMesh = &m_mesh;
	m_pCpuEntity->pMaterial = &m_material;
	
	m_speedRotation = 0.8f;
	m_speedMovement = 0.0f;
	m_maxSpeed = 10.0f;

	m_shootCooldown = 0.2f;
	m_shootTimer = 0.0f;

	cpuEngine.GetParticleData()->Create(1000000);
	m_pEmitter = cpuEngine.CreateParticleEmitter();
	m_pEmitter->density = 3000.0f;
	m_pEmitter->colorMin = cpu::ToColor(0, 0, 255);
	m_pEmitter->colorMax = cpu::ToColor(255, 128, 0);
}

Player::~Player()
{
	
}

void Player::Update(float dt)
{
	m_pCpuEntity->transform.Move(dt * m_speedMovement);
	

}

void Player::Accelerate(float dt)
{
	m_speedMovement = std::min(m_speedMovement + dt * 3.0f, m_maxSpeed);

}

void Player::Brake(float dt)
{
	m_speedMovement = std::max(m_speedMovement - dt * 8.0f, 1.0f);
}

void Player::UpdateCamera()
{
	XMFLOAT3 pos = m_pCpuEntity->transform.pos;
	XMFLOAT3 camPos = pos;

	float speedMult = (m_speedMovement) / 10.0f + 1.0f; 
	
	camPos.x -= m_pCpuEntity->transform.dir.x * 6.0f * speedMult - m_pCpuEntity->transform.up.x * 2.0f;
	camPos.y -= m_pCpuEntity->transform.dir.y * 6.0f * speedMult - m_pCpuEntity->transform.up.y * 2.0f;
	camPos.z -= m_pCpuEntity->transform.dir.z * 6.0f * speedMult - m_pCpuEntity->transform.up.z * 2.0f;
	
	cpuEngine.GetCamera()->transform.pos = camPos;

	cpuEngine.GetCamera()->transform.SetRotation(m_pCpuEntity->transform);
	cpuEngine.GetCamera()->transform.AddYPR(0.0f, 0.1f / speedMult, 0.0f);
}

void Player::Rotate(float x, float y,float z, float dt)
{
	m_pCpuEntity->transform.AddYPR(x * dt * m_speedRotation, y * dt * m_speedRotation, z * dt * m_speedRotation * 1.5f);

}

void Player::Shoot()
{
	m_shootTimer += cpuTime.delta;
	if (m_shootTimer >= m_shootCooldown)
	{
		Projectile* pProjectile = GameManager::GetInstance()->CreateEntity<Projectile>();
		pProjectile->Init(m_pCpuEntity->transform);
		m_shootTimer = 0.0f;
	}
}
void Player::Render()
{
	m_pEmitter->density = m_speedMovement * 500.0f;
	m_pEmitter->pos = m_pCpuEntity->transform.pos;
	m_pEmitter->dir = m_pCpuEntity->transform.dir;
	m_pEmitter->dir.x = -m_pEmitter->dir.x;
	m_pEmitter->dir.y = -m_pEmitter->dir.y;
	m_pEmitter->dir.z = -m_pEmitter->dir.z;
}


#endif

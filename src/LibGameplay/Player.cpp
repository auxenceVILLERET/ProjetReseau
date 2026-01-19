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
	m_speedMovement = 4.0f;
	m_shootCooldown = 0.2f;
	m_shootTimer = 0.0f;
}

Player::~Player()
{
	

}

void Player::Update(float dt)
{

}


void Player::Move(float dist, float dt)
{
	m_pCpuEntity->transform.Move(dist * dt * m_speedMovement);
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
#endif

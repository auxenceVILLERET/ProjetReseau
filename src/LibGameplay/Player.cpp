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
	m_speedMovement = 1.0f;
	m_maxSpeed = 10.0f;

	m_shootCooldown = 0.1f;
	m_shootTimer = 0.0f;

	m_collider.radius = 1.0f;
	m_type = EntityType::PLAYER;
}

Player::~Player()
{
	
}

void Player::Update(float dt)
{
	XMFLOAT3 dir = GetTransform().dir;
	Move(dt * m_speedMovement * dir.x, dt * m_speedMovement * dir.y, dt * m_speedMovement * dir.z);
	m_shootTimer += dt;
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
	m_pEmitter->density = 3000.0f;
	m_pEmitter->colorMin = cpu::ToColor(156, 0, 250);
	m_pEmitter->colorMax = cpu::ToColor(213, 125, 255);
}

void Player::UpdateRenderElements(float dt)
{
	m_shootTimer += dt;
	if (m_pEmitter == nullptr) return;
	
	m_pEmitter->density = m_speedMovement * 200.0f;
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
	if(other->GetType() == EntityType::ASTEROID)
	{
		m_health -= 100.0f;
		if (m_health <= 0.0f)
		{
			Destroy();
		}
	}
}



#endif

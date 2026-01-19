#include "pch.h"
#ifndef PLAYER_CPP_DEFINED
#define PLAYER_CPP_DEFINED
#include "Player.h"

Player::Player()
{
	m_health = 100.0f;
	
	m_mesh.CreateSpaceship();
	m_material.color = cpu::ToColor(255, 128, 0);
	m_pCpuEntity->pMesh = &m_mesh;
	m_pCpuEntity->pMaterial = &m_material;
	m_speedRotation = 0.8f;
	m_speedMovement = 1.0f;
	m_maxSpeed = 4.0f;

	m_isMoving = false;
}

Player::~Player()
{

}

void Player::Update(float dt)
{
	if (!m_isMoving)
	{
		m_speedMovement = std::max(m_speedMovement - dt * 4.0f, 1.0f);
	}

	m_isMoving = false;
}

void Player::Move(float dist, float dt)
{
	m_speedMovement += dt;
	m_speedMovement = std::min(m_speedMovement + dt, 4.0f);
	m_isMoving = true;
	
	m_pCpuEntity->transform.Move(dist * dt * m_speedMovement);
}

void Player::UpdateCamera()
{
	XMFLOAT3 pos = m_pCpuEntity->transform.pos;
	XMFLOAT3 camPos = pos;

	float speedMult = (m_speedMovement - 1.0f) / 10.0f + 1.0f; 
	
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

#endif

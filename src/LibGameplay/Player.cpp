#include "pch.h"
#ifndef PLAYER_CPP_DEFINED
#define PLAYER_CPP_DEFINED
#include "Player.h"

Player::Player()
{
	m_health = 100.0f;
	m_isCameraBinded = false;
	
	m_mesh.CreateSpaceship();
	m_material.color = cpu::ToColor(255, 128, 0);
	m_pCpuEntity->pMesh = &m_mesh;
	m_pCpuEntity->pMaterial = &m_material;
}

Player::~Player()
{

}

void Player::Update(float dt)
{
	if (m_isCameraBinded)
	{
		XMFLOAT3 pos = m_pCpuEntity->transform.pos;
		XMFLOAT3 camPos = pos;
		camPos.x -= m_pCpuEntity->transform.dir.x * 8.0f;
		camPos.y -= m_pCpuEntity->transform.dir.y * 8.0f;
		camPos.z -= m_pCpuEntity->transform.dir.z * 8.0f;

		cpuEngine.GetCamera()->transform.pos = camPos;
		cpuEngine.GetCamera()->transform.LookAt(pos.x, pos.y, pos.z);
	}
}

void Player::Move(float x, float y, float z, float dt)
{
		
}

void Player::BindCamera(bool state)
{
	m_isCameraBinded = state;
	
}

#endif

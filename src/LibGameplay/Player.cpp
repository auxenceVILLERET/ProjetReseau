#include "pch.h"
#ifndef PLAYER_CPP_DEFINED
#define PLAYER_CPP_DEFINED
#include "Player.h"

Player::Player()
{
	Entity::Entity();
	m_health = 100.0f;
	m_mesh.CreateSpaceship();
	m_pCpuEntity->pMesh = &m_mesh;
}

Player::~Player()
{
	

}

void Player::Update(float dt)
{

}

void Player::Move(float x, float y, float z, float dt)
{
	
}

#endif

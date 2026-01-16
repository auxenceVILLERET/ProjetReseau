#include "pch.h"
#include "Player.h"

Player::Player()
{
	Entity::Entity();
	m_textPlayer.Load("../../res/LibGameplay/PlayerBlue1.png");

	m_pSprite = cpuEngine.CreateSprite();
	m_pSprite->pTexture = &m_textPlayer;
	m_pSprite->CenterAnchor();
	m_pSprite->x = 1920/2;
	m_pSprite->y = 1080/2;
}

Player::~Player()
{
	m_textPlayer.Close();
	cpuEngine.Release(m_pSprite);

}

void Player::Update(float dt)
{

}

void Player::Move(float x, float y, float dt)
{
	m_pSprite->x += x * dt;
	m_pSprite->y += y * dt;

}

#pragma once
#include "Entity.h"

class Player : public Entity
{
public:
	Player();
	~Player();
	void Update(float dt);
	void Move(float x, float y, float dt);

private:
	cpu_sprite* m_pSprite = nullptr;
	cpu_texture m_textPlayer;
};


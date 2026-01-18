#ifndef PLAYER_H_DEFINED
#define PLAYER_H_DEFINED
#include "Entity.h"

class Player : public Entity
{
public:
	Player();
	~Player();
	void Update(float dt);
	void Move(float x, float y, float z, float dt);

private:
	float m_health;

	cpu_mesh m_mesh;
	cpu_material m_material;
};

#endif


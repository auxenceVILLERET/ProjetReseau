#ifndef PLAYER_H_DEFINED
#define PLAYER_H_DEFINED
#include "Entity.h"

class Player : public Entity
{
public:
	Player();
	~Player();
	void Update(float dt);
	void Move(float dist, float dt);

	void Rotate(float x, float y,float z, float dt);

	void BindCamera(bool state = true);

private:
	float m_health;
	float m_speedRotation;
	float m_speedMovement;

	cpu_mesh m_mesh;
	cpu_material m_material;

	bool m_isCameraBinded;
};

#endif


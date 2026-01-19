#ifndef PLAYER_H_DEFINED
#define PLAYER_H_DEFINED
#include "Entity.h"

class Player : public Entity
{
public:
	Player();
	~Player();
	void Update(float dt);
	void Move(float dt);
	void Brake(float dt);

	void Rotate(float x, float y,float z, float dt);
	
	void UpdateCamera();

private:
	float m_health;
	float m_speedRotation;
	float m_speedMovement;
	float m_maxSpeed;
	
	cpu_mesh m_mesh;
	cpu_material m_material;
};

#endif


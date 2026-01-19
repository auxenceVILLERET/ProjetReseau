#ifndef PLAYER_H_DEFINED
#define PLAYER_H_DEFINED
#include "Entity.h"

class Player : public Entity
{
public:
	Player();
	~Player();
	
	void Update(float dt) override;
	void Accelerate(float dt);
	void Brake(float dt);
	
	void Rotate(float x, float y,float z, float dt);
	
	void UpdateCamera();

	void Shoot();

	void Render() override;

private:
	float m_health;

	float m_speedRotation;
	float m_speedMovement;
	float m_maxSpeed;

	float m_shootCooldown;
	float m_shootTimer;

	cpu_mesh m_mesh;
	cpu_material m_material;
	cpu_particle_emitter* m_pEmitter;

};

#endif


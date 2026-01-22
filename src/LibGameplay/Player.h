#ifndef PLAYER_H_DEFINED
#define PLAYER_H_DEFINED
#include "Entity.h"

class Player : public Entity
{
public:
	Player(bool isServerSide);
	~Player();
	
	void Update(float dt) override;
	void Accelerate(float dt);
	void Brake(float dt);
	
	void Rotate(float x, float y,float z, float dt);
	
	void UpdateCamera();

	void Shoot();

	void InitRenderElements() override;
	void UpdateRenderElements(float dt) override;
	void Render() override;
	void OnCollision(Entity* other) override;

	float GetHealth() const { return m_health; }
	void TakeDamage(float damage) { m_health -= damage; }

	float GetSpeed()			{ return m_speedMovement; }
	float GetRotationSpeed()	{ return m_speedRotation; }

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


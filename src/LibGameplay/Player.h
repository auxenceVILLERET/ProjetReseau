#ifndef PLAYER_H_DEFINED
#define PLAYER_H_DEFINED
#include "Entity.h"

class Player : public Entity
{
public:
	Player(bool isServerSide);
	~Player();
	
	void Update(float dt) override;
	
	void UpdateCamera();

	void Shoot();

	void InitRenderElements()			override;
	void UpdateRenderElements(float dt) override;
	void Render()						override;
	void OnCollision(Entity* other)		override;

	float GetHealth() const			{ return m_health; }
	void TakeDamage(float damage)	{ m_health -= damage; }

	void SetSpeed(float speed)	{ m_speedMovement = std::clamp(speed, 1.0f, m_maxSpeed); }
	void AddSpeed(float delta)  { SetSpeed(m_speedMovement + delta); }
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


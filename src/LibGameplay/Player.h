#ifndef PLAYER_H_DEFINED
#define PLAYER_H_DEFINED
#include "Entity.h"

class Player : public Entity
{
public:
	Player(bool isServerSide);
	~Player();
	
	void Update(float dt)				override;
	
	void UpdateCamera();

	bool Shoot();

	void InitRenderElements()			override;
	void UpdateRenderElements(float dt) override;
	void Render()						override;
	void OnCollision(Entity* other)		override;
	void TakeDamage(float damage)		override;

	void SetSpeed(float speed)			{ m_speedMovement = std::clamp(speed, 1.0f, m_maxSpeed); }
	void AddSpeed(float delta)			{ SetSpeed(m_speedMovement + delta); }
	float GetSpeed()					{ return m_speedMovement; }
	float GetRotationSpeed()			{ return m_speedRotation; }

	bool ActivateSpeedBoost();
	void SpeedBoost(float dt, float speedBoost);
	bool ActivateShield();

	void SetActive()					override;
	void SetInactive()					override;
	
	void ChangeColorShip(int index);
	void ChangeColorParticle(int index);

	void SetUsername(std::string const& username)				{ m_name = username; }
	void SetStats(int killCount, int deathCount, int score)		{ m_killCount = killCount; m_deathCount = deathCount; m_score = score; }
	int GetKillCount()											{ return m_killCount; }
	int GetDeathCount()											{ return m_deathCount; }
	int GetScore()												{ return m_score; }
	std::string& GetName()										{ return m_name; }
	std::string& GetKillName()									{ return m_killName; }
	
	static std::vector<XMFLOAT3> m_shipColor;
	static std::vector<XMFLOAT3> m_particleColor;

private:

	float m_speedRotation;
	float m_speedMovement;
	float m_maxSpeed;

	float m_shootCooldown;
	float m_shootTimer;

	int m_killCount;
	int m_deathCount;
	int m_score;
	std::string m_name;
	std::string m_killName;
	
	cpu_mesh m_mesh;
	cpu_material m_material;
	cpu_particle_emitter* m_pEmitter;

	uint32_t m_shipColorIndex;
	uint32_t m_particleColorIndex;

	bool m_speedBoostActive;
	float m_speedBoostTimer;
	float m_speedBoostDuration;

	bool m_shieldActive;
	float m_shieldAmount;

};

#endif


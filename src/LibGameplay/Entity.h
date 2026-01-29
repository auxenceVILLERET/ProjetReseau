#ifndef ENTITY_H_DEFINED
#define ENTITY_H_DEFINED
#include "pch.h"

#include "Gameplay.h"


struct SphereCollider
{
	XMFLOAT3 center = { 0, 0, 0 };
	float radius = 1.0f;
};

enum DIRTY_TYPES
{
	POS				= 0b100000,
	ROTATION		= 0b010000,
	SCALE			= 0b001000,
	HEALTH			= 0b000100,
	SHIP_COLOR		= 0b000100,
	PARTICLE_COLOR	= 0b000010,
	OTHER			= 0b000001
};

class Entity
{
public:
	Entity(bool isServerSide);
	virtual ~Entity();
	void SetID(uint32_t id) { m_id = id; }

	virtual void Update(float dt);
	virtual void UpdateCollider();

	virtual void InitRenderElements() {}
	virtual void UpdateRenderElements(float dt) {}
	virtual void Render();

	virtual void SetActive() {}
	virtual void SetInactive() {}

	virtual void Destroy();
	bool GetToDestroy() { return m_toDestroy; };
	
	uint32_t GetID() const { return m_id; }
	cpu_transform& GetTransform();
	EntityType GetType() const { return m_type; }

	virtual void OnCollision(Entity* other) {}

	void SetDirtyFlag(DIRTY_TYPES type) { m_dirtyFlags |= type; }
	int GetDirtyFlags() const { return m_dirtyFlags; }
	void ClearDirtyFlags() { m_dirtyFlags = 0; }
	
	void Move(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void Scale(float scale);

	void SetPos(float x, float y, float z);
	XMFLOAT3 GetPos() { return GetTransform().pos; }

	void SetRotation(XMFLOAT4X4& rot);
	void SetScale(float scale);

	float GetScale();

	bool IsAlive() { return m_isAlive; }
	void SetAlive(bool newState) { m_isAlive = newState; }

	virtual void SetHealth(float health);
	void SetMaxHealth(float maxHealth) { m_maxHealth = maxHealth; }
	float GetHealth() { return m_health; }
	float GetMaxHealth() { return m_maxHealth; }
	virtual void TakeDamage(float damage);
	void Heal(float amount);
	void FullHeal();

	bool GetActiveState() { return m_isActive; }
protected:
	cpu_entity* m_pCpuEntity;
	SphereCollider m_collider;

	EntityType m_type;
	bool m_toDestroy;
	bool m_isServerSide;
	
	int m_dirtyFlags;

	bool m_isAlive;
	float m_health;
	float m_maxHealth;
	bool m_isActive;

private:
	static uint32_t ID_COUNT;
	uint32_t m_id;
	cpu_transform m_cpuTransform;
	
	friend class GameManager;
};


#endif


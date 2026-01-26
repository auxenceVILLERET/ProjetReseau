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
	POS			= 0b1000,
	ROTATION	= 0b0100,
	SCALE		= 0b0010,
	HEALTH		= 0b0001
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

	void SetHealth(float health);
	void SetMaxHealth(float maxHealth) { m_maxHealth = maxHealth; }
	float GetHealth() { return m_health; }
	void TakeDamage(float damage);
	void Heal(float amount);

	bool GetActiveState() { return m_isActive; }
protected:
	cpu_entity* m_pCpuEntity;
	SphereCollider m_collider;

	EntityType m_type;
	bool m_toDestroy = false;
	bool m_isServerSide = false;
	
	int m_dirtyFlags = 0;

	bool m_isAlive = true;
	float m_health = 100.0f;
	float m_maxHealth = 100.0f;
	bool m_isActive = true;

private:
	static uint32_t ID_COUNT;
	uint32_t m_id;
	cpu_transform m_cpuTransform;
	
	friend class GameManager;
};


#endif


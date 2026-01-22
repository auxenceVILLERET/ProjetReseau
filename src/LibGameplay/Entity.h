#ifndef ENTITY_H_DEFINED
#define ENTITY_H_DEFINED
#include "pch.h"

#include "Gameplay.h"


struct SphereCollider
{
	XMFLOAT3 center;
	float radius;
};

enum DIRTY_TYPES
{
	POS			= 0b100,
	ROTATION	= 0b010,
	SCALE		= 0b001
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

	void Destroy();
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
	void SetRotation(XMFLOAT4X4& rot);
	void SetScale(float scale);

protected:
	cpu_entity* m_pCpuEntity;
	SphereCollider m_collider;

	EntityType m_type;
	bool m_toDestroy = false;
	bool m_isServerSide = false;
	
	int m_dirtyFlags = 0;
private:
	static uint32_t ID_COUNT;
	uint32_t m_id;
	cpu_transform m_cpuTransform;
	
	friend class GameManager;
};


#endif


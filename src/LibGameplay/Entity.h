#ifndef ENTITY_H_DEFINED
#define ENTITY_H_DEFINED
#include "pch.h"

#include "Gameplay.h"


class Entity
{
public:
	Entity(bool isServerSide);
	virtual ~Entity();
	void SetID(uint32_t id) { m_id = id; }

	virtual void Update(float dt);

	void Destroy();
	uint32_t GetID() const { return m_id; }
	cpu_transform& GetTransform();
	EntityType GetType() const { return m_type; }

	virtual void InitRender() {};
	virtual void OnRender() {};

protected:
	cpu_entity* m_pCpuEntity;
	
	EntityType m_type;
	bool m_toDestroy = false;
	bool m_isServerSide = false;
	
private:
	static uint32_t ID_COUNT;
	uint32_t m_id;
	cpu_transform m_cpuTransform;
	
	friend class GameManager;
};


#endif


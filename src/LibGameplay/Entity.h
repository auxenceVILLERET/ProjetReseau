#ifndef ENTITY_H_DEFINED
#define ENTITY_H_DEFINED

class Entity
{
public:

	Entity();
	virtual void Update(float dt);

	void Destroy();
	uint32_t GetID() const { return m_id; }

protected:
	cpu_entity* m_pCpuEntity;
	
private:
	static uint32_t ID_COUNT;
	uint32_t m_id;
	bool m_toDestroy;

	friend class GameManager;
};


#endif


#ifndef ENTITY_H_DEFINED
#define ENTITY_H_DEFINED

class Entity
{
public:

	Entity();
	virtual ~Entity();

	virtual void Update(float dt);
	virtual void Render();

	void Destroy();
	uint32_t GetID() const { return m_id; }

protected:
	cpu_entity* m_pCpuEntity;
	
private:
	static uint32_t ID_COUNT;
	uint32_t m_id;
	bool m_toDestroy = false;

	friend class GameManager;
};


#endif


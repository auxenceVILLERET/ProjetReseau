#ifndef ENTITY_H_DEFINED
#define ENTITY_H_DEFINED

struct SphereCollider
{
	XMFLOAT3 center;
	float radius;
};

enum class EntityType
{
	Player,
	Asteroid,
	Bullet
};

class Entity
{
public:

	Entity();
	virtual ~Entity();

	virtual void Update(float dt);
	virtual void UpdateCollider();

	virtual void InitRenderElements() {}
	virtual void UpdateRenderElements(float dt) {}
	virtual void Render();

	void Destroy();
	uint32_t GetID() const { return m_id; }

	virtual void OnCollision(Entity* other) {}

	virtual EntityType GetType() const = 0;


protected:
	cpu_entity* m_pCpuEntity;
	SphereCollider m_collider;

private:
	static uint32_t ID_COUNT;
	uint32_t m_id;
	bool m_toDestroy = false;

	friend class GameManager;
};


#endif


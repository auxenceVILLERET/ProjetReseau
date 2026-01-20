#ifndef GAMEMANAGER_H_DEFINED
#define GAMEMANAGER_H_DEFINED

class Entity;

class GameManager
{
public:
	void Start();
	void Update();
	void Exit();
	void Render(int pass);

	float GetDeltaTime() const { return m_deltaTime; }

	static GameManager* GetInstance();

	template<typename T>
	T* CreateEntity(bool isServerSide = false);

	std::vector<Entity*>& GetEntities();

	~GameManager();
	
private:
	inline static GameManager* s_pInstance = nullptr;
	float m_deltaTime;

	std::vector<Entity*> m_entities;

	GameManager();
};

#include "GameManager.inl"

#endif

#pragma once
#include "Entity.h"

class Asteroid : public Entity
{

public:
	Asteroid();
	~Asteroid();
	void Update(float dt) override;
	void Init(float size);
	void SetPos(XMFLOAT3 pos) { m_pCpuEntity->transform.pos = pos; }

private:
	cpu_mesh m_mesh;
	cpu_material m_material;
};


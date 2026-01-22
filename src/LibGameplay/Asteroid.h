#pragma once
#include "Entity.h"

class Asteroid : public Entity
{

public:
	Asteroid(bool isServerSide);
	~Asteroid();

	void Update(float dt) override;
	void Init(float size);
	void SetPos(XMFLOAT3 pos) { m_pCpuEntity->transform.pos = pos; }
	void SetColor(XMFLOAT3 color) { m_material.color = color; }
	void SetYPR(float yaw, float pitch, float roll);
	void OnCollision(Entity* other) override;
	void ExplosionParticul();

	void InitRenderElements() override;
	void UpdateRenderElements(float dt) override;
	void Render() override;

private:
	XMFLOAT3 m_YPR;
	cpu_mesh m_mesh;
	cpu_material m_material;

	float m_timerParticul;
	float m_durationParticul;
	bool m_Explo = false;

	cpu_particle_emitter* m_pEmitter;

};


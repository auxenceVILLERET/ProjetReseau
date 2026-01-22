#include "pch.h"
#include "Asteroid.h"

Asteroid::Asteroid(bool isServerSide) : Entity(isServerSide)
{
	if (!m_isServerSide)
	{
		m_mesh.CreateSphere(1.0f);
		m_material.color = cpu::ToColor(97, 58, 0);
		m_pCpuEntity->pMesh = &m_mesh;
		m_pCpuEntity->pMaterial = &m_material;
	}

	m_type = EntityType::ASTEROID;
}

Asteroid::~Asteroid()
{
}

void Asteroid::Update(float dt)
{
	m_pCpuEntity->transform.AddYPR(m_YPR.x * dt, m_YPR.y * dt, m_YPR.z * dt );

}

void Asteroid::Init(float size)
{
	m_pCpuEntity->transform.SetScaling(size);
	m_collider.radius = size;
}

void Asteroid::SetYPR(float yaw, float pitch, float roll)
{
	m_YPR = XMFLOAT3(yaw, pitch, roll);
}

void Asteroid::OnCollision(Entity* other)
{
	if(other->GetType() == EntityType::PROJECTILE)
	{
		m_pCpuEntity->pMesh->Clear();
		ExplosionParticul();
	}
}

void Asteroid::ExplosionParticul()
{
	m_pEmitter->pos = m_pCpuEntity->transform.pos;
	m_pEmitter->dir = m_pCpuEntity->transform.dir;
	m_pEmitter->dir.x = 0;
	m_pEmitter->dir.y = 0;
	m_pEmitter->dir.z = 0;
	m_pEmitter->density = 3000.0f;
	m_pEmitter->spread = 5.0f;
	m_Explo = true;
}

void Asteroid::InitRenderElements()
{
	m_pEmitter = cpuEngine.CreateParticleEmitter();
	m_pEmitter->density = 0.0f;
	m_pEmitter->colorMin = cpu::ToColor(100, 0, 0);
	m_pEmitter->colorMax = cpu::ToColor(255, 125, 0);
	m_timerParticul = 0.0f;
	m_durationParticul = 0.2f;
}

void Asteroid::UpdateRenderElements(float dt)
{

	if (m_Explo == true)
	{
		m_timerParticul += dt;
		if (m_timerParticul >= m_durationParticul)
		{
			m_pEmitter->density = 0.0f;
			m_timerParticul = 0.0f;
			delete m_pEmitter;
			Destroy();
		}
	}
}

void Asteroid::Render()
{

}


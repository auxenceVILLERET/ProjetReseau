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
	m_YPR = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotationSpeed = 1.0f;
	m_type = EntityType::ASTEROID;
}

Asteroid::~Asteroid()
{

}

void Asteroid::Update(float dt)
{
	
}

void Asteroid::Init(float size)
{
	Scale(size);
	m_collider.radius = size * 0.5f;
}

void Asteroid::SetRotDir(float yaw, float pitch, float roll)
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
	int r = static_cast<int>(RandomRange(80, 130));
	int g = static_cast<int>(r * RandomRange(0.45f, 0.50f));
	int b = static_cast<int>(r * RandomRange(0, 0));

	SetColor(cpu::ToColor(r, g, b));

	m_rotationSpeed = RandomRange(0.1f, 0.8f);
	float x = RandomRange(-1.0f, 1.0f);
	float y = RandomRange(-1.0f, 1.0f);
	float z = RandomRange(-1.0f, 1.0f);
	SetRotDir(x, y, z);
}

void Asteroid::UpdateRenderElements(float dt)
{
	Rotate(m_YPR.x * dt * m_rotationSpeed, m_YPR.y * dt * m_rotationSpeed, m_YPR.z * dt * m_rotationSpeed);
	
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


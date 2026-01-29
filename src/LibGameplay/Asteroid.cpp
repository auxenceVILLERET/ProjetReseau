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
	m_timerParticul = 0.0f;
	m_durationParticul = 0.2f;
	m_type = EntityType::ASTEROID;
	m_Explo = false;
	m_pEmitter = nullptr;
}

Asteroid::~Asteroid()
{

}

void Asteroid::Update(float dt)
{
	
}

void Asteroid::Destroy()
{
	if (m_isServerSide)
		m_toDestroy = true;
	else
		ExplosionParticul();
}

void Asteroid::Init(float size)
{
	Scale(size);
	m_collider.radius = size * 0.9f;
}

void Asteroid::SetRotDir(float yaw, float pitch, float roll)	
{
	m_YPR = XMFLOAT3(yaw, pitch, roll);
}

void Asteroid::OnCollision(Entity* other)
{
	if(other->GetType() == EntityType::PROJECTILE)
	{
		Destroy();
	}
}

void Asteroid::ExplosionParticul()
{
	m_pCpuEntity->pMesh->Clear();
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
	
	int r = static_cast<int>(RandomRange(80, 130));
	int g = static_cast<int>(r * RandomRange(0.40f, 0.60f));
	int b = 0;

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
			cpuEngine.Release(m_pEmitter);
			m_pEmitter = nullptr;
			m_toDestroy = true;
		}
	}
}

void Asteroid::Render()
{

}


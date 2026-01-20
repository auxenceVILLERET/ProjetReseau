#include "pch.h"
#include "Asteroid.h"

Asteroid::Asteroid()
{
	m_mesh.CreateSphere(1.0f);
	m_material.color = cpu::ToColor(97, 58, 0);
	m_pCpuEntity->pMesh = &m_mesh;
	m_pCpuEntity->pMaterial = &m_material;

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
	if(other->GetType() == EntityType::Bullet)
	{
		Destroy();
	}
}


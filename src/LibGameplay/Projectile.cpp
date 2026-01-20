#include "pch.h"
#include "Projectile.h"

Projectile::Projectile()
{
	m_mesh.CreateSphere(0.5f);
	m_material.color = cpu::ToColor(171, 181, 255);
	m_pCpuEntity->pMesh = &m_mesh;
	m_pCpuEntity->pMaterial = &m_material;
	m_pCpuEntity->transform.SetScaling(0.2f);
	m_speed = 20.0f;
	m_collider.radius = 0.2f;
}

Projectile::~Projectile()
{
	
}

void Projectile::Update(float dt)
{
	m_pCpuEntity->transform.Move(1.0f * dt * m_speed);

	m_distTraveled += dt * m_speed;
	if (m_distTraveled >= m_distMax)
	{
		Destroy();
	}
}

void Projectile::Init(cpu_transform& transform)
{

	m_pCpuEntity->transform.pos = transform.pos;
	m_pCpuEntity->transform.pos.x += transform.dir.x * 2.0f;
	m_pCpuEntity->transform.pos.y += transform.dir.y * 2.0f;
	m_pCpuEntity->transform.pos.z += transform.dir.z * 2.0f;
	m_pCpuEntity->transform.SetRotation(transform);
}

void Projectile::OnCollision(Entity* other)
{
	Destroy();
}

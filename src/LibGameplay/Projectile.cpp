#include "pch.h"
#include "Projectile.h"

Projectile::Projectile(bool isServerSide) : Entity(isServerSide)
{
	if (!isServerSide)
	{
		m_mesh.CreateSphere(0.5f);
		m_material.color = cpu::ToColor(255, 255, 0);	
		m_pCpuEntity->pMesh = &m_mesh;
		m_pCpuEntity->pMaterial = &m_material;
	}
	
	GetTransform().SetScaling(0.2f);
	m_speed = 20.0f;
}

Projectile::~Projectile()
{
	
}

void Projectile::Update(float dt)
{
	GetTransform().Move(1.0f * dt * m_speed);

	m_distTraveled += dt * m_speed;
	if (m_distTraveled >= m_distMax)
	{
		Destroy();
	}
}

void Projectile::Init(cpu_transform& transform)
{

	GetTransform().pos = transform.pos;
	GetTransform().pos.x += transform.dir.x * 2.0f;
	GetTransform().pos.y += transform.dir.y * 2.0f;
	GetTransform().pos.z += transform.dir.z * 2.0f;
	GetTransform().SetRotation(transform);
}
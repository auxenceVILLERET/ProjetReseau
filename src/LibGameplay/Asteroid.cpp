#include "pch.h"
#include "Asteroid.h"

Asteroid::Asteroid()
{
	m_mesh.CreateCube(1.0f);
	m_material.color = cpu::ToColor(97, 58, 0);
	m_pCpuEntity->pMesh = &m_mesh;
	m_pCpuEntity->pMaterial = &m_material;
}

Asteroid::~Asteroid()
{
}

void Asteroid::Update(float dt)
{
}

void Asteroid::Init(float size)
{
	m_pCpuEntity->transform.SetScaling(size);
}


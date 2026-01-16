#pragma once
#include "pch.h"

class Entity
{
public:

	Entity();

	uint32_t GetID() const { return m_id; }

private:
	static uint32_t ID_COUNT;
	uint32_t m_id;
	cpu_entity* m_pCpuEntity;

};


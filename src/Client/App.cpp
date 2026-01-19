#include "pch.h"
#include "Player.h"
#include "GameManager.h"

App::App()
{
	s_pApp = this;
	CPU_CALLBACK_START(OnStart);
	CPU_CALLBACK_UPDATE(OnUpdate);
	CPU_CALLBACK_EXIT(OnExit);
	CPU_CALLBACK_RENDER(OnRender);
	
}

App::~App()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void App::OnStart()
{
	GameManager::GetInstance();

	m_font.Create(12);

	m_pPlayer = GameManager::GetInstance()->CreateEntity<Player>();

	cpuEngine.GetCamera()->transform.pos.z = -8.0f;
	cpuEngine.GetCamera()->transform.pos.y = 2.5f;

	cpuEngine.GetCamera()->transform.SetYPR(0.0f, 0.0f, 0.0f);

}

void App::OnUpdate()
{
	// YOUR CODE HERE

	float dt = cpuTime.delta;
	float time = cpuTime.total;

	GameManager::GetInstance()->Update();
	HandleInut();

	// Quit
	if ( cpuInput.IsKeyDown(VK_ESCAPE) )
		cpuEngine.Quit();
}

void App::OnExit()
{
	// YOUR CODE HERE
	GameManager::GetInstance()->Exit();

}

void App::OnRender(int pass)
{

	switch (pass)
	{
	case CPU_PASS_PARTICLE_BEGIN:
	{
		// Blur particles
		//cpuEngine.SetRT(m_rts[0]);
		//cpuEngine.ClearColor();
		break;
	}
	case CPU_PASS_PARTICLE_END:
	{
		// Blur particles
		//cpuEngine.Blur(10);
		//cpuEngine.SetMainRT();
		//cpuEngine.AlphaBlend(m_rts[0]);
		break;
	}
	case CPU_PASS_UI_END:
	{
		// Debug
		cpu_stats& stats = *cpuEngine.GetStats();
		std::string info = CPU_STR(cpuTime.fps) + " fps, ";
		info += CPU_STR(stats.drawnTriangleCount) + " triangles, ";
		info += CPU_STR(stats.clipEntityCount) + " clipped entities\n";
		info += CPU_STR(cpuEngine.GetParticleData()->alive) + " particles, ";
		info += CPU_STR(stats.threadCount) + " threads, ";
		info += CPU_STR(stats.tileCount) + " tiles";

		// Ray cast
		cpu_ray ray;
		cpuEngine.GetCursorRay(ray);
		cpu_hit hit;
		cpu_entity* pEntity = cpuEngine.HitEntity(hit, ray);
		if (pEntity)
		{
			info += "\nHIT: ";
			info += CPU_STR(pEntity->index).c_str();
		}

		cpuDevice.DrawText(&m_font, info.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), 10, CPU_TEXT_CENTER);
		break;
	}
	}

	
}

void App::HandleInut()
{
	if (cpuInput.IsKey('Z'))
	{
		m_pPlayer->Rotate(0.0f, 1.0f, 0.0f, cpuTime.delta);
	}
	if (cpuInput.IsKey('S'))
	{
		m_pPlayer->Rotate(0.0f, -1.0f, 0.0f, cpuTime.delta);
	}
	if (cpuInput.IsKey('Q'))
	{
		m_pPlayer->Rotate(-1.0f, 0.0f, 0.0f, cpuTime.delta);
	}
	if (cpuInput.IsKey('D'))
	{
		m_pPlayer->Rotate(1.0f, 0.0f, 0.0f, cpuTime.delta);
	}
	if (cpuInput.IsKey('A'))
	{
		m_pPlayer->Rotate(0.0f, 0.0f, 1.0f, cpuTime.delta);
	}
	if (cpuInput.IsKey('E'))
	{
		m_pPlayer->Rotate(0.0f, 0.0f, -1.0f, cpuTime.delta);
	}
	if (cpuInput.IsKey(VK_SPACE))
	{
		m_pPlayer->Move(1.0f, cpuTime.delta);
	}
	if(cpuInput.IsKey(VK_LBUTTON))
	{
		m_pPlayer->Shoot();
	}

}

void App::MyPixelShader(cpu_ps_io& io)
{
	io.color = io.p.color;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

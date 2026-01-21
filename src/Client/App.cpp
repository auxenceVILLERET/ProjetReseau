#include "pch.h"
#include "Player.h"
#include "GameManager.h"
#include "Asteroid.h"
#include <iostream>

const float ARENA_MIN_X = -50.0f;
const float ARENA_MAX_X = 50.0f;
const float ARENA_MIN_Y = -50.0f;
const float ARENA_MAX_Y = 50.0f;
const float ARENA_MIN_Z = -50.0f;
const float ARENA_MAX_Z = 50.0f;

const int ASTEROID_COUNT = 160;

float RandomRange(float min, float max)
{
	return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}

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
	srand(time(NULL));
	cpuEngine.GetCamera()->far = 200.0f;
	cpuEngine.GetCamera()->UpdateProjection();
	cpuEngine.GetParticleData()->Create(1000000);

	m_texture.Load("../../res/Client/CarreRougeVie.png");
	GameManager::GetInstance();

	m_font.Create(12);

	m_pPlayer = GameManager::GetInstance()->CreateEntity<Player>();

	CreateHealthSprite();

	for (int i = 0; i < ASTEROID_COUNT; ++i)
	{
		Asteroid* asteroid = GameManager::GetInstance()->CreateEntity<Asteroid>();

		XMFLOAT3 pos;
		pos.x = RandomRange(ARENA_MIN_X, ARENA_MAX_X);
		pos.y = RandomRange(ARENA_MIN_Y, ARENA_MAX_Y);
		pos.z = RandomRange(ARENA_MIN_Z, ARENA_MAX_Z);

		asteroid->SetPos(pos);
		float size = RandomRange(0.5f, 5.0f);
		asteroid->Init(size);
		int r = static_cast<int>(RandomRange(80, 130));
		int g = static_cast<int>(r * RandomRange(0.45f, 0.50f));
		int b = static_cast<int>(r * RandomRange(0, 0));

		asteroid->SetColor(cpu::ToColor(r, g, b));

		asteroid->SetYPR(RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f));
	}
}

void App::OnUpdate()
{
	// YOUR CODE HERE

	float dt = cpuTime.delta;
	float time = cpuTime.total;

	GameManager::GetInstance()->Update();

	
	HandleInput();
	m_pPlayer->UpdateCamera();
	UpdateHealthSprite();

	// Quit
	if ( cpuInput.IsKeyDown(VK_ESCAPE) )
		cpuEngine.Quit();
}

void App::OnExit()
{
	// YOUR CODE HERE
	GameManager::GetInstance()->Exit();
	delete GameManager::GetInstance();

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

	GameManager::GetInstance()->Render(pass);
	
}

void App::HandleInput()
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
		m_pPlayer->Accelerate(cpuTime.delta);
	}
	if (cpuInput.IsKey(VK_CONTROL))
	{
		m_pPlayer->Brake(cpuTime.delta);
	}
	if(cpuInput.IsKey(VK_LBUTTON))
	{
		m_pPlayer->Shoot();
	}
	if(cpuInput.IsKeyDown('H'))
	{
		m_pPlayer->TakeDamage(5.0f);
	}

}

void App::UpdateHealthSprite()
{
	float currentHealth = m_healthSprites.size() * 10.0f - 10.0f;
	while (currentHealth >= m_pPlayer->GetHealth() && m_healthSprites.size() != 0)
	{
		cpu_sprite* temp = m_healthSprites.back();

		cpuEngine.Release(temp);

		m_healthSprites.pop_back();

		currentHealth -= 10.0f;
	}
}

void App::CreateHealthSprite()
{
	for(int i = 0; i < m_pPlayer->GetHealth() / 10; ++i)
	{
		cpu_sprite* m_pSprite = cpuEngine.CreateSprite();
		m_pSprite->pTexture = &m_texture;
		m_pSprite->CenterAnchor();
		m_pSprite->x = 30 ;
		m_pSprite->y = 50 + (i * 20);
		m_healthSprites.push_back(m_pSprite);
	}
}

void App::MyPixelShader(cpu_ps_io& io)
{
	io.color = io.p.color;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

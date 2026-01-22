#include "pch.h"

#include "Client.h"
#include "Player.h"
#include "GameManager.h"
#include "Asteroid.h"
#include <iostream>

#include "ClientMethods.h"

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

App::App() : m_loginInput(m_loginFont), m_loginHeader(m_loginFont)
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

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void App::OnStart()
{
	srand(time(NULL));
	cpuEngine.GetCamera()->far = 200.0f;
	cpuEngine.GetCamera()->UpdateProjection();
	cpuEngine.GetParticleData()->Create(1000000);

	m_texture.Load("../../res/Client/CarreRougeVie.png");
	GameManager::GetInstance();
	m_font.Create(12);


	/*for (int i = 0; i < ASTEROID_COUNT; ++i)
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
	}*/
	Client* client = Client::GetInstance();
	
	m_font.Create(12);
	m_loginFont.Create(15);
	m_loginHeader.SetText("Please enter your username:");
	m_loginHeader.SetAnchor(CPU_TEXT_CENTER);
	m_loginHeader.SetPos({ 256, 128 - 10 });
	m_loginInput.SetAnchor(CPU_TEXT_CENTER);
	m_loginInput.SetPos({ 256, 128 + 10 });
}

void App::OnUpdate()
{
	// YOUR CODE HERE

	float dt = cpuTime.delta;
	float time = cpuTime.total;
	
	GameManager::GetInstance()->UpdateRenderElements(dt);
	Client::GetInstance()->Update();
	
	if (m_isConnected == false)
		LoginUpdate(dt);
	else
	{
		GameManager::GetInstance()->Update();
		if (m_pPlayer != nullptr)
		{
			HandleInput();
			m_pPlayer->UpdateCamera();
			UpdateHealthSprite();
		}
		else
		{
			Entity* entity = GameManager::GetInstance()->GetEntity(Client::GetInstance()->GetPlayerID());
			m_pPlayer = dynamic_cast<Player*>(entity);
			if(m_pPlayer != nullptr)
				CreateHealthSprite();
		}
	}

	// Quit
	if ( cpuInput.IsKeyDown(VK_ESCAPE) )
		cpuEngine.Quit();
}

void App::OnExit()
{
	// YOUR CODE HERE
	GameManager::GetInstance()->Exit();
	delete GameManager::GetInstance();
	Client::GetInstance()->Exit();
	delete Client::GetInstance();
}

void App::OnRender(int pass)
{
	if (!m_isConnected)
	{
		m_loginHeader.Render();
		m_loginInput.Render();
	}		
	
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
	XMFLOAT3 dir = m_pPlayer->GetTransform().dir;
	float rotSpeed = m_pPlayer->GetRotationSpeed();
	float speed = m_pPlayer->GetSpeed();
	uint32_t id = m_pPlayer->GetID();

	float dt = cpuTime.delta;
	
	if (cpuInput.IsKey('Z'))
	{
		ClientMethods::RotateEntity(id, {0.0f, rotSpeed * dt, 0.0f});
	}
	if (cpuInput.IsKey('S'))
	{
		ClientMethods::RotateEntity(id, {0.0f, -rotSpeed * dt, 0.0f});
	}
	if (cpuInput.IsKey('Q'))
	{
		ClientMethods::RotateEntity(id, {-rotSpeed * dt, 0.0f, 0.0f});
	}
	if (cpuInput.IsKey('D'))
	{
		ClientMethods::RotateEntity(id, {rotSpeed * dt, 0.0f, 0.0f});
		
	}
	if (cpuInput.IsKey('A'))
	{
		ClientMethods::RotateEntity(id, {0.0f, 0.0f, rotSpeed * dt});
	}
	if (cpuInput.IsKey('E'))
	{
		ClientMethods::RotateEntity(id, {0.0f, 0.0f, -rotSpeed * dt});
	}
// 	if (cpuInput.IsKey(VK_SPACE))
// 	{
// 		m_pPlayer->Accelerate(cpuTime.delta);
// 	}
// 	if (cpuInput.IsKey(VK_CONTROL))
// 	{
// 		m_pPlayer->Brake(cpuTime.delta);
// 	}
// 	if(cpuInput.IsKey(VK_LBUTTON))
// 	{
// 		m_pPlayer->Shoot();
// 	}
// if(cpuInput.IsKeyDown('H'))
// {
// 	m_pPlayer->TakeDamage(5.0f);
// }
}

void App::LoginUpdate(float dt)
{
	if (m_isConnecting)
	{
		m_requestTime += dt;
		if (m_requestTime >= 2 * m_requestCooldown && m_isConnected == false)
		{
			m_serverIp.clear();
			m_serverPort = -1;
			m_requestTime = 0.0f;
			m_isConnecting = false;
			m_loginInput.Reset();
			m_loginHeader.SetText("Please enter the server IP :");
		}
		else if (m_requestTime >= m_requestCooldown && m_isConnected == false)
		{
			m_loginHeader.SetText("Connexion attempt failed");
		}
		return;
	}
	
	if (m_username.size() == 0 && m_loginInput.IsFinished())
	{
		m_loginHeader.SetText("Please enter the server IP :");
		m_username = m_loginInput.GetText();
		m_loginInput.Reset();
	}
	else if (m_serverIp.size() == 0 && m_loginInput.IsFinished())
	{
		m_loginHeader.SetText("Please enter the server port :");
		m_serverIp = m_loginInput.GetText();
		m_loginInput.Reset();
	}
	else if (m_serverPort == -1 && m_loginInput.IsFinished())
	{
		m_loginHeader.SetText("Connexion Request Sent");
		m_serverPort = std::stoi( m_loginInput.GetText() );
		m_loginInput.Reset();
		m_isConnecting = true;
		Client::GetInstance()->Connect(m_serverIp, m_serverPort, m_username);
	}

	m_loginInput.HandleInput();

	m_isConnected = Client::GetInstance()->GetIsConnected();
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

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

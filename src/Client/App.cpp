#include "pch.h"

#include "Client.h"
#include "Player.h"
#include "GameManager.h"

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
	GameManager::GetInstance();

	Client* client = Client::GetInstance();
	
	m_font.Create(12);
	m_loginFont.Create(30);
	m_loginHeader.SetText("Please enter your username:");
	m_loginHeader.SetAnchor(CPU_TEXT_CENTER);
	m_loginHeader.SetPos({ cpuWindow.GetWidth() / 2, cpuWindow.GetHeight() / 2 - 30 });
	m_loginInput.SetAnchor(CPU_TEXT_CENTER);
	m_loginInput.SetPos({ cpuWindow.GetWidth() / 2, cpuWindow.GetHeight() / 2 + 30 });
}

void App::OnUpdate()
{
	// YOUR CODE HERE

	float dt = cpuTime.delta;
	float time = cpuTime.total;

	if (m_isConnected == false)
		LoginUpdate(dt);
	else
	{
		GameManager::GetInstance()->Update();
		if (m_pPlayer != nullptr)
		{
			HandleInut();
			m_pPlayer->UpdateCamera();
		}
		else
		{
			Entity* entity = GameManager::GetInstance()->GetEntity(Client::GetInstance()->GetPlayerID());
			m_pPlayer = dynamic_cast<Player*>(entity);
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

	
}

void App::HandleInut()
{
// 	if (cpuInput.IsKey('Z'))
// 	{
// 		m_pPlayer->Rotate(0.0f, 1.0f, 0.0f, cpuTime.delta);
// 	}
// 	if (cpuInput.IsKey('S'))
// 	{
// 		m_pPlayer->Rotate(0.0f, -1.0f, 0.0f, cpuTime.delta);
// 	}
// 	if (cpuInput.IsKey('Q'))
// 	{
// 		m_pPlayer->Rotate(-1.0f, 0.0f, 0.0f, cpuTime.delta);
// 	}
// 	if (cpuInput.IsKey('D'))
// 	{
// 		m_pPlayer->Rotate(1.0f, 0.0f, 0.0f, cpuTime.delta);
// 	}
// 	if (cpuInput.IsKey('A'))
// 	{
// 		m_pPlayer->Rotate(0.0f, 0.0f, 1.0f, cpuTime.delta);
// 	}
// 	if (cpuInput.IsKey('E'))
// 	{
// 		m_pPlayer->Rotate(0.0f, 0.0f, -1.0f, cpuTime.delta);
// 	}
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

void App::MyPixelShader(cpu_ps_io& io)
{
	io.color = io.p.color;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

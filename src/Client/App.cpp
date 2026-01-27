#include "pch.h"

#include "Client.h"
#include "Player.h"
#include "GameManager.h"
#include "Asteroid.h"
#include <iostream>
#include "Gameplay.h"

#include "ClientMethods.h"

const XMFLOAT3 ARENEA_CENTER = { 0.0f, 0.0f, 0.0f };


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

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void App::OnStart()
{;
	cpuEngine.GetCamera()->far = 500.0f;
	cpuEngine.GetCamera()->UpdateProjection();
	cpuEngine.GetParticleData()->Create(20000);

	m_texture.Load("../../res/Client/vie.png");
	GameManager::GetInstance();
	m_font.Create(12, { 1.0f, 1.0f, 1.0f });

	Client* client = Client::GetInstance();

	m_loginHeader.Create(12, { 1.0f, 1.0f, 1.0f });
	m_loginInput.Create(12, { 1.0f, 1.0f, 1.0f });
	m_loginHeader.SetText("Please enter your username:");
	m_loginHeader.SetAnchor(CPU_TEXT_CENTER);
	m_loginHeader.SetPos({ 256, 128 - 10 });
	m_loginInput.SetAnchor(CPU_TEXT_CENTER);
	m_loginInput.SetPos({ 256, 128 + 10 });

	m_respawnText.Create(12, { 0.0f, 1.0f, 0.0f });
	m_respawnText.SetAnchor(CPU_TEXT_CENTER);
	m_respawnText.SetPos({ 256, 128 });

	m_outOfArenaText.Create(12, { 1.0f, 0.0f, 0.0f });
	m_outOfArenaText.SetAnchor(CPU_TEXT_CENTER);
	m_outOfArenaText.SetPos({ 256, 128 - 10 });

	m_usernameText.Create(12, { 1.0f, 1.0f, 1.0f });
	m_usernameText.SetAnchor(CPU_TEXT_CENTER);
	m_usernameText.SetPos({ 256, 220 });

	m_chatText.Create(12, { 1.0f, 1.0f, 1.0f });
	m_chatText.SetAnchor(CPU_TEXT_LEFT);
	m_chatText.SetPos({ 340, 220 });
	m_chatText.SetText("Chat:");
	
	m_chatInput.Create(12, { 1.0f, 1.0f, 1.0f });
	m_chatInput.SetAnchor(CPU_TEXT_LEFT);
	m_chatInput.SetPos({ 400, 220 });

	CreateHealthSprite();
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
		if (m_pPlayer != nullptr)
		{
			if(m_chatOpen == false)
				HandleInput();
			OutOfArenaUpdate(dt);
			m_pPlayer->UpdateCamera();
			UpdateHealthSprite();
			ChatUpdate();
			if (m_pPlayer->IsAlive() == false)
			{
				if (m_pPlayer->GetActiveState() == true)
				{
					ClientMethods::SetActiveState(m_pPlayer->GetID(), false);
				}
			}

			if (m_respawnTimer < m_timeRespawn && m_pPlayer->IsAlive() == false)
			{
				m_respawnTimer += dt;
				if (m_respawnTimer >= 2.0f)
				{
					m_respawnText.SetText("Respawning in 1...");
				}
				else if (m_respawnTimer >= 1.0f)
				{
					m_respawnText.SetText("Respawning in 2...");
				}
				else if (m_respawnTimer >= 0.0f)
				{
					m_respawnText.SetText("Respawning in 3...");
				}

				if (m_respawnTimer >= m_timeRespawn)
				{
					m_respawnTimer = 0.0f;
					Respawn();
				}
			}

		}
		else
		{
			Entity* entity = GameManager::GetInstance()->GetEntity(Client::GetInstance()->GetPlayerID());
			m_pPlayer = dynamic_cast<Player*>(entity);
			if(m_pPlayer != nullptr)
				CreateHealthSprite();
		}
	}

	GameManager::GetInstance()->ClearDestroyedEntities();
	
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

void App::Respawn()
{
	uint32_t id = m_pPlayer->GetID();

	ClientMethods::SetActiveState(id, true);

	ClientMethods::SetHealth(id, 100.0f);

	XMFLOAT3 spawnPos = GetSpawnPoint();
	ClientMethods::SetPosition(id, spawnPos);

	XMFLOAT3 dir = {
		ARENEA_CENTER.x - spawnPos.x,
		ARENEA_CENTER.y - spawnPos.y,
		ARENEA_CENTER.z - spawnPos.z
	};
	float length = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	if (length > 0.0001f)
	{
		dir.x /= length;
		dir.y /= length;
		dir.z /= length;
	}
	ClientMethods::SetDirection(id, dir);
}

void App::ChatUpdate()
{
	if(m_chatOpen)
	{
		m_chatInput.HandleInput();

		if (m_chatInput.IsFinished())
		{
			ClientMethods::SendChatMessage(m_pPlayer->GetID(),m_chatInput.GetText().c_str());
			ClientMethods::AddChatMessage(m_username, m_chatInput.GetText());
			m_chatOpen = false;
		}
		return;
	}
}

void App::OnRender(int pass)
{
	if (!m_isConnected)
	{
		m_loginHeader.Render();
		m_loginInput.Render();
	}

	if(m_isConnected)
	{
		m_usernameText.SetText(m_username);
		m_usernameText.Render();
	}

	if (m_chatOpen)
	{
		m_chatInput.Render();
		m_chatText.Render();
		float y = 100.0f;

		for (const ChatLine& line : ClientMethods::s_chatMessages)
		{
			std::string msg = "[" + line.user + "] " + line.text;

			cpuDevice.DrawText(&m_font,msg.c_str(),380,y,CPU_TEXT_LEFT);

			y += 16.0f;
		}
	}

	if(m_pPlayer != nullptr && m_pPlayer->IsAlive() == false && m_respawnTimer < m_timeRespawn)
	{
		m_respawnText.Render();
	}

	if (m_pPlayer != nullptr && InArena() == true && m_pPlayer->IsAlive() == true)
	{
		m_outOfArenaText.Render();
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
		// info += CPU_STR(stats.drawnTriangleCount) + " triangles, ";
		// info += CPU_STR(stats.clipEntityCount) + " clipped entities\n";
		info += CPU_STR(cpuEngine.GetParticleData()->alive) + " particles, ";
		// info += CPU_STR(stats.threadCount) + " threads, ";
		// info += CPU_STR(stats.tileCount) + " tiles";

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
	if (cpuInput.IsKey(VK_SPACE))
	{
		ClientMethods::ChangePlayerSpeed(id, dt * 3.0f);
	}
	if (cpuInput.IsKey(VK_CONTROL))
	{
		ClientMethods::ChangePlayerSpeed(id, -dt * 8.0f);
	}
 	if(cpuInput.IsKey(VK_LBUTTON))
 	{
 		if (m_pPlayer->Shoot())
			ClientMethods::ShootProjectile(m_pPlayer->GetTransform().pos, dir);
 	}
	if(cpuInput.IsKeyDown('T'))
	{
		m_chatOpen = true;
		m_chatInput.Reset();
	}
	if (cpuInput.IsKeyDown('H'))
	{
		m_pPlayer->TakeDamage(10.0f);
	}
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

bool App::InArena()
{
	if (m_pPlayer->GetPos().x > BORDER_MAX)
		return true;
	if(m_pPlayer->GetPos().x < BORDER_MIN)
		return true;
	if (m_pPlayer->GetPos().y > BORDER_MAX)
		return true;
	if (m_pPlayer->GetPos().y < BORDER_MIN)
		return true;
	if (m_pPlayer->GetPos().z > BORDER_MAX)
		return true;
	if (m_pPlayer->GetPos().z < BORDER_MIN)
		return true;
	return false;
}

void App::OutOfArenaUpdate(float dt)
{
	if (InArena() && m_pPlayer->IsAlive() == true)
	{
		m_outTimer += dt;
		
		if (m_outTimer >= 7.0f)
		{
			m_outOfArenaText.SetText("Return to the arena in 1...");
		}
		else if (m_outTimer >= 6.0f)
		{
			m_outOfArenaText.SetText("Return to the arena in 2...");
		}
		else if (m_outTimer >= 5.0f)
		{
			m_outOfArenaText.SetText("Return to the arena in 3...");
		}
		else if (m_outTimer >= 4.0f)
		{
			m_outOfArenaText.SetText("Return to the arena in 4...");
		}
		else if (m_outTimer >= 3.0f)
		{
			m_outOfArenaText.SetText("Return to the arena in 5...");
		}
		else if (m_outTimer >= 2.0f)
		{
			m_outOfArenaText.SetText("Return to the arena in 6...");
		}
		else if (m_outTimer >= 1.0f)
		{
			m_outOfArenaText.SetText("Return to the arena in 7...");
		}
		if (m_outTimer >= m_timeBeforeOut)
		{
			m_outTimer = 0.0f;
			m_outOfArenaText.SetText("Return to the arena in 8...");
			m_pPlayer->TakeDamage(100.0f);
		}
	}
	else
	{
		m_outTimer = 0.0f;
		m_outOfArenaText.SetText("Return to the arena in 8...");
	}
}

void App::UpdateHealthSprite()
{
	float currentHealth = m_pPlayer->GetHealth();
	for (int i = 0; i < 10; i++)
	{
		if (currentHealth > i * 10.0f)
			m_healthSprites[i]->visible = true;
		else
			m_healthSprites[i]->visible = false;
	}
}

void App::CreateHealthSprite()
{
	for(int i = 0; i < 10; ++i)
	{
		cpu_sprite* m_pSprite = cpuEngine.CreateSprite();
		m_pSprite->pTexture = &m_texture;
		m_pSprite->CenterAnchor();
		m_pSprite->x = 30 ;
		m_pSprite->y = 50 + (i * 20);
		m_healthSprites.push_back(m_pSprite);
		m_pSprite->visible = false;
	}
}

void App::MyPixelShader(cpu_ps_io& io)
{
	io.color = io.p.color;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

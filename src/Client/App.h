#pragma once
#include "Entity.h"
#include "InputText.h"

class Player;
class Asteroid;

struct ChatLine
{
	std::string user;
	std::string text;
};

class App
{
public:
	App();
	virtual ~App();

	static App& GetInstance() { return *s_pApp; }

	void OnStart();
	void OnUpdate();
	void OnExit();
	void OnRender(int pass);

	static void MyPixelShader(cpu_ps_io& io);

	void HandleInput();

	void UpdateHealthSprite();
	void CreateHealthSprite();

	void LoginUpdate(float dt);

	bool InArena();
	void OutOfArenaUpdate(float dt);

	void Respawn();
	void ChatUpdate();

	bool AddChatMessage(const std::string& user, const std::string& msg);
	void ClearChatMessages();

	void RenderOtherPlayersHealth();
	std::string MakeHpBar(float currentHealth, float maxHealth);

private:
	std::vector<ChatLine> s_chatMessages;
	inline static App* s_pApp = nullptr;
	cpu_font m_font;

	Player* m_pPlayer = nullptr;
	cpu_texture m_texture;
	std::vector<cpu_sprite*> m_healthSprites;

	////////// OUT OF ARENA //////////////
	float m_timeBeforeOut = 8.0f;
	float m_outTimer = 0.0f;
	InputText m_outOfArenaText;
	////////////////////////////////////

	//////// RESPAWN //////////////
	float m_timeRespawn = 3.0f;
	float m_respawnTimer = 0.0f;
	InputText m_respawnText;
	//////////////

	////////// LOGIN //////////////
	bool m_isConnected = false;
	std::string m_username;
	std::string m_serverIp;
	int m_serverPort = -1;

	bool m_isConnecting = false;
	float m_requestCooldown = 1.0f;
	float m_requestTime = 0.0f;
	
	InputText m_loginHeader;
	InputText m_loginInput;
	///////////////////////////////

	////////// USERNAME //////////////
	InputText m_usernameText;
	////////////////////////////////

	///////// CHAT ////////////////
	bool m_chatOpen = false;
	bool m_newChatMessage = false;
	InputText m_chatInput;
	InputText m_chatText;
	float m_chatUpdateTimer = 0.0f;
	float m_chatUpdateCooldown = 5.0f;
	////////////////////////////////

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



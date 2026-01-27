#ifndef SERVER_H_DEFINED
#define SERVER_H_DEFINED

#include <map>
#include <vector>

#include "Chrono.h"
#include "Message.h"
#include "CriticalSection.h"
#include "../LibNetwork/UDPSocket.h"

struct ClientInfo
{
    sockaddr_in sockAddr;
    std::string username;
    std::string ip;
    int port = 0;
    bool connected = false;
    
    int playerId = 0;
    int deathCount = 0;
    int killCount = 0;
};

struct ReceivedPacket
{
    Packet* packet;
    sockaddr_in sockAddr;
};

class Server
{
public:
    Server();
    ~Server();

    static Server* GetInstance();
    UDPSocket* GetSocket();

    bool LogUser(sockaddr_in newAddr, std::string username);
    void GlobalMsg(const char* msg);
    void PrintMessage(const char* msg, sockaddr_in addr);

    static DWORD WINAPI ReceiveThread(LPVOID lpParam);
    void HandlePackets();
    
    void SendPacket(Packet* packet);
    void SendTargetedPacket(Packet* packet, ClientInfo* target);
    ClientInfo* FindClient(std::string username);
    void ClearMessages();
    
    void Update();
        
private:
    static Server* m_pInstance;
    UDPSocket m_udpSocket;

    CriticalSection m_packetProtection;
    std::vector<ReceivedPacket> m_packets;
    std::vector<Message*> m_pendingMessages;
    std::map<ClientInfo*, std::vector<Message*>> m_pendingTargetedMessage;
    std::vector<ClientInfo*> m_vClients;

    char m_buffer[1024];
    
    void Init();
    Chrono m_chrono;
	int m_asteroidDestroyedCount = 0;
};

#endif

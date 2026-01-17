#ifndef SERVER_H_DEFINED
#define SERVER_H_DEFINED

#include <vector>

#include "Message.h"
#include "CriticalSection.h"
#include "../LibNetwork/UDPSocket.h"

struct ClientInfo
{
    sockaddr_in sockAddr;
    std::string ip;
    int port;
    std::string username;
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

    void Update();
        
private:
    static Server* m_pInstance;
    UDPSocket m_udpSocket;

    CriticalSection m_packetProtection;
    std::vector<ReceivedPacket> m_packets;
    std::vector<Message> m_pendingMessages;
    std::vector<ClientInfo> m_vClients;
    
    void Init();
};

#endif

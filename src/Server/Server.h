#ifndef SERVER_H_DEFINED
#define SERVER_H_DEFINED

#include <vector>

#include "Message.h"
#include "../LibNetwork/UDPSocket.h"
#include "Client/Client.h"

struct ClientInfo
{
    sockaddr_in sockAddr;
    std::string ip;
    int port;
    std::string username;
};

class Server
{
public:
    Server();
    ~Server();

    static Server* GetInstance();
    UDPSocket* GetSocket();

    void LogUser(sockaddr_in newAddr);
    void GlobalMsg(const char* msg);
    void PrintMessage(const char* msg, sockaddr_in addr);

    static DWORD WINAPI ReceiveThread(LPVOID lpParam);
    void HandlePackets();

    void SendPacket(Packet* packet);
        
private:
    static Server* m_pInstance;
    UDPSocket m_udpSocket;

    std::vector<Packet*> m_packets;
    std::vector<Message> m_pendingMessages;
    std::vector<sockaddr_in> m_vClients;
    
    void Init();
};

#endif

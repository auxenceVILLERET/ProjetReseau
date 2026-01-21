#ifndef CLIENT_H_DEFINED
#define CLIENT_H_DEFINED

#include <vector>

#include "UDPSocket.h"
#include "CriticalSection.h"

class Packet;

class Client 
{
public:
    Client();
    ~Client();

    static Client* GetInstance();
    UDPSocket* GetSocket();

    void Init();
    bool Connect(std::string ip, int port);
    void Exit() { m_isRunning = false; }

    static DWORD WINAPI ReceiveThread(LPVOID lpParam);
    void HandlePackets();
    
private:
    static Client* m_pInstance;

    CriticalSection m_packetProtection;
    std::vector<Packet*> m_packets;
    
    UDPSocket m_udpSocket;

    char m_buffer[1024];
    
    std::string m_serverIp;
    int m_serverPort = 1888;
    std::string m_username;

    bool m_isRunning = true;
    bool m_isConnected;
    bool m_hasPinged;
};

#endif

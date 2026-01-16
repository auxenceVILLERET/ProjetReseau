#ifndef CLIENT_H_DEFINED
#define CLIENT_H_DEFINED

#include <vector>

#include "UDPSocket.h"

class Client 
{
public:
    Client();
    ~Client();

    static Client* GetInstance();
    UDPSocket* GetSocket();

    void Init();
    bool Connect(char* ip, int port);
    
private:
    static Client* m_pInstance;
    
    UDPSocket m_udpSocket;

    char* m_serverIp = nullptr;
    int serverPort = 1888;
    std::string m_username;
    
};

#endif

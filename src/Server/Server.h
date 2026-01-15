#ifndef SERVER_H_DEFINED
#define SERVER_H_DEFINED

#include <vector>

#include "../LibNetwork/UDPSocket.h"

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
        
private:
    static Server* m_pInstance;
    UDPSocket m_udpSocket;

    std::vector<sockaddr_in> m_vClients;
    
    void Init();
};

#endif

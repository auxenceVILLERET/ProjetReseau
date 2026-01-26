#include "TCPSocket.h"
#include <iostream>
#include <sstream>
#include <WS2tcpip.h>

TCPSocket::TCPSocket()
{
    mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mSocket == INVALID_SOCKET)
    {
        std::ostringstream error;
        error << "Erreur initialisation socket [" << Sockets::GetError() << "]";
        throw std::runtime_error(error.str());
    }
}

TCPSocket::~TCPSocket()
{
    Sockets::CloseSocket((int)mSocket);
}

bool TCPSocket::Connect(const std::string& ipaddress, unsigned short port)
{
    sockaddr_in server;
    if ( inet_pton(AF_INET, ipaddress.c_str(), &server.sin_addr)<=0 )
        return false;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    return connect(mSocket, (sockaddr*)&server, sizeof(server)) == 0;
}

int TCPSocket::Send(const char* data, unsigned int len)
{
    unsigned short networkLen = htons(len);
    return send(mSocket, reinterpret_cast<const char*>(& networkLen), sizeof(networkLen), 0) == sizeof(networkLen)
        && send(mSocket, reinterpret_cast<const char*>(data), len, 0) == len;
}
int TCPSocket::Receive(char* buffer, unsigned int len)
{
    return recv(mSocket, buffer, len, 0);
}
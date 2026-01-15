#include "UDPSocket.h"
#include <iostream>
#include <sstream>
#include <WS2tcpip.h>

UDPSocket::UDPSocket()
{
    mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (mSocket == INVALID_SOCKET)
    {
        std::ostringstream error;
        error << "Erreur initialisation socket [" << Sockets::GetError() << "]";
        throw std::runtime_error(error.str());
    }
}

UDPSocket::~UDPSocket()
{
    Sockets::CloseSocket(mSocket);
}

bool UDPSocket::Bind(const std::string& ipaddress, unsigned short port)
{
    sockaddr_in server;
    if ( inet_pton(AF_INET, ipaddress.c_str(), &server.sin_addr)<=0 )
        return false;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    int res = bind(mSocket, (sockaddr*)&server, sizeof(server));

    if (res == SOCKET_ERROR)
        return false;

    m_adress = &server;
    isBinded = true;
    return true;
}

int UDPSocket::SendTo(const char* data, unsigned int len, sockaddr_in& target)
{
    return sendto(mSocket, data, len, 0, reinterpret_cast<sockaddr*>(&target), sizeof(target));
}

int UDPSocket::ReceiveFrom(char* buffer, unsigned int len, sockaddr_in& target)
{
    int addrSize = sizeof(target);
    return recvfrom(mSocket, buffer, len, 0, reinterpret_cast<sockaddr*>(&target), &addrSize);
}
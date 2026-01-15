#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP
#pragma once

#include "Socket.h"

#include <string>
#include <WinSock2.h>

class UDPSocket
{
public:
    UDPSocket();
    ~UDPSocket();

    bool Bind(const std::string& ipaddress, unsigned short port);
    int SendTo(const char* data, unsigned int len, sockaddr_in& target);
    int ReceiveFrom(char* buffer, unsigned int len, sockaddr_in& target);

    sockaddr_in* GetAddress() { return m_adress; };

private:
    SOCKET mSocket;
    sockaddr_in* m_adress = nullptr;
    bool isBinded = false;
};

#endif // TCPSOCKET_HPP
#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP
#pragma once

#include "Socket.h"

#include <string>
#include <WinSock2.h>

class TCPSocket
{
public:
    TCPSocket();
    ~TCPSocket();

    bool Connect(const std::string& ipaddress, unsigned short port);
    int Send(const char* data, unsigned int len);
    int Receive(char* buffer, unsigned int len);

private:
    SOCKET mSocket;
};

#endif // TCPSOCKET_HPP
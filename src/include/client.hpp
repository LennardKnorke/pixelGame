#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "stdlibs.hpp"

class Clients {

    protected:
    sf::TcpSocket socket;
    bool isConnected;

    public:

    bool connect(sf::IpAddress hostIp, unsigned short hostPort);
    bool sendData(sf::Packet &OutGoingPacket);
    bool receiveData(sf::Packet &incomingDestination);
    void disconnect(void);
};
#endif //CLIENT_HPP
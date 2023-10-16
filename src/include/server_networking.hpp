#pragma once
#ifndef SERVER_NETWORKING_HPP
#define SERVER_NETWORKING_HPP
#include "stdlibs.hpp"
#include "gamesave.hpp"

//void server_thread_function(unsigned short port, sf::IpAddress adress, std::string savePath);

class Server {
    protected:
    unsigned short serverPort;
    sf::IpAddress serverAdress;
    sf::TcpListener serverSocket;

    gameSave *GAME;
    public:
    Server(unsigned short port, sf::IpAddress adress, std::string savePath);
    void acceptConnection();
    void sendPacket();


};





#endif //SERVER_NETWORKING_HPP
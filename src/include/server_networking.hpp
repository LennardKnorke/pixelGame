#pragma once
#ifndef SERVER_NETWORKING_HPP
#define SERVER_NETWORKING_HPP
#include "stdlibs.hpp"
#include "gamesave.hpp"

//void server_thread_function(unsigned short port, sf::IpAddress adress, std::string savePath);

class Server {
    private:
    unsigned short serverPort;
    sf::IpAddress serverAdress;
    sf::TcpListener serverSocket;
    std::string hostId;
    void acceptConnection();
    void sendPacket();
    void HandleClient(sf::TcpSocket clientSocket);
    gameSave *GAME;
    public:
    Server(unsigned short port, sf::IpAddress adress, std::string savePath, std::string hostId);
    


};





#endif //SERVER_NETWORKING_HPP
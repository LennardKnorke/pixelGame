#pragma once
#ifndef SERVER_NETWORKING_HPP
#define SERVER_NETWORKING_HPP
#include "stdlibs.hpp"
#include "gamesave.hpp"

//void server_thread_function(unsigned short port, sf::IpAddress adress, std::string savePath);

struct serverClient {
    bool connected = false;
    std::string key = "";
    sf::TcpSocket *socket=nullptr;
};

class Server {
    private:

    unsigned short serverPort;
    sf::IpAddress serverAdress;
    sf::TcpListener serverListener;
    bool hostConnected;
    gameMode mode;
    int maxPlayers;
    std::string hostId;
    std::vector<serverClient> connectedClients;
    gameSave *GAME;

    bool setUp_LocalServer();
    bool setUp_OnlineServer();
    bool loadGameSave();

    void acceptConnections();
    bool hostConnectionCheck();
    void sendPacket();
    void HandleClient(sf::TcpSocket clientSocket);
    
    public:
    Server(unsigned short port, sf::IpAddress adress, std::string savePath, std::string hostId, gameMode modeToLoad);
    


};





#endif //SERVER_NETWORKING_HPP
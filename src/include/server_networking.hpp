#pragma once
#ifndef SERVER_NETWORKING_HPP
#define SERVER_NETWORKING_HPP
#include "stdlibs.hpp"
#include "gamesave.hpp"


struct timeOutTimer {
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> current;
    std::chrono::seconds difference;
};

struct serverClient {
    bool active = false;
    unsigned short id = USHRT_MAX;
    std::string key = "";
    sf::Thread *threadPtr = nullptr;
    sf::TcpSocket socket;
    timeOutTimer timeOut;
    playerMessagePackage playerMessage;
};


//Empty=Unused
bool clientInUse(serverClient *c);
void clearClient(serverClient *c);

class Server {
    private:

    unsigned short serverPort;
    sf::IpAddress serverAdress;
    sf::TcpListener serverListener;
    struct hostinfo{
        bool connected;
        std::string hostId;
        timeOutTimer timeOutTimer;
    }hostConnectionState;
    
    gameMode mode;
    unsigned short maxPlayers;
    
    std::vector<serverClient*> clientsAvailable;
    gameSave *GAME;

    bool setUp_LocalServer();
    bool setUp_OnlineServer();
    bool loadGameSave();

    void acceptConnections();
    unsigned short getFreeClientPosition();
    
    bool hostConnectionCheck();
    void updateTimeOut(timeOutTimer *timer);
    void initTimeOut(timeOutTimer *timer);

    void sendPacket();
    void handleClient();
    
    public:
    Server(unsigned short port, sf::IpAddress adress, std::string savePath, std::string hostId, gameMode modeToLoad);
    


};

typedef struct clientThreadParameters{
    unsigned short i;
}clientThreadParameters;
void clientThread(clientThreadParameters p);


#endif //SERVER_NETWORKING_HPP
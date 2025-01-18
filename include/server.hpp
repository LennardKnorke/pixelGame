#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>

#include "gamesave.hpp"
#include "utils.hpp"

class server_clients{
    private:
    
    std::string userID;
    
    public:
    bool inUse;
    sf::TcpSocket socket;
    void reset(void);
    void send(void);
    void receive(void);
    server_clients(void);

};


class Server {
    private:
    bool DEV_MODE;
    gameMode mode;
    GameSave *save;

    short max_players;
    short n_players;
    unsigned short port;
    sf::IpAddress ip;
    bool running = true;

    std::vector<server_clients *> clients;
    sf::Thread *accepting_thread;
    sf::TcpListener listener;
    
    bool loadSave(std::string path);
    bool save_game(void);
    void acceptConnections(void);
    
    short getFreeClient(void);

    public:
    Server(unsigned short port, sf::IpAddress ip, std::string savePath, short maxPlayer, bool dev_mode);
    bool run(void);
    bool terminate(void);

};


#endif //SERVER_HPP
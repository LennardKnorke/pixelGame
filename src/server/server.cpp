

#include "server.hpp"

Server::Server(unsigned short port, sf::IpAddress ip, std::string savePath, short maxPlayer, bool dev_mode){
    running = true;

    // Save params
    this->port = port;
    this->ip = ip;
    this->mode = mode;
    this->DEV_MODE = dev_mode;
    this->max_players = maxPlayer;

    // Init other params
    n_players = 0;

    for  (auto i = 0; i < max_players; i++){
        clients.push_back(new server_clients());
    }
    if (DEV_MODE){
        std::cout << "INITIALIZED SERVER" << std::endl;
    }

    // Load save file
    if (!loadSave(savePath)){
        std::cerr << "Error loading save file" << std::endl;
        running = false;
        return;
    }

    // Start accepting connections
    accepting_thread = new sf::Thread(&Server::acceptConnections, this);
}


bool Server::run(void){
    // Return early if initialization failed
    if (!running){
        this->terminate();
        return false;
    }

    if (DEV_MODE){
        std::cout << "\tServer is running" << std::endl;
    }
    accepting_thread->launch();
    // Begin of server loop
    while (running){
        if (n_players == 0){
            if (DEV_MODE){
                std::cout << "\tNo active clients" << std::endl;
            }
            sf::sleep(sf::seconds(15));
            if (n_players == 0){
                std::cout << "\tServer Shutdown" << std::endl;
                running = false;
            }
        }
    }

    // End of server loop
    return this->terminate();
}

short Server::getFreeClient(void){
    for (auto i = 0; i < max_players; i++){
        if (!clients[i]->inUse){
            return i;
        }
    }
    return -1;
}




bool Server::terminate(void){
    if (DEV_MODE){
        std::cout << "TERMINATING SERVER" << std::endl;
    }
    listener.close();
    for (auto i = 0; i < max_players; i++){
        clients[i]->reset();
        delete clients[i];
    }
    accepting_thread->terminate();
    delete accepting_thread;
    // Save Game
    if (!save_game()){
        std::cerr << "\tError saving game" << std::endl;
        return false;
    }
    return true;
}

void Server::acceptConnections(void){
    sf::Packet packet;
    packet << "Hello";
    sf::TcpSocket socket;
    if (listener.listen(port) != sf::Socket::Done){
        std::cerr << "Error listening on port" << std::endl;
        running = false;
        return;
    }

    short clientIndex = getFreeClient();
    std::cout << "\tListening on port " << port << std::endl;
    while (running){
        // Accept new connections if there are free clients
        if (clientIndex != -1 && listener.accept(clients[clientIndex]->socket) == sf::Socket::Done){
            std::cout << "\tConnection accepted" << std::endl;
            socket.receive(packet);
            std::string userID;
            packet >> userID;
            packet.clear();
            userID = "Welcome " + userID;
            std::cout << userID << std::endl;
            packet << userID;
            socket.send(packet);
            packet.clear();
        }
        // Check if there are any free clients
        clientIndex = getFreeClient();
        // If no free clients, wait for 5 seconds
        if (clientIndex == -1){
            sf::sleep(sf::seconds(5));
        }
    }
}


bool Server::loadSave(std::string path){
    std::ifstream file(path, std::ios::binary);
    file.open(path);
    if (!file.is_open()){
        return false;
    }
    return true;
}

bool Server::save_game(void){
    return true;
}


server_clients::server_clients(void){
    inUse = false;
}

void server_clients::reset(void){
    inUse = false;
    socket.disconnect();
}
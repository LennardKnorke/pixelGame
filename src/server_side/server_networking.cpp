#include "server_networking.hpp"


Server::Server(unsigned short port, sf::IpAddress adress, std::string savePath, std::string hostId, gameMode modeToLoad){
    //SET UP SERVER Variables
    this->hostConnectionState.connected = false;
    this->hostConnectionState.hostId = hostId;
    this->serverPort = port;
    this->serverAdress = adress;
    this->mode = modeToLoad;
    this->hostConnectionState.timeOutTimer.start = std::chrono::high_resolution_clock::now();
    this->hostConnectionState.timeOutTimer.current = hostConnectionState.timeOutTimer.start;
    this->hostConnectionState.timeOutTimer.difference = std::chrono::duration_cast<std::chrono::seconds>(this->hostConnectionState.timeOutTimer.current - this->hostConnectionState.timeOutTimer.start);
    
    //Maximum number of allowed connection depends on playing mode
    if (mode == gameMode::Single){
        this->maxPlayers = 1;
    }
    else {
        this->maxPlayers = 4;
    }
 
    //Load Game (will check for errors/corruptions etc.)
    if (!loadGameSave(savePath)){
        std::cout << "Failed to load GameSave\n";
        return;
    }
    system("pause");
    //Set up server either locally or online
    //(both work the EXACT same way right now due to hamachi)
    if (mode_Online(mode)){
        if (!setUp_OnlineServer()){
            std::cout << "Failed to set up online server\n";
            return;
        }
    }
    else {
        if (!setUp_LocalServer()){
            std::cout << "Failed to set up local server\n";
            return;
        }
    }

    //Everything worked? 
    //set up pointers to other client threads/sockets etc.
    for (int i = 0; i < this->maxPlayers; i++){
        clientsAvailable.push_back(new serverClient);
    }

    //Start Accepting New Connections;
    sf::Thread connectingThread(&acceptConnections, this);
    connectingThread.launch();

    ///SERVER LOOP
    //  -host check
    //      - host not connected? run timeout
    //          - end if host ist disconnected for too long
    //      - host WAS connected && is not anymore in the client list -> start timouttimer
    //  - update gameState (or should it run in another thread, memory risks!!)



    //SERVER LOOP STARTS HERE
    std::cout << "Server ready. Listening on adress: " << serverAdress.toString() << ":" << serverPort <<std::endl;
    bool SERVER_RUNNING = true;
    while (SERVER_RUNNING){

        if (SERVER_RUNNING && hostConnectionState.connected == false){

            updateTimeOut(&hostConnectionState.timeOutTimer);

            //End process if host disconnected for 15 seconds
            if (hostConnectionState.timeOutTimer.difference.count() > 15){
                //Stop accepting new connections
                connectingThread.terminate();
                //End all Client threads
                for (serverClient* tmp : clientsAvailable){
                    if (clientInUse(tmp)){
                        clearClient(tmp);
                    }
                }
                //End Server Process
                SERVER_RUNNING = false;
                std::cout << "Host failed to connect/timed out\n";
                break;
            }
        }
        else if (SERVER_RUNNING && hostConnectionState.connected && !hostConnectionCheck()){
            std::cout<< "Host disconnected!\n";
            initTimeOut(&hostConnectionState.timeOutTimer);
            hostConnectionState.connected = false;
            //PAUSE GAME!
        }


        //Pass info to game


        //Update Gamestate


    }
    //END OF SERVER LOOP (SERVER_RUNNING == FALSE)


    //End of server process. delete all pointers
    for (serverClient* c : clientsAvailable){
        delete c;
    }
    //End(/save?) game
    if (GAME){
        delete GAME;
    }
    return;
}


bool Server::loadGameSave(std::string savePath){

    GAME = new gameSave(savePath);
    if (GAME->getSaveLoadedState()){
        return true;
    }
    else {
        return false;
    }
}


//Set up server on given local port + port
bool Server::setUp_LocalServer(){
    if (serverListener.listen(serverPort) != sf::Socket::Done){
        std::cout << "ERROR CONNECTING PORT AND SOCKET\n";
        return false;
    }
    return true;
}


//Hamachi magic, it does the same as local servers
bool Server::setUp_OnlineServer(){
    if (serverListener.listen(serverPort) != sf::Socket::Done){
        std::cout << "ERROR CONNECTING PORT AND SOCKET\n";
        return false;
    }
    return true;
}


//
void Server::acceptConnections(void){

    while (true){
        unsigned short socketPosition= getFreeClientPosition();
        if (socketPosition != USHRT_MAX && serverListener.accept(clientsAvailable[socketPosition]->socket) == sf::Socket::Done) {
            std::cout << "New client connected" << std::endl;
            clientThreadParameters parameters;
            parameters.i = socketPosition;
            parameters.yourClientPlace = clientsAvailable[socketPosition];
            clientsAvailable[socketPosition]->threadPtr = new sf::Thread(clientThread, parameters);
            clientsAvailable[socketPosition]->threadPtr->launch();
        }
    }
}




unsigned short Server::getFreeClientPosition(){
    for (unsigned short i = 0; i<maxPlayers; i++){
        if (!clientInUse(clientsAvailable[i])){
            return i;
        }
    }
    return USHRT_MAX;
}


void clientThread(clientThreadParameters p){
    p.yourClientPlace->id = p.i;
    p.yourClientPlace->active = true;
    p.yourClientPlace->connected = true;

    sf::Packet package;
    //Get id from clinet

    //(Option 1) Send full gameworld here

    //LOOP
        //  Timouthandling
        //  get player info about client from gamesave
        //  (Option 1) send updates to players; (Option 2) send window to player


    // Exchange Loop
    while (true){
        package << std::string("Hi Player!");
        p.yourClientPlace->socket.send(package);
        package.clear();
        p.yourClientPlace->socket.receive(package);
        std::string s;
        package >> s;
        std::cout<< s;
        package.clear();
    }
    return;
}


bool Server::hostConnectionCheck(void){
    for (serverClient *tmpClient : clientsAvailable){
        if (clientInUse(tmpClient) && tmpClient->key == hostConnectionState.hostId){
            return true;
        }
    }
    return false;
}

void Server::updateTimeOut(timeOutTimer *timer){
    timer->current = std::chrono::high_resolution_clock::now();
    timer->difference = std::chrono::duration_cast<std::chrono::seconds>(timer->current - timer->start);
}

void Server::initTimeOut(timeOutTimer *timer){
    timer->start = std::chrono::high_resolution_clock::now();
}
// Accept a new connection from client to listener (TCP port)
/*
void acceptConnection(sf::TcpListener listener){
    sf::TcpSocket client;
    if (listener.accept(client) != sf::Socket::Done){
        std::cout << "An client's connection attempt failed succesfully." <<  std::endl;
    }
}

void sendPacket(){
    sf::Uint16 x = 10;
    std::string s = "hello";
    double d = 0.6;

    sf::Packet packet;
    packet << x << s << d;
}
*/
bool clientInUse(serverClient *c){
    return (c->active!=false) 
            && (c->id!=USHRT_MAX) 
            && (c->key!="") 
            && (c->threadPtr != nullptr)
            ;
}

void clearClient(serverClient *c){
    c->active = false;
    c->connected = false;
    c->id = USHRT_MAX;
    c->key.clear();
    c->key = "";
    c->threadPtr->terminate();
    c->threadPtr->~Thread();
    c->threadPtr = nullptr;
}
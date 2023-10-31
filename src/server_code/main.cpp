#include "server_networking.hpp"


int main(int argc, char *argv[]){

    std::cout<<"Server process started\n";
    if (argc != 6) {
        std::cout << "Invalid number of parameters!\n";
        system("pause");
        return -1;
    }
    
    unsigned short server_port = static_cast<unsigned short>(std::stoi(argv[1]));
    std::cout << "Port: " << server_port << std::endl;
    
    sf::IpAddress server_adress = sf::IpAddress(argv[2]);
    std::cout << "Adress: " << server_adress.toString() << std::endl;
    
    std::string savePath = argv[3];
    std::cout << "Savepath: " << savePath << std::endl;

    std::string hostId = argv[4];
    std::cout << "HostId: " << hostId << std::endl;

    gameMode modeToLoad = static_cast<gameMode>(std::stoi(argv[5]));
    if (mode_Online(modeToLoad)){
        std::cout << "Playing Online\n";
    }

    Server RunningServer(server_port, server_adress, savePath, hostId, modeToLoad);
    std::cout<<"Server was running und succesfully ended\n";
    system("pause");
    return 0;
}


Server::Server(unsigned short port, sf::IpAddress adress, std::string savePath, std::string hostId, gameMode modeToLoad){
    //SET UP SERVER
    this->hostConnected = false;
    this->serverPort = port;
    this->serverAdress = adress;
    this->mode = modeToLoad;
    if (mode == gameMode::Single){
        this->maxPlayers = 1;
    }
    else {
        this->maxPlayers = 4;
    }

    if (!loadGameSave()){
        std::cout << "Failed to load GameSave\n";
        return;
    }

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

    //Start Accepting New Connections;
    sf::Thread connectingThread(&acceptConnections, this);
    connectingThread.launch();

    
    auto hostTimeOutTimer_start = std::chrono::high_resolution_clock::now();
    // Calculate the elapsed time
    auto hostTimeOutTimer_end = std::chrono::high_resolution_clock::now();
    auto hostTimeOutTimer_diff = std::chrono::duration_cast<std::chrono::seconds>(hostTimeOutTimer_end - hostTimeOutTimer_start);
    bool host_joined = false;
    //SERVER LOOP
    std::cout << "Server ready. Listening on adress: " << serverAdress.toString() << ":" << serverPort <<std::endl;
    bool SERVER_RUNNING = true;
    while (SERVER_RUNNING){

        if (!hostConnected){
            hostTimeOutTimer_end = std::chrono::high_resolution_clock::now();
            hostTimeOutTimer_diff = std::chrono::duration_cast<std::chrono::seconds>(hostTimeOutTimer_end - hostTimeOutTimer_start);

            if (hostTimeOutTimer_diff.count() > 15){
                connectingThread.terminate();
                //CLose other threads as well!
                SERVER_RUNNING = false;
                std::cout << "Host failed to connect/timed out\n";
                break;
            }
        }
        else if (hostConnected && !hostConnectionCheck()){
            std::cout<< "Host disconnected!\n";
            hostTimeOutTimer_start = std::chrono::high_resolution_clock::now();
            hostConnected = false;
        }

        //Update Gamestate
    }

    return;
}



bool Server::loadGameSave(){
    return true;
}



bool Server::setUp_LocalServer(){
    if (serverListener.listen(serverPort) != sf::Socket::Done){
        std::cout << "ERROR CONNECTING PORT AND SOCKET\n";
        return false;
    }
    return true;
}



bool Server::setUp_OnlineServer(){
    if (serverListener.listen(serverPort) != sf::Socket::Done){
        std::cout << "ERROR CONNECTING PORT AND SOCKET\n";
        return false;
    }
    return true;
}


void Server::acceptConnections(void){

    while (true){
        sf::TcpSocket client;
        if (serverListener.accept(client) == sf::Socket::Done) {
            std::cout << "New client connected" << std::endl;
        // Start a new thread to handle the client
        //std::thread clientThread(HandleClient, std::move(client));
        //clientThread.detach(); // Detach the thread to run independently
        }
    }
}


bool Server::hostConnectionCheck(void){
    return false;
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

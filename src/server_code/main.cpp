#include "server_networking.hpp"


int main(int argc, char *argv[]){
    std::cout<<"Server process started\n";
    std::cout << argc << std::endl;
    for (int i = 0; i < argc; i++){
        std::cout << argv[i] << std::endl;
    }
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " <string1> <unsigned short> <string2>" << std::endl;
        system("pause");
        return -1;
    }
    
    unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));
    sf::IpAddress adress = sf::IpAddress(argv[2]);
    std::string savePath = argv[3];
    std::string hostId = argv[4];
    std::cout << "Adress: " << adress.toString() << std::endl;
    std::cout << "Port: " << port << std::endl;
    std::cout << "Savepath: " << savePath << std::endl;
    std::cout << "HostId: " << hostId << std::endl;

    Server RunningServer(port, adress, savePath, hostId);
    std::cout<<"Server was running und succesfully ended\n";
    system("pause");
    return 0;
}


Server::Server(unsigned short port, sf::IpAddress adress, std::string savePath, std::string hostId){
    //SET UP SERVER
    this->serverPort = port;
    this->serverAdress = adress;

    if (serverSocket.listen(serverPort) != sf::Socket::Done){
        std::cout << "ERROR CONNECTING PORT AND SOCKET\n";
        return;
    }
    //LOAD GAMESAVE


    auto start_time = std::chrono::high_resolution_clock::now();
    // Calculate the elapsed time
    auto end_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
    bool host_joined = false;
    //SERVER LOOP
    std::cout << "Server ready. Listening on adress: " << serverAdress.toString() << ":" << serverPort <<std::endl;
    bool SERVER_RUNNING = true;
    while (SERVER_RUNNING){
        sf::TcpSocket client;
        if (serverSocket.accept(client) == sf::Socket::Done) {
            std::cout << "New client connected" << std::endl;

            // Start a new thread to handle the client
            //std::thread clientThread(HandleClient, std::move(client));
            //clientThread.detach(); // Detach the thread to run independently
        }
        SERVER_RUNNING = false;
    }

    return;
}

//// Server side
// Start a TCP server
sf::IpAddress startServer(int port = 5426, bool online = false){
    sf::TcpListener listener;
    if (listener.listen(port) != sf::Socket::Done){
        std::cout << "The Server initializen on port " << port << "failed succesfully." <<  std::endl;
    }
    // Get IP address
    sf::IpAddress serverIP;
    if (online){
        serverIP = sf::IpAddress::getPublicAddress();
    }

    else {
        serverIP = sf::IpAddress::getLocalAddress();    
    }

    std::cout << "Please share your IP-Address with fellow whores! It is " << serverIP <<  std::endl;

    return serverIP;
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

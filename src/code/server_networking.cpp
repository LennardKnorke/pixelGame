#include "server_networking.hpp"

void server_thread_function(unsigned short port, sf::IpAddress adress, std::string savePath){
    Server RunningServer(port, adress, savePath);
}

int main(void){
    server_thread_function(0, sf::IpAddress("192.168.2.199"), "");
}


Server::Server(unsigned short port, sf::IpAddress adress, std::string savePath){
    //SET UP SERVER
    this->serverPort = port;
    this->serverAdress = adress;
    
    if (serverSocket.listen(serverPort) != sf::Socket::Done){
        std::cout << "ERROR CONNECTING PORT AND SOCKET\n";
        return;
    }
    //LOAD GAMESAVE



    //SERVER LOOP
    std::cout << "Server ready. Listening on adress: " << serverAdress.toString() << ":" << serverPort <<std::endl;
    bool SERVER_RUNNING = false;
    while (SERVER_RUNNING){



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



int main (int arg, char *argv){

}
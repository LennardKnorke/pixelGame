#include <SFML/Network.hpp>
#include <iostream>

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

//// Client side
void startClient(const char* serverIP){
    //instead of serverIP as input, box that asks for IP address 
    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect(serverIP, 5426);
    if (status != sf::Socket::Done)
    {
        // error...
    }

}
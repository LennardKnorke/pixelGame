#include "client.hpp"

bool Clients::connect(sf::IpAddress hostIp, unsigned short hostPort){
    //Connect with host
    std::cout << "Attempting to connect to: " << hostIp.toString() << " : " << hostPort << std::endl;
    if (socket.connect(hostIp, hostPort) == sf::Socket::Done){
        std::cout << "Connected to: " << hostIp << ":" << hostPort<<std::endl;
        socket.setBlocking(false);
        return true;
    }
    
    std::cout << "Failed to connect with host\n";
    return false;
};



bool Clients::sendData(sf::Packet &OutGoingPacket){
    if (socket.send(OutGoingPacket) == sf::Socket::Done){
        return true;
    }
    std::cout << "Failed to send data!\n";
    return false;
};



bool Clients::receiveData(sf::Packet &incomingDestination){
    if (socket.receive(incomingDestination) == sf::Socket::Done){
        return true;
    }
    std::cout<<"Failed to receive data\n";
    return false;
};



void Clients::disconnect(void){
    socket.disconnect();
}

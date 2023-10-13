#include "client.hpp"

//CLIENT (PARENT) CLASS
unsigned short Clients::findFreePort(void){
    sf::TcpListener listener;
    unsigned short port_tmp = 1024;
    while (port_tmp <= USHRT_MAX){
        if (listener.listen(port_tmp) == sf::Socket::Done){
            listener.close();
            return port_tmp;
        }
        port_tmp++;
    }
    listener.close();
    return 0;
}



bool Clients::connect(sf::IpAddress hostIp, unsigned short hostPort){
    port = findFreePort();
    //Found no free port
    if (port == 0){
        std::cout << "No free port for client socket\n";
        return false;
    }
    //Connect with host
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

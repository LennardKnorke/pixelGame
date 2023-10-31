#include "stdlibs.hpp"
#include <steam/Isteamnetworkingsockets.h>
#include <steam/steamnetworkingtypes.h>
union sockUnion {
    sf::TcpSocket *localSocket;
    ISteamNetworkingSockets *publicSock;
};

typedef struct socket_instance{
    sockUnion sock;
    bool beHost;

}socket_instance;

int main (int argc, char *argv[]){
    const SteamNetworkingIdentity *pIdentity;
    SteamNetworkingErrMsg msg("wassup");
    ISteamNetworkingSockets *soc;
    if (false){
        std::cout << "Failed to init GameNetworkinSockets.\n";
        return -1;
    }
    
    if (argc != 2){
        return 1;
    }
    unsigned short loadAs = std::stoi(argv[1]);
    if (loadAs != 0 && loadAs != 1){
        std::cout << loadAs << std::endl;
        return -1;
    }
    socket_instance s;
    if (loadAs == 0){
        s.beHost = false;
    }
    else {
        s.beHost = true;
    }

    return 0;
}
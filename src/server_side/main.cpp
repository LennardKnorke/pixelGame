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
    else {
        std::cout << "Playing Local/Alone\n";
    
    }
    Server RunningServer(server_port, server_adress, savePath, hostId, modeToLoad);
    std::cout<<"Server was running und succesfully ended\n";
    system("pause");
    return 0;
}




#include "server.hpp"


int main(int argc, char *argv[]){
    // Starting server process
    // Reading arguments
    unsigned short assigned_port;
    sf::IpAddress ip;
    std::string path_to_save;
    short max_players;
    bool dev_mode;

    // Error checking. Specific number of arguments required
    if (argc != 6 && argc != 5){
        std::cout << "Invalid set up of server\n";
        std::cout << argc << std::endl;
        for (auto i = 0; i < argc; i++){
            std::cout << argv[1] << std::endl;
        }
        return 1;
    }
    // Read adress
    assigned_port = atoi(argv[1]);
    ip = sf::IpAddress(std::string(argv[2]));
    if (ip == sf::IpAddress::None){
        std::cout << "Invalid IP address\n";
        return 1;
    }
    // Read path of save
    path_to_save = std::string(argv[3]);

    // Read max players
    if (std::isdigit(atoi(argv[4]))){
        std::cout << "Invalid number of players\n";
        return 1;
    }
    max_players = atoi(argv[4]);

    // Read dev mode
    if (argc == 6) {
        if (std::isdigit(argv[5][0])) {
            dev_mode = (atoi(argv[5]) == 1);
        } else {
            std::cout << "Invalid dev mode\n";
            return 1;
        }
    } else {
        dev_mode = false;
    }

    // Running Server
    Server server(assigned_port, ip, path_to_save, max_players, dev_mode);
    if (!server.run()){
        system("pause");
        return 1;
    }


    return 0;
}
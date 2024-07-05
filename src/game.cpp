#include "application.hpp"
#include "game.hpp"

GAME_STATE Application::gameLoop(void){
    Game *game;

    if (mode == gameMode::Single || mode == gameMode::Host){
        game = new Game(mode, &window, cursor, loadingTools.chosenSave);
    }
    else if (mode == gameMode::Join){
        game = new Game(mode, &window, cursor, loadingTools.ip, loadingTools.port);
    }
    else {
        std::cerr << "ERROR: Invalid game mode" << std::endl;
        return GAME_STATE::QUIT;
    }

    GAME_STATE next_state = game->run();


    delete game;
    return next_state;
}

// Constructor for playing singleplayer or hosting game
Game::Game(gameMode m, sf::RenderWindow *win, Cursor *cur, gamesave_summary save){
    // Constructor. save all parameters given
    mode = m;
    info = save;
    window = win;
    cursor = cur;

    if (mode == gameMode::Host){
        network_tools_c.host_ip = sf::IpAddress::getLocalAddress();
        network_tools_c.host_port = findFreePort();
        std::cout << "Hosting on " << network_tools_c.host_ip << ":" << network_tools_c.host_port << std::endl;
    }
}

// Constructor for joining a game
Game::Game(gameMode m, sf::RenderWindow *win, Cursor *cur, sf::IpAddress ip, unsigned short port){
    mode = m;
    window = win;
    cursor = cur;

    network_tools_c.host_ip = ip;
    network_tools_c.host_port = port;

}


GAME_STATE Game::run(void){
    // If hosting or joining, set up sockets for communication
    
    if (mode == gameMode::Host){
        sf::TcpSocket socket;
        if (network_tools_c.host_port == 0){
            std::cerr << "ERROR: Could not find free port" << std::endl;
            return GAME_STATE::QUIT;
        }
        hostingTools.listener.listen(network_tools_c.host_port);
        hostingTools.listener.accept(socket);

        std::string message = "Hello from server\n";
        sf::Packet packet;
        packet << message;
        socket.send(packet);
        packet.clear();
        socket.receive(packet);
        packet >> message;
        std::cout << message << std::endl;
        socket.disconnect();
    }
    else if (mode == gameMode::Join){
        network_tools_c.socket.connect(network_tools_c.host_ip, network_tools_c.host_port);
        
        std::string message = "Hello from client\n";
        sf::Packet packet;
        packet << message;
        network_tools_c.socket.send(packet);
        packet.clear();
        network_tools_c.socket.receive(packet);
        packet >> message;
        std::cout << message << std::endl;
        network_tools_c.socket.disconnect();
    }
    // If hosting or single player, load the save file
    // else if joining, wait for the host to send the save file

    // Main game loop
        //draw

        //handle input by user

        //if hosting or joining, communicate with other players

        //update game state

    // End of loop
    // Cut off communication if hosting or joining
    // Save the game if hosting or single player

    // Return to main menu, exit, and mark possible errors
    return GAME_STATE::QUIT;
}

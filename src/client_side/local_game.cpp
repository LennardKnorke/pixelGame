#include "application.hpp"
#include "local_game.hpp"
#include "client.hpp"

//Application main function
GAME_STATE Application::gameLoop(void){

    // Set up server process (IF HOSTING)
    if (mode_Host(mode)){
        if (mode_Online(mode)){
            hostAdress.ip = machinePublicAdress;
        }
        else {
            hostAdress.ip = machineLocalAdress;
        }
        if (initServerProcess(&hostAdress.ip, hostAdress.port, localUserID, hostAdress.pathSave, mode)){
            std::cout << "Host process started\n";
        }
        else {
            std::cout << "Failed to initiate server process\n";
            resetHostInfo();
            return GAME_STATE::MENU;
        }
    }  

    // Run Game
    GameClass *game = new GameClass(this);
    if (game->loadAndConnect()){
        GAME_STATE nextAppState = game->runGame();
        resetHostInfo();
        return nextAppState;
    }
    else {
        delete game;
        resetHostInfo();
        return GAME_STATE::MENU;
    }

}



GameClass::GameClass(Application *appPointer){

    //Set up pointers + variables
    window = &(appPointer->window);
    backgroundMusic = appPointer->backgroundMusic;

    gameState = gameLoopState::Game;

    playerID = appPointer->localUserID;
    cursor = &(appPointer->cursor);

    connection_status = connectionStatus::disconnected;
    host.ip = appPointer->hostAdress.ip;
    host.port = appPointer->hostAdress.port;
    //Set up menu buttons
    for (int i  = 0; i < n_menugameButtons; i++){
        menuButtons.push_back(new inGameMenuButton(i, appPointer));
    }

    //Set up music
    backgroundMusic[musicIdx::mainMenu].stop();
    backgroundMusic[musicIdx::game_main].play();

    // set up a connecting thread
    networkThread = new sf::Thread(&GameClass::connectToHost, this);
}

GameClass::~GameClass(void){
    // Send quit command to server
    sf::Packet p;
    p << sf::Uint8(gameLoopState::QuitGame);
    socket.send(p);


    backgroundMusic[musicIdx::game_main].stop();
    //Delete helpful stuff
    for (inGameMenuButton* tmp: menuButtons){
        delete tmp;
    }

};


//displays the loading screen while waiting for the client/host to connect
bool GameClass::loadAndConnect(void){
    // start the networking thead
    connection_status = connectionStatus::connecting;
    networkThread->launch();


    // Display loading screen
    
    while (connection_status == connectionStatus::connecting){
        window->clear(sf::Color::Yellow);

        window->display();
    }

    if (connection_status == connectionStatus::disconnected){
        std::cout << "Failed to connect to host\n";
        return false;
    }
    return true;
}

void GameClass::connectToHost(void){
    for (int i = 0; i < 5; i++){
        std::cout << "Attempting to connect with host no: " << i+1 <<std::endl;
        if (socket.connect(host.ip, host.port) == sf::Socket::Done){
            sf::Packet p;
            p << playerID;
            socket.send(p);
            connection_status = connectionStatus::connected;
            return;
        }
        auto start_time = std::chrono::high_resolution_clock::now();
        // Calculate the elapsed time
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
        while (elapsed_time.count() < 5){
            end_time = std::chrono::high_resolution_clock::now();
            elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);  
        }

    }
    connection_status = connectionStatus::disconnected;
    return;  
};

GAME_STATE GameClass::runGame(void){

    while (gameState != gameLoopState::QuitGame && gameState != gameLoopState::QuitMenu)
    {
        // Draw
        draw();
        
        // Register Input
        if (gameState == gameLoopState::Game){
            userInput_game();
        }
        else if (gameState == gameLoopState::SkillTree){
            userInput_skilltree();
        }
        else if (gameState == gameLoopState::Pause){
            userInput_pause();
        }
        else {
            std::cout << "Invalid game state!\n";
            return GAME_STATE::QUIT;
        }
        cursor->update();

        // Exchange with server
        sf::Packet responsePacket = send_receive();

        // Extract game state from package/ update information
        if (gameState == gameLoopState::Game){
            update_game();
        }
        else if (gameState == gameLoopState::SkillTree){
            update_skilltree();
        }
        else if (gameState == gameLoopState::Pause){
            update_pause();
        }
    }
    //GAMELOOP STOPS HERE
    if (gameState == gameLoopState::QuitMenu){
        return GAME_STATE::MENU;
    }
    return GAME_STATE::QUIT;
};



void GameClass::draw(void){
    
    if (gameState == gameLoopState::Game){
        draw_game();
    }
    else if (gameState == gameLoopState::SkillTree){
        draw_skillTree();
    }
    else if (gameState == gameLoopState::Pause){
        draw_pause();
    }
    cursor->draw(*window);
    window->display();
}

void GameClass::draw_skillTree(void){
    window->clear(sf::Color::Blue);
}


void GameClass::draw_pause(void){
    window->clear(sf::Color::Green);
    for (inGameMenuButton* tmp : menuButtons){
        tmp->draw(window);
    }
}

void GameClass::draw_game(void){
    window->clear(sf::Color::Red);
}

void GameClass::userInput_game(void){
    sf::Event ev;
    while (window->pollEvent(ev))
    {
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape){
            gameState = gameLoopState::Pause;
        }
    }
}

void GameClass::userInput_skilltree(void){
    sf::Event ev;
    while (window->pollEvent(ev)){
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape){
            gameState = gameLoopState::Pause;
        }
    }
}

void GameClass::userInput_pause(void){
    sf::Event ev;
    while (window->pollEvent(ev)){
        if (ev.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            gameState = gameLoopState::Game;
        }

        else if (ev.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
            for (inGameMenuButton* tmp : menuButtons){
                if (tmp->focus){
                    gameState = tmp->followUpState;
                    break;
                }
            }
        }
    }
}

sf::Packet GameClass::send_receive(){
    sf::Packet package;
    // Add game_state to package and send to server
    package << sf::Uint8(gameState);
    // Add in for relevant gamestates
    socket.send(package);
    package.clear();
    // Receive package from server
    socket.receive(package);
    return package;
}


void GameClass::update_game(void){
    
}

void GameClass::update_skilltree(void){
    
}

void GameClass::update_pause(void){
    for (inGameMenuButton *tmp : menuButtons){
        tmp->update(cursor->returnPosition());
    }
}

bool initServerProcess(sf::IpAddress *adress, unsigned short &port, std::string HostId, std::string pathToSave, gameMode Mode){
    if (!setHostPort(port, adress, Mode)){
        return false;
    }

    std::string command = "start Server.exe " 
                            + std::to_string(port) + " "
                            + adress->toString() + " " 
                            + pathToSave + " " 
                            + HostId + " "
                            + std::to_string(Mode)
                            ;
    std::cout << command <<std::endl;

    if (std::system(command.c_str()) != 0){
        std::cout << "Failed to run server command\n";
        return false;
    }
    return true;
}

bool setHostPort(unsigned short &port, sf::IpAddress *adress, gameMode Mode){
    unsigned short tmp = 1024;
    if (mode_Online(Mode)){
        tmp = 54000;
    }
    sf::TcpListener listener;
    while (tmp <= USHRT_MAX)
    {
        if (listener.listen(tmp) == sf::Socket::Done){
            port = tmp;
            listener.close();
            return true;
        }
        tmp++;
    }
    listener.close();

    //reset adress and port
    port = 0;
    *adress = sf::IpAddress::None;
    return false;
}


//////////////
//inGameMenuButton
//////////////
inGameMenuButton::inGameMenuButton(int currentIdx, Application *app){
    if (currentIdx == 0){
        textString = "Return to Menu";
        followUpState = gameLoopState::QuitMenu;
    }
    else if (currentIdx == 1){
        textString = "Open Skilltree";
        followUpState = gameLoopState::SkillTree;
    }
    else if (currentIdx == 2){
        textString = "Exit Game";
        followUpState = gameLoopState::QuitGame;
    }
    focus = false;
    text.setString(textString);
    text.setFont(app->gameFont);
    text.setCharacterSize(GAMEFONT_SIZE);
    text.setFillColor(sf::Color::White);
    text.setPosition((app->resolution.x/2.0) - (text.getGlobalBounds().getSize().x/2.0), ((app->resolution.y/(n_menugameButtons + 1)) * (currentIdx + 1)) - (text.getGlobalBounds().getSize().y/2));
}



inGameMenuButton::~inGameMenuButton(void){};



void inGameMenuButton::draw(sf::RenderWindow *window){
    window->draw(text);
}



void inGameMenuButton::update(sf::Vector2f mousePos){
    if (mousePos.x > text.getPosition().x && mousePos.x < text.getPosition().x + text.getGlobalBounds().getSize().x
    && mousePos.y > text.getPosition().y && mousePos.y < text.getPosition().y + text.getGlobalBounds().getSize().y){
        if (!focus){
            focus = true;
            text.setStyle(sf::Text::Style::Underlined);
            text.setFillColor(sf::Color::Red);
        }
    }    
    else {
        if (focus){
            focus = false;
            text.setStyle(sf::Text::Style::Regular);
            text.setFillColor(sf::Color::White);
        }
    }
}




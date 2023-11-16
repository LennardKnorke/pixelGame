#include "application.hpp"
#include "local_game.hpp"
#include "client.hpp"
GAME_STATE Application::gameLoop(void){
    //"Global variables for this loop"
    gameLoopState state = gameLoopState::Game;
    Clients ClientSocket;
    playerControl gamePlayInput;
    std::vector<inGameMenuButton*> menuButtons;
    sf::Packet communicationPacket;


    if (mode_Host(mode)){
        if (initServerProcess(&hostAdress.ip, hostAdress.port, localUserID, hostAdress.pathSave, mode)){
            std::cout << "Process started\n";
        }
        else {
            std::cout << "Failed to initiate server process\n";
            state = gameLoopState::QuitMenu;
        }
    }    
    

    //loading screen... wait for connection with host
    if (state == gameLoopState::Game){
        for (int i  = 0; i < n_menugameButtons; i++){
            menuButtons.push_back(new inGameMenuButton(i, this));
        }
        state = loadingScreen(ClientSocket);
    }//return menu with error if timeout
    
    
    //GAME LOOP STARTS HERE
    while (state != gameLoopState::QuitGame && state != gameLoopState::QuitMenu){
        if (state == gameLoopState::Game){

            drawGame();
            registerGameInput(state, gamePlayInput);
            updateGame(ClientSocket, communicationPacket);// will exchange data with host/server/savethread
        }
        else if(state == gameLoopState::SkillTree){
            drawTree();
            registerTreeInput(&state);
            updateTree();
        }
        else if (state == gameLoopState::Menu){
            drawMenu(menuButtons);
            registerMenuInput(&state, menuButtons);
            updateMenu(menuButtons);//quit, quit to menu, to skill tree, see information, send character updates to host
        }
        cursor.update();
        cursor.draw(window);
        window.display();
    }
    //GAMELOOP STOPS HERE
    

    ClientSocket.disconnect();
    //Delete helpful stuff
    for (inGameMenuButton* tmp: menuButtons){
        delete tmp;
    }

    //Reset host values
    hostAdress.ip = sf::IpAddress::None;
    hostAdress.port = 0;
    hostAdress.pathSave = "";

    if (state == gameLoopState::QuitMenu){
        return GAME_STATE::MENU;
    }
    return GAME_STATE::QUIT;
}



bool initServerProcess(sf::IpAddress *adress, unsigned short &port, std::string HostId, std::string pathToSave, gameMode Mode){

    if (!setHostIp(adress, Mode)){
        return false;
    }
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
        *adress = sf::IpAddress::None;
        port = 0;
        return false;
    }
    return true;
}

bool setHostIp(sf::IpAddress *adress, gameMode Mode){
    if (mode_Online(Mode)){
        if (!fileExists("multiplayer.txt")){
            std::cout << "'multiplayer.txt' missing\n";
            return false;
        }
        //*adress = sf::IpAddress::getPublicAddress();
        std::ifstream inputFile("multiplayer.txt");
        std::string hamachiIp;
        std::getline(inputFile, hamachiIp);
        inputFile.close();
        std::cout << "Hamachi Ip: " << sf::IpAddress(hamachiIp).toString() << std::endl;
        *adress = sf::IpAddress(hamachiIp);
    }
    else {
        *adress = sf::IpAddress::getLocalAddress();
    }

    if (*adress == sf::IpAddress::None){
        std::cout << "Invalid IpAdress!\n";
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


//displays the loading screen while waiting for the client/host to connect
gameLoopState Application::loadingScreen(Clients &socket){
    
    window.clear(sf::Color::Black);
    window.display();
    
    for (int i = 0; i < 4; i++){
        std::cout << "Attempting to connect with host no: " << i+1 <<std::endl;
        if (socket.connect(hostAdress.ip, hostAdress.port)){
            return gameLoopState::Game; 
        }

        
        auto start_time = std::chrono::high_resolution_clock::now();
        // Calculate the elapsed time
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
        while (elapsed_time.count() < 1){
            end_time = std::chrono::high_resolution_clock::now();
            elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);  
        }
        
    }
    return gameLoopState::QuitMenu;    
}



void Application::readUserGameInput(playerControl &controlInput){
    //Read key input
    for (int i =0; i < n_keyInputOptions; i++){
        if (inGameControls[i].iType == inputType::KEYBOARD){
            controlInput.keyInput[i] = sf::Keyboard::isKeyPressed(inGameControls[i].input.keyInput);
        }
        else if (inGameControls[i].iType == inputType::MOUSE_BUTTON){
            controlInput.keyInput[i] = sf::Mouse::isButtonPressed(inGameControls[i].input.mouseInput);
        }
    }
}



void Application::drawGame(void){
    window.clear(sf::Color::Red);
}



void Application::registerGameInput(gameLoopState &s, playerControl &controlInput){
    sf::Event ev;
    while (window.pollEvent(ev)){
            if (ev.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            s = gameLoopState::Menu; 
        }
        else if (ev.type == sf::Event::KeyPressed || ev.type == sf::Event::MouseButtonPressed){
            readUserGameInput(controlInput);
        }
    }
}



void Application::updateGame(Clients &socket, sf::Packet &packet){
    packet << localUserID;
    socket.sendData(packet);
    packet.clear();
    socket.receiveData(packet);
    packet.clear();
}



void Application::drawMenu(std::vector<inGameMenuButton*> v){
    window.clear(sf::Color::Black);
    for (inGameMenuButton* tmp : v){
        tmp->draw(&window);
    }
}



void Application::registerMenuInput(gameLoopState *s, std::vector<inGameMenuButton*> v){
    sf::Event ev;
    while (window.pollEvent(ev)){
        if (ev.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            *s = gameLoopState::Game;
        }
        if (ev.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
            for (inGameMenuButton* tmp : v){
                if (tmp->focus){
                    *s = tmp->followUpState;
                }
            }
        }
    }
}



void Application::updateMenu(std::vector<inGameMenuButton*> v){
    for (inGameMenuButton* tmp : v){
        tmp->update(cursor.returnPosition());
    }
}
//
void Application::drawTree(void){
    window.clear(sf::Color::Yellow);
}



void Application::registerTreeInput(gameLoopState *s){
    sf::Event ev;
    while (window.pollEvent(ev)){
        if (ev.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            *s = gameLoopState::Menu;
        }
    }
}



void Application::updateTree(void){
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
    text.setCharacterSize(30/app->ratioScaling);
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




#include "application.hpp"
#include "local_game.hpp"
#include "client.hpp"
GAME_STATE Application::gameLoop(void){
    gameLoopState state = gameLoopState::Game;
    Clients ClientSocket;
    std::cout << mode << std::endl;
    if (mode == gameMode::Local_host || mode == gameMode::Online_host || mode == gameMode::Single){
        if (setHostAddress(mode, &hostAdress.ip, &hostAdress.port)){
            const char* path = "./Server.exe";
            std::string port_string = std::to_string(hostAdress.port);
            std::string command = "start Server.exe " 
                                + port_string + " " 
                                + hostAdress.ip.toString() + " "
                                + hostAdress.pathSave + " "
                                + localUserID + " "
                                + std::to_string(mode);
                                ;
            std::cout << command <<std::endl;
            if (std::system(command.c_str()) != 0){
                std::cout << "Failed to initalize server thread\n";
                state = gameLoopState::QuitMenu;
            }
            else {
                std::cout << "Initialized command\n";
            }
        }
        else {
            std::cout <<"Failed to set ip/port for server\n";
            state = gameLoopState::QuitMenu;
        }
    }
    

    //ClientSocket.connect(hostAdress.ip, hostAdress.port);
    
    bool gamePlayInput[7] = {false, false, false, false, false, false, false};
    std::vector<inGameMenuButton*> menuButtons;
    sf::Packet communicationPacket;
    //loading screen... wait for connection with host
    if (state == gameLoopState::Game){
        for (int i  = 0; i < n_menugameButtons; i++){
            menuButtons.push_back(new inGameMenuButton(i, this));
        }
        state = loadingScreen(&ClientSocket);
    }//return menu with error if timeout
    
    
    while (state != gameLoopState::QuitGame && state != gameLoopState::QuitMenu){
        if (state == gameLoopState::Game){
           drawGame();
           registerGameInput(&state, gamePlayInput);
           updateGame();// will exchange data with host/server/savethread
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
        cursor.draw(&window);
        window.display();
    }
    
    //Delete helpful stuff
    for (inGameMenuButton* tmp: menuButtons){
        delete tmp;
    }
    //at the end. dereference the current game, preparing for either ending game or going to the menu

    ClientSocket.disconnect();
    if (state == gameLoopState::QuitMenu){
        return GAME_STATE::MENU;
    }
    return GAME_STATE::QUIT;
}



bool setHostAddress(gameMode mode, sf::IpAddress *ip, unsigned short *p){
    if (mode == gameMode::Online_host){
        std::cout << "Online!\n";
        *ip = sf::IpAddress::getPublicAddress();
    }
    else {
        std::cout << "Local or Single!\n";
        *ip = sf::IpAddress::getLocalAddress();
    }
    unsigned short tmp = 1024;
    sf::TcpListener listener;
    while (tmp <= USHRT_MAX)
    {
        if (listener.listen(tmp) == sf::Socket::Done){
            *p = tmp;
            listener.close();
            return true;
        }
        tmp++;
    }
    listener.close();
    *p = 0;
    *ip = sf::IpAddress::None;
    return false;
}



//displays the loading screen while waiting for the client/host to connect
gameLoopState Application::loadingScreen(Clients *socket){
    
    window.clear(sf::Color::Black);
    window.display();
    
    for (int i = 0; i < 4; i++){
        std::cout << "Attempting to connect with host no: " << i+1 <<std::endl;
        if (socket->connect(hostAdress.ip, hostAdress.port)){
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

    //everything alright? then the game can start
    
}



void Application::readUserGameInput(sf::Vector2i origin, bool *gamePlayInput){
    //Read key input
    for (int i =0; i < 7; i++){
        if (inGameControls[i].iType == inputType::KEYBOARD){
            gamePlayInput[i] = sf::Keyboard::isKeyPressed(inGameControls[i].input.keyInput);
        }
        else if (inGameControls[i].iType == inputType::MOUSE_BUTTON){
            gamePlayInput[i] = sf::Mouse::isButtonPressed(inGameControls[i].input.mouseInput);
        }
    }
}



void Application::drawGame(void){
    window.clear(sf::Color::Red);
}



void Application::registerGameInput(gameLoopState *s, bool *gamePlayInput){
    sf::Event ev;
    while (window.pollEvent(ev)){
        if (ev.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            *s = gameLoopState::Menu; 
        }
        else if (ev.type == sf::Event::KeyPressed || ev.type == sf::Event::MouseButtonPressed){
            readUserGameInput(sf::Vector2i(resolution.x/2, resolution.y/2), gamePlayInput);
        }
    }
}



void Application::updateGame(void){

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
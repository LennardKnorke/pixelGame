#include "application.hpp"
#include "local_game_client.hpp"

GAME_STATE Application::gameLoop(void){

    
    if (mode == gameMode::Local  && wantsHost){
        if (wantsHost){
            //Start a new threat/process for the host server
        }
        //Set up client socket
    }
    else if (mode == gameMode::Online){//joining local or online
        if (wantsHost){
            //Start a new threat/process for the host server
        }
        //Set up client socket
    }
    else if (mode == gameMode::Single){
        //Start a new process to
    }
    
    


    //loading screen... wait for connection with host
    gameLoopState state = loadingScreen();
        //return menu with error if timeout


    //variables to display the menu
    std::vector<inGameMenuButton*> menuButtons;
    for (int i  = 0; i < n_menugameButtons; i++){
        menuButtons.push_back(new inGameMenuButton(i, this));
    }


    while (state != gameLoopState::QuitGame && state != gameLoopState::QuitMenu){    
        if (state == gameLoopState::Game){
           drawGame();
           registerGameInput(&state);
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
    delete activeSave;

    if (state == gameLoopState::QuitMenu){
        return GAME_STATE::MENU;
    }
    return GAME_STATE::QUIT;
}



//displays the loading screen while waiting for the client/host to connect
gameLoopState Application::loadingScreen(void){
    
    window.clear(sf::Color::Black);
    window.display();
    //setUp singleplayer
    if (mode == gameMode::Single){
        
    }

    //everything alright? then the game can start
    return gameLoopState::Game;
}



void Application::readUserGameInput(sf::Vector2i origin){
    //Read key input
    gamePlayInput.Jump = sf::Keyboard::isKeyPressed(inGameControls.Jump);
    gamePlayInput.Down = sf::Keyboard::isKeyPressed(inGameControls.Down);
    gamePlayInput.Left = sf::Keyboard::isKeyPressed(inGameControls.Left);
    gamePlayInput.Right = sf::Keyboard::isKeyPressed(inGameControls.Right);
    gamePlayInput.nextItem = sf::Keyboard::isKeyPressed(inGameControls.nextItem);
    gamePlayInput.prevItem = sf::Keyboard::isKeyPressed(inGameControls.prevItem);
    //read attack input
    gamePlayInput.attack = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    //calculate aim
    gamePlayInput.aim = sf::Mouse::getPosition() - origin;
}



void Application::drawGame(void){
    window.clear(sf::Color::Red);
}



void Application::registerGameInput(gameLoopState *s){
    sf::Event ev;
    while (window.pollEvent(ev)){
        if (ev.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            *s = gameLoopState::Menu; 
        }
        else if (ev.type == sf::Event::KeyPressed || ev.type == sf::Event::MouseButtonPressed){
            readUserGameInput(sf::Vector2i(resolution.x/2, resolution.y/2));
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
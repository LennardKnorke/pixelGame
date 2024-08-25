#include "application.hpp"
#include "game.hpp"

GAME_STATE Application::gameLoop(void){
    Game *game;
    if (mode == gameMode::Single || mode == gameMode::Host){
        game = new Host_Game(mode, &window, cursor, DEV_MODE, settings, &assets, loadingTools.chosenSave);
    }
    else if (mode == gameMode::Join){
        game = new Join_Game(mode, &window, cursor, DEV_MODE, settings, &assets, loadingTools.ip, loadingTools.port);
    }
    else {
        std::cerr << "ERROR: Invalid game mode" << std::endl;
        return GAME_STATE::QUIT;
    }

    // END OF GAME LOOP.
    // Go to menu
    if (game->gameState == GameStates::MENU_){
        delete game;
        return GAME_STATE::MENU;
    }
    //Or quit application
    //(optional with error)
    if (game->gameState == GameStates::QUIT_ERROR){
        error = Error;
    }
    delete game;
    return GAME_STATE::QUIT;
}


game_save::game_save(gamesave_summary *s){
    // Constructor. save all parameters given
    summary = *s;
    if (summary.initialized){

    }
}
game_save::game_save(){
    // Constructor. save all parameters given
    seed = 0;
}



// Constructor for playing singleplayer or hosting game
Game::Game(gameMode m, sf::RenderWindow *win, Cursor *cur, bool dev, settings_class *ps, asset_manager *am){
    // Constructor. save all parameters given
    this->mode = m;
    this->window = win;
    
    this->cursor = cur;
    this->settings = ps;
    this->DEV_MODE = dev;
    this->assets = am;

    this->loading_done = false;
    //win->setActive(true);
    this->loadingThread = new sf::Thread(&Game::loadingScreen_t, this);
    if (DEV_MODE){
        std::cout << "INIT GAME: " << std::endl;
        std::cout << "\tGame mode: " << m << std::endl;
        std::cout << "\tWindow size: " << window->getSize().x << "x" << window->getSize().y << std::endl;
        std::cout << "\tResolution: " << settings->res.width << "x" << settings->res.height << std::endl;
        std::cout << "\tScale: " << settings->res.scaling.x << "x" << settings->res.scaling.y << std::endl;

    }
}

void Game::loadingScreen_t(void){
    window->setActive(true);

    sf::Text loadingText;
    loadingText.setString("Loading...");
    loadingText.setFont(assets->gameFont);
    loadingText.setCharacterSize(GAMEFONT_SIZE * settings->res.scaling.x);
    loadingText.setFillColor(sf::Color::Red);
    loadingText.setPosition(window->getSize().x / 2 - loadingText.getGlobalBounds().width / 2, window->getSize().y / 2 - loadingText.getGlobalBounds().height / 2);


    while (!loading_done){
        // Draw loading screen
        window->clear(sf::Color::Black);
        window->draw(loadingText);
        window->display();
    }
    window->setActive(false);
}


Join_Game::Join_Game(gameMode m, sf::RenderWindow *win, Cursor *cur, bool dev, settings_class *ps, asset_manager *am, sf::IpAddress ip, unsigned short port) 
: Game(m, win, cur, dev, ps, am){
    // Constructor. save all parameters given
    h_ip = ip;
    h_port = port;

    if (DEV_MODE){
        std::cout << "INIT JOIN GAME: " << std::endl;
        std::cout << "\tHost Ip " << h_ip << std::endl;
        std::cout << "\tHost Port" << h_port << std::endl;
    }

    // LOADING THREAD. Connect with host and load game from host
    loadingThread->launch();

    loading_done = true;
    loadingThread->wait();
    // END OF LOADING THREAD
    gameState = QUIT_NOERROR;
}

Host_Game::Host_Game(gameMode m, sf::RenderWindow *win, Cursor *cur, bool dev, settings_class *ps, asset_manager *am, gamesave_summary s)
: Game(m, win, cur, dev, ps, am){
    // Constructor. save all parameters given
    this->save = new game_save(&s);
    if (DEV_MODE){
        std::cout << "INIT HOST GAME: " << std::endl;
        std::cout << "\t" << save->summary.path << "\t";
        std::cout << "\t" << save->summary.name << "\t";
        std::cout << "\t" << save->summary.initialized << std::endl;
    }
    
    // LOADING THREAD. Load game from save
    loadingThread->launch();
    // Host specific loading
    if (mode == gameMode::Host){
        port = findFreePort();
        if (port == 0){
            std::cerr << "ERROR: Failed to find free port" << std::endl;
            gameState = QUIT_ERROR;
        }
        else {
            ip = sf::IpAddress::getLocalAddress();

        }
        listener.listen(port, ip);
    }
    loading_done = true;
    loadingThread->wait();
    // END OF LOADING THREAD






    // END OF GAME APPLICATION
    if (mode == gameMode::Host){
        listener.close();
    }
    gameState = QUIT_NOERROR;
}
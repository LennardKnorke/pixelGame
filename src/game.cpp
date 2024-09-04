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


game_save::game_save(gamesave_summary *s, std::string host_id, bool *dev){
    save_thread = new sf::Thread(&game_save::save_save_t, this);
    DEV_MODE = dev;

    std::string file_path = s->path;
    std::ifstream inputFile(file_path, std::ios::binary);

    std::string confirm_name;
    readStrOfFile(inputFile, confirm_name);
    bool confirm_initialized;
    inputFile.read(reinterpret_cast<char*>(&confirm_initialized), sizeof(bool));

    if (confirm_initialized == s->initialized && confirm_name == s->name){
        summary.name = s->name;
        summary.path = s->path;
        summary.initialized = s->initialized;
    }
    else {
        std::cerr << "ERROR: Save file corrupted" << std::endl;
        valid = false;
        return;
    }

    if (summary.initialized){
        load_save(inputFile);
        inputFile.close();
    }
    else {
        inputFile.close();
        init_save(host_id);
        save_thread->launch();
    }

    valid = true;
}

void game_save::init_save(std::string host_id){
    std::random_device rd;
    std::mt19937_64 eng(rd()); 
    // Initialize save
    seed = std::uniform_int_distribution<unsigned int>(0, UINT16_MAX)(eng);


    // Initialize (empty) map
    Map_Generator mapGen(seed);
    // Ugly but alright.
    for (auto i = 0; i < MAP_WIDTH; i++){
        for (auto j = 0; j < MAP_HEIGHT; j++){
            map[i][j] = mapGen.tile_map[i][j];
        }
    }
    add_player(host_id);
    //
}

short game_save::add_player(std::string id){
    /// Returns the index of the player after adding them to the Player List
    players.push_back(id);

    if (hosting){
        save_thread->launch();
    }
    return players.size()-1;
}

void game_save::load_save(std::ifstream &inputFile){
    // Load game
}

void game_save::save_save_t(void){
    std::cout << "Saving game" << std::endl;
}

game_save::game_save(){
    // Constructor. save all parameters given
}
game_save::~game_save(){
    // Destructor
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
    
    if (DEV_MODE){
        std::cout << "INIT HOST GAME: " << std::endl;
        std::cout << "\t" << save->summary.path << "\t";
        std::cout << "\t" << save->summary.name << "\t";
        std::cout << "\t" << save->summary.initialized << std::endl;
    }
    // LOADING THREAD. Load game from save
    loadingThread->launch();
    this->save = new game_save(&s); // Loads game
    if (!save->valid){
        gameState = QUIT_ERROR;
    }
    // Host specific loading
    if (mode == gameMode::Host && gameState != QUIT_ERROR){
        port = findFreePort();
        if (port == 0){
            std::cerr << "ERROR: Failed to find free port" << std::endl;
            gameState = QUIT_ERROR;
        }
        else {
            ip = sf::IpAddress::getLocalAddress();
            listener.listen(port, ip);
        }
        
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
#include "application.hpp"

////////////////////////////////////////////
//APPLICATION CLASS


//Application main function
Application::Application(bool dev){
    DEV_MODE = dev;     // Prints much more information to the console + visual debug (pending)

    //Set up assets, settings etc.
    State = GAME_STATE::MENU;
    initSettings();
    initWindow();

    if (!loadAssets()){
        State = GAME_STATE::QUIT;
    }
    else {
        cursor = new Cursor(&window, &scale_window, &assets.textures.cursors);
        setUpSaveFolder();
    }

    
    //Main application loop. Switches between the main menu, game and exit
    while (State != GAME_STATE::QUIT){
        if (State == GAME_STATE::MENU){
            State = menuLoop();
        }
        else if (State == GAME_STATE::GAME)
        {
            State = gameLoop();
        }
    }

    //Finish and close everything
    window.close();
    return;
}



void Application::initSettings(void){
    // Set up Settings
    if (fileExists("settings.bin")){
        loadSettings();
    }
    else {
        createSettings();
    }
}



void Application::loadSettings(void){
    /*
    load the settings from the file into temporary struct then convert
    */
    settings = load_settings();
    scale_window = sf::Vector2f((float)settings.res_x / (float)DEFAULT_WINDOW_WIDTH, (float)settings.res_y / (float)DEFAULT_WINDOW_HEIGHT);

    if (DEV_MODE){
        std::cout << "LOADED SETTINGS:" << std::endl;
        std::cout << "\tUser: " << settings.userId << std::endl;
        std::cout << "\tVolume: " << settings.volumne << std::endl;
        std::cout << "\tFPS: " << settings.fps << std::endl;
        std::cout << "\tFullscreen: " << settings.fullscreen << std::endl;
        std::cout << "\tResolution: " << settings.res_x << "x" << settings.res_y << std::endl;
        std::cout << "\tControls: " << std::endl;
        for (short i = 0; i < n_keyInputOptions; i++){
            std::cout << "\t\t" << settings.controls[i].iType;
            if (settings.controls[i].iType == inputType::KEYBOARD){
                std::cout << "\t" << settings.controls[i].input.keyInput << std::endl;
            }
            else if (settings.controls[i].iType == inputType::MOUSE_BUTTON){
                std::cout << "\t" << settings.controls[i].input.mouseInput << std::endl;
            }
        }
        std::cout << std::endl;
    }
}



void Application::createSettings(void){
    
    //Generate A key
    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<std::string::size_type> dist(0, alphabet.size()-1);
    for (short i = 0 ; i < maxInputLengths::userId; i++){
        //this->localUserID += alphabet[dist(rng)];
        this->settings.userId += alphabet[dist(rng)];
    }

    //Set default volume
    this->settings.volumne = DEFAULT_VOLUME;

    //set default controls
    this->settings.controls[up].iType = inputType::KEYBOARD;
    this->settings.controls[up].input.keyInput = sf::Keyboard::W;

    this->settings.controls[up].iType = inputType::KEYBOARD;
    this->settings.controls[up].input.keyInput = sf::Keyboard::S;

    this->settings.controls[up].iType = inputType::KEYBOARD;
    this->settings.controls[up].input.keyInput = sf::Keyboard::A;

    this->settings.controls[up].iType = inputType::KEYBOARD;
    this->settings.controls[up].input.keyInput = sf::Keyboard::D;

    this->settings.controls[up].iType = inputType::KEYBOARD;
    this->settings.controls[up].input.keyInput = sf::Keyboard::Q;

    this->settings.controls[up].iType = inputType::KEYBOARD;
    this->settings.controls[up].input.keyInput = sf::Keyboard::E;

    this->settings.controls[up].iType = inputType::MOUSE_BUTTON;
    this->settings.controls[up].input.mouseInput = sf::Mouse::Button::Left;

    this->settings.controls[up].iType = inputType::KEYBOARD;
    this->settings.controls[up].input.keyInput = sf::Keyboard::LShift;

    this->settings.controls[up].iType = inputType::KEYBOARD;
    this->settings.controls[up].input.keyInput = sf::Keyboard::G;

    //settings default grafic infos;
    this->settings.fps = DEFAULT_FPS;
    this->settings.fullscreen = DEFAULT_FULLSCREEN;
    this->settings.res_x = DEFAULT_WINDOW_WIDTH;
    this->settings.res_y = DEFAULT_WINDOW_HEIGHT;

    if (DEV_MODE){
        std::cout << "CREATED SETTINGS:" << std::endl;
        std::cout << "\tUser: " << settings.userId << std::endl;
        std::cout << "\tVolume: " << settings.volumne << std::endl;
        std::cout << "\tFPS: " << settings.fps << std::endl;
        std::cout << "\tFullscreen: " << settings.fullscreen << std::endl;
        std::cout << "\tResolution: " << settings.res_x << "x" << settings.res_y << std::endl;
        std::cout << "\tControls: " << std::endl;
        for (short i = 0; i < n_keyInputOptions; i++){
            std::cout << "\t\t" << i << ": " << settings.controls[i].iType;
            if (settings.controls[i].iType == inputType::KEYBOARD){
                std::cout << " " << settings.controls[i].input.keyInput;
            }
            else if (settings.controls[i].iType == inputType::MOUSE_BUTTON){
                std::cout << " " << settings.controls[i].input.mouseInput;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    save_settings(settings);

    scale_window = sf::Vector2f((float)settings.res_x / (float)DEFAULT_WINDOW_WIDTH, (float)settings.res_y / (float)DEFAULT_WINDOW_HEIGHT);
}



void Application::initWindow(void){


    //Set up window
    if (settings.fullscreen){
        settings.res_x = sf::VideoMode::getDesktopMode().width;
        settings.res_y =  sf::VideoMode::getDesktopMode().height;
        window.create(sf::VideoMode(settings.res_x, settings.res_y, 32), "ReFrAcTuReD", sf::Style::Fullscreen);
    }
    else {
        settings.res_x = DEFAULT_WINDOW_WIDTH;
        settings.res_y = DEFAULT_WINDOW_HEIGHT;
        window.create(sf::VideoMode(settings.res_x, settings.res_y, 32), "ReFrAcTuReD", sf::Style::Titlebar | sf::Style::Close);
    }
    scale_window = sf::Vector2f((float)settings.res_x / (float)DEFAULT_WINDOW_WIDTH, (float)settings.res_y / (float)DEFAULT_WINDOW_HEIGHT);

    // Further window settings
    window.setFramerateLimit(settings.fps);
    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);
    window.setVerticalSyncEnabled(true);
}


//MANAGE ASSETS
bool Application::loadAssets(void){
    //Load Textures
    if (!(loadTextures())){
        return false;
    }

    //Load Music Assets
    int i = 0;
    for (auto &iterator : std::filesystem::directory_iterator("assets/music/")){
        if (!assets.backgroundMusic[i].openFromFile(iterator.path().string())){
            std::cout << "Failed to load Music: " << iterator.path().string() << std::endl;
            return false;
        }
        assets.backgroundMusic[i].setLoop(true);
        assets.backgroundMusic[i].setVolume(settings.volumne);
        i++;
    }

    //Load Font Asset
    if (!(assets.gameFont.loadFromFile("PFAgoraSlabPro Bold.ttf"))){
        std::cout << "Failed to load display font!\n";
        return false;
    }
    //Load Otherstuff?


    return true;
}



void Application::setUpSaveFolder(void){
    std::filesystem::path pathDirectory = std::string("sav/");
    if (!std::filesystem::exists(pathDirectory)){
        std::cout<<"Savefolder being created"<<std::endl;
        std::filesystem::create_directory(pathDirectory);
    }
}



bool Application::loadTextures(void){
    // Load Images
    if (!assets.textures.cursors.loadFromFile("assets/sprites/cursor.png") ||
        !assets.textures.menu.loadFromFile("assets/sprites/menu/0.png")
        ){
        std::cout << "Failed to textures: " << std::endl;
        return false;
    }
    assets.textures.cursors.setSmooth(true);
    assets.textures.menu.setSmooth(true);
    /* 
    // Load Menu Textures
    int texture_idx = 0;
    for (auto &iterator : std::filesystem::directory_iterator("assets/sprites/menu/")){
        if (!(assets.textures.menu[texture_idx].loadFromFile(iterator.path().string()))){
            std::cout << "Failed to load menu texture: " << iterator << std::endl;
            return false;
        }
        assets.textures.menu[texture_idx].setSmooth(true);
        texture_idx += 1;
        if (texture_idx == nr_menu_textures){//So we don't load random assets, forgotten or added to the folder
            break;
        }
    }
    // Load CursorTextures
    texture_idx = 0;
    for (auto &iterator : std::filesystem::directory_iterator("assets/sprites/cursor/")){
        if (!(assets.textures.cursors[texture_idx].loadFromFile(iterator.path().string()))){
            std::cout << "Failed to load cursor texture: " << iterator << std::endl;
            return false;
        }
        texture_idx += 1;
        if (texture_idx == nr_cursor_textures){
            break;
        }
    }
    //Get Character Textures

    //Get Monster Textures

    //Get Weapon Textures

    //Get Game Textures
    */

    return true;
}

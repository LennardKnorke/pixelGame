#include "application.hpp"

////////////////////////////////////////////
//APPLICATION CLASS


//Application main function
Application::Application(void){

    //Set up assets, settings etc.
    State = GAME_STATE::MENU;
    initSettings();
    initWindow();

    if (!loadAssets()){
        State = GAME_STATE::QUIT;
    }
    else {
        cursor = new Cursor(&window, res_tools, assets.textures.cursors);
        setUpSaveFolder();
        std::cout << machinePublicAdress << "\t" << machineLocalAdress <<  std::endl;
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
    std::ifstream inputFile("settings.bin", std::ios::binary);

    //Read id of client
    readStrOfFile(inputFile, localUserID);

    //Read volumne
    inputFile.read(reinterpret_cast<char *>(&volume), sizeof(float));

    std::cout   <<"Loaded user: " << localUserID << std::endl;

    //load control settings
    for (int i = 0; i < n_keyInputOptions; i++){
        inputFile.read(reinterpret_cast<char *>(&inGameControls[i].iType), sizeof(inputType));
        if (inGameControls[i].iType == inputType::KEYBOARD){
            inputFile.read(reinterpret_cast<char *>(&inGameControls[i].input.keyInput),sizeof(sf::Keyboard::Key));
        }
        else if (inGameControls[i].iType == inputType::MOUSE_BUTTON){
            inputFile.read(reinterpret_cast<char *>(&inGameControls[i].input.mouseInput),sizeof(sf::Mouse::Button));
        }
    }
    
    // FPs and full screen
    inputFile.read(reinterpret_cast<char *>(&FPS), sizeof(int));
    inputFile.read(reinterpret_cast<char *>(&res_tools.fullscreen), sizeof(bool));

    inputFile.close();

    

}



void Application::createSettings(void){
    
    //Generate A key
    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<std::string::size_type> dist(0, alphabet.size()-1);
    for (int i = 0 ; i < maxInputLengths::userId; i++){
        this->localUserID += alphabet[dist(rng)];
    }
    std::cout   << "Created User:  "<< this->localUserID << std::endl;

    //Set default volume
    volume = DEFAULT_VOLUME;
    FPS = DEFAULT_FPS;

    //set default controls
    inGameControls[up].iType = inputType::KEYBOARD;
    inGameControls[up].input.keyInput = sf::Keyboard::W;

    inGameControls[down].iType = inputType::KEYBOARD;
    inGameControls[down].input.keyInput = sf::Keyboard::S;

    inGameControls[left].iType = inputType::KEYBOARD;
    inGameControls[left].input.keyInput = sf::Keyboard::A;

    inGameControls[right].iType = inputType::KEYBOARD;
    inGameControls[right].input.keyInput = sf::Keyboard::D;

    inGameControls[prevItem].iType = inputType::KEYBOARD;
    inGameControls[prevItem].input.keyInput = sf::Keyboard::Q;

    inGameControls[nextItem].iType = inputType::KEYBOARD;
    inGameControls[nextItem].input.keyInput = sf::Keyboard::E;

    inGameControls[attack].iType = inputType::MOUSE_BUTTON;
    inGameControls[attack].input.mouseInput = sf::Mouse::Button::Left;
    
    inGameControls[special].iType = inputType::KEYBOARD;
    inGameControls[special].input.keyInput = sf::Keyboard::LShift;

    inGameControls[useItem].iType = inputType::KEYBOARD;
    inGameControls[useItem].input.keyInput = sf::Keyboard::G;

    res_tools.fullscreen = DEFAULT_FULLSCREEN;

    saveSettings();
}



void Application::saveSettings(void){
    std::ofstream outputFile("settings.bin", std::ios::binary);
    
    // save local user ID
    writeStrToFile(outputFile, localUserID);


    // save volumne setting resolution
    outputFile.write(reinterpret_cast<char *>(&volume), sizeof(float));


    // Save Control settings
    for (int i = 0; i < n_keyInputOptions; i++){
        outputFile.write(reinterpret_cast<const char*>(&inGameControls[i].iType), sizeof(inputType));
        if (inGameControls[i].iType == inputType::KEYBOARD){
            outputFile.write(reinterpret_cast<const char*>(&inGameControls[i].input.keyInput), sizeof(sf::Keyboard::Key));
        }
        else if (inGameControls[i].iType == inputType::MOUSE_BUTTON){
            outputFile.write(reinterpret_cast<const char*>(&inGameControls[i].input.mouseInput), sizeof(sf::Mouse::Button));
        }        
    }

    // Save FPS and Fullscreen
    outputFile.write(reinterpret_cast<char *>(&FPS), sizeof(int));
    outputFile.write(reinterpret_cast<char *>(&res_tools.fullscreen), sizeof(bool));

    outputFile.close();
}



void Application::initWindow(void){
    //Get resolutions
    res_tools.dev_res = sf::Vector2u(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    res_tools.desktop_res = sf::Vector2u(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);

    //Set up window
    if (res_tools.fullscreen){
        res_tools.res = res_tools.desktop_res;
        window.create(sf::VideoMode(res_tools.res.x, res_tools.res.y, 32), "ReFrAcTuReD", sf::Style::Fullscreen);
    }
    else {
        res_tools.res = res_tools.dev_res;
        window.create(sf::VideoMode(res_tools.res.x, res_tools.res.y, 32), "ReFrAcTuReD", sf::Style::Titlebar | sf::Style::Close);
    }
    res_tools.scale = sf::Vector2f((float)res_tools.res.x/(float)res_tools.dev_res.x, (float)res_tools.res.y/(float)res_tools.res.y);

    std::cout << window.getSize().x << " " << window.getSize().y << std::endl;

    // Further window settings
    window.setFramerateLimit(FPS);
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
        assets.backgroundMusic[i].setVolume(volume);
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


    return true;
}

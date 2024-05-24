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

    setUpCursorAssets();
    setUpSaveFolder();
    std::cout << machinePublicAdress << "\t" << machineLocalAdress <<  std::endl;


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



void Application::initWindow(void){
    //Get screen resolution
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    resolution = sf::Vector2u(desktop.width, desktop.height);
    
    window.create(sf::VideoMode(resolution.x, resolution.y, 32), "ReFrAcTuReD", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);
}



void Application::initSettings(void){
    // Set up Settings
    if (fileExists("settings.bin")){
        loadSettings();
    }
    else {
        createSettings();
    }

    // Set up hamachi ip
    if (!fileExists("multiplayer.txt")){
        std::ofstream outputFile("multiplayer.txt");
        outputFile << "Copy Your Own Hamachi Ip Here\n";
        outputFile.close();
    }
    else {
        std::string line;
        // Read one line from file
        std::ifstream inputFile("multiplayer.txt");
        std::getline(inputFile, line);
        inputFile.close();
        // Check if line is a valid IP
        if (validIP(line)){
            machinePublicAdress = sf::IpAddress(line);
        }
        
    }
}



void Application::loadSettings(void){
    std::ifstream inputFile("settings.bin", std::ios::binary);

    //Read id of client
    readStrOfFile(inputFile, localUserID);

    //Read standart resolution
    inputFile.read(reinterpret_cast<char *>(&volume), sizeof(float));

    std::cout   <<"Loaded user: " << localUserID << std::endl;

    //load control settings
    for (int i = 0; i < 7; i++){
        inputFile.read(reinterpret_cast<char *>(&inGameControls[i].iType), sizeof(inputType));
        if (inGameControls[i].iType == inputType::KEYBOARD){
            inputFile.read(reinterpret_cast<char *>(&inGameControls[i].input.keyInput),sizeof(sf::Keyboard::Key));
        }
        else if (inGameControls[i].iType == inputType::MOUSE_BUTTON){
            inputFile.read(reinterpret_cast<char *>(&inGameControls[i].input.mouseInput),sizeof(sf::Mouse::Button));
        }
    }
    
    inputFile.close();
}




void Application::createSettings(void){
    
    //Generate A key
    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<std::string::size_type> dist(0, alphabet.size()-1);
    for (int i = 0 ; i < maxInputLengths::userId; i++){
        localUserID += alphabet[dist(rng)];
    }
    std::cout   << "Created User:  "<< localUserID << std::endl;

    //Set default volume
    volume = 50.0;

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

    outputFile.close();
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
        if (!backgroundMusic[i].openFromFile(iterator.path().string())){
            std::cout << "Failed to load Music: " << iterator.path().string() << std::endl;
            return false;
        }
        backgroundMusic[i].setLoop(true);
        backgroundMusic[i].setVolume(volume);
        i++;
    }

    //Load Font Asset
    if (!(gameFont.loadFromFile("PFAgoraSlabPro Bold.ttf"))){
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
        if (!(textures.menu[texture_idx].loadFromFile(iterator.path().string()))){
            std::cout << "Failed to load menu texture: " << iterator << std::endl;
            return false;
        }
        texture_idx += 1;
        if (texture_idx == nr_menu_textures){//So we don't load random assets, forgotten or added to the folder
            break;
        }
    }
    // Load CursorTextures
    texture_idx = 0;
    for (auto &iterator : std::filesystem::directory_iterator("assets/sprites/cursor/")){
        if (!(textures.cursors[texture_idx].loadFromFile(iterator.path().string()))){
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




void Application::setUpCursorAssets(void){
    for (int i = 0; i < nr_cursor_textures; i++){
        cursor.sprite[i].setTexture(textures.cursors[i]);
    }
    cursor.sprite[cursorSpriteIndexes::menu].setScale((100)/cursor.sprite[cursorSpriteIndexes::menu].getGlobalBounds().getSize().x, (100)/cursor.sprite[cursorSpriteIndexes::menu].getGlobalBounds().getSize().y);
    cursor.changeSprite(cursorSpriteIndexes::menu);
    cursor.print = true;
}


void Application::resetHostInfo(void){
    this->hostAdress.ip = sf::IpAddress::None;
    this->hostAdress.port = 0;
    this->hostAdress.pathSave.clear();
}
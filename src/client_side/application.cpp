#include "application.hpp"
#include "gameSaveSummary.hpp"
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



//Set up render window
void Application::initWindow(void){
    window.create(sf::VideoMode(resolution.x, resolution.y, 32), "ReFrAcTuReD", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);
}



//find or create settings
void Application::initSettings(void){
    std::string setting_file = "settings.bin";
    if (fileExists(setting_file)){
        loadSettings(setting_file);
    }
    else {
        createSettings(setting_file);
    }
    ratioScaling = 1920.0/resolution.x;

    if (!fileExists("multiplayer.txt")){
        std::ofstream outputFile("multiplayer.txt");
        outputFile << "None\n";
        outputFile.close();
    }
}


//load
void Application::loadSettings(const std::string &filename){
    std::ifstream inputFile(filename, std::ios::binary);

    size_t size;
    inputFile.read(reinterpret_cast<char *>(&size), sizeof(size));
    localUserID.resize(size);
    inputFile.read(reinterpret_cast<char *>(&localUserID[0]), size);
    inputFile.read(reinterpret_cast<char *>(&resolution), sizeof(sf::Vector2u));
    //inputFile.read(reinterpret_cast<char*>(&inGameControls), sizeof(userKeys));

    std::cout   <<"Loaded user: " << localUserID << std::endl
                << resolution.x << "\t"
                << resolution.y << std::endl;

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



//First create settings/controls/userId and then run save function
void Application::createSettings(const std::string &filename){
    //Get Screen Size
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    resolution = sf::Vector2u(desktop.width, desktop.height);
    if (desktop.width >= 1920 && desktop.height >= 1080){
        resolution.x = 1920;
        resolution.y = 1080;
    }
    else if (desktop.width >= 1600 && desktop.height >= 900){
        resolution.x = 1600;
        resolution.y = 900;
    }
    else if (desktop.width >= 1280 && desktop.height >= 720){
        resolution.x = 1280;
        resolution.y = 720;
    }else if (desktop.width >= 1024 && desktop.height >= 576){
        resolution.x = 1024;
        resolution.y = 576;
    }
    else {
        resolution.x = 960;
        resolution.y = 540;
    }
    

    //Generate A key
    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<std::string::size_type> dist(0, alphabet.size()-1);
    for (int i = 0 ; i < MAX_LENGTH_KEY; i++){
        localUserID += alphabet[dist(rng)];
    }
    std::cout   << "Created User:  "<< localUserID << std::endl
                << resolution.x << "\t"
                << resolution.y << std::endl;

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
    saveSettings(filename);
}



void Application::saveSettings(const std::string &filename){
    std::ofstream outputFile("settings.bin", std::ios::binary);
    size_t size = localUserID.size();
    outputFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
    outputFile.write(reinterpret_cast<const char *>(localUserID.data()), size);
    outputFile.write(reinterpret_cast<const char *>(&resolution), sizeof(sf::Vector2u));
    //Save Control settings
    for (int i = 0; i < 7; i++){
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
    //Load Sound Assets

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
    //Get Menu Textures
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
    //Get CursorTextures
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
    cursor.sprite[cursorSpriteIndexes::menu].setScale((100 / ratioScaling)/cursor.sprite[cursorSpriteIndexes::menu].getGlobalBounds().getSize().x, (100 / ratioScaling)/cursor.sprite[cursorSpriteIndexes::menu].getGlobalBounds().getSize().y);
    cursor.changeSprite(cursorSpriteIndexes::menu);
    cursor.print = true;
}



////////////////////////////////////////////
// CURSOR CLASS
////////////////////////////////////////////
void CursorSprite::changeSprite(cursorSpriteIndexes i){
    activeSprite = i;
}



void CursorSprite::update(void){
    sprite[activeSprite].setPosition(sf::Vector2f(sf::Mouse::getPosition()));
    pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
}



void CursorSprite::draw(sf::RenderWindow *renderwindow){
    if (print){
        renderwindow->draw(sprite[activeSprite]);
    }
    
}



sf::Vector2f CursorSprite::returnPosition(void){
    return sprite[activeSprite].getPosition();
}



sf::Vector2f CursorSprite::returnSize(void){
    return sprite[activeSprite].getGlobalBounds().getSize();
}




#include "application.hpp"



////////////////////////////////////////////
//APPLICATION CLASS
////////////////////////////////////////////
Application::Application(void){
    State = GAME_STATE::MENU;
    initSettings();
    initWindow();

    if (!loadAssets()){
        State = GAME_STATE::QUIT;
    }

    setUpCursorAssets();
    setUpSaveFolder();

    std::cout << publicAdress << "\t" << localAdress <<  std::endl;

    //Main Game Loop. Switch between gamestates
    while (State != GAME_STATE::QUIT){
        if (State == GAME_STATE::MENU){
            State = menuLoop();
        }
        else if (State == GAME_STATE::GAME)
        {
            State = gameLoop();
        }
    }
    window.close();
    return;
}



void Application::initWindow(void){
    window.create(sf::VideoMode(resolution.x, resolution.y, 32), "ReFrAcTuReD", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);
}



//SETTINGS FUNCTIONS
void Application::initSettings(void){
    std::string setting_file = "settings.bin";
    if (fileExists(setting_file)){
        loadSettings(setting_file);
    }
    else {
        createSettings(setting_file);
    }
    ratioScaling = 1920.0/resolution.x;
}



void Application::loadSettings(const std::string &filename){
    std::ifstream inputFile(filename, std::ios::binary);

    inputFile.read(reinterpret_cast<char *>(&userKey), sizeof(userKey));
    inputFile.read(reinterpret_cast<char *>(&resolution), sizeof(sf::Vector2u));
    inputFile.read(reinterpret_cast<char*>(&inGameControls), sizeof(userKeys));

    std::cout   <<"Loaded user: " << userKey << std::endl
                << resolution.x << "\t"
                << resolution.y << std::endl;
    
    inputFile.close();
}



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
    const std::string alphabet = "!#$%^&*(){}=-+/abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<std::string::size_type> dist(0, alphabet.size()-1);
    for (int i = 0 ; i < MAX_LENGTH_KEY; i++){
        userKey[i] = alphabet[dist(rng)];
    }
    userKey[MAX_LENGTH_KEY] = '\0';
    std::cout   << "Created User:  "<< userKey << std::endl
                << resolution.x << "\t"
                << resolution.y << std::endl;

    //set default controls
    inGameControls.Jump = sf::Keyboard::W;
    inGameControls.Down = sf::Keyboard::S;
    inGameControls.Left = sf::Keyboard::A;
    inGameControls.Right = sf::Keyboard::D;
    inGameControls.nextItem = sf::Keyboard::E;
    inGameControls.prevItem = sf::Keyboard::Q;

    inGameControls.attack.butt = sf::Mouse::Button::Left;
    saveSettings(filename);
}



void Application::saveSettings(const std::string &filename){
    std::ofstream outputFile("settings.bin", std::ios::binary);
    outputFile.write(reinterpret_cast<const char *>(&userKey), sizeof(userKey));
    outputFile.write(reinterpret_cast<const char *>(&resolution), sizeof(sf::Vector2u));
    outputFile.write(reinterpret_cast<char*>(&inGameControls), sizeof(userKeys));
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



void Application::readAllSaveSummaries(void){
    std::filesystem::path pathDirectory = std::string("sav/");
    for (auto &iterator : std::filesystem::recursive_directory_iterator(pathDirectory)){
        if (iterator.path().extension() == std::filesystem::path(".SAV")){
            availableSaveFiles.push_back(loadSaveSummary(iterator.path().string()));
        }
    }
}



gameSaveSummary Application::loadSaveSummary(const std::string &filename){
    std::ifstream inputFile(filename, std::ios::binary);
    gameSaveSummary sumTmp;
    size_t size;

    inputFile.read(reinterpret_cast<char *>(&size), sizeof(size));
    sumTmp.saveName.resize(size);
    inputFile.read(reinterpret_cast<char *>(&sumTmp.saveName[0]), size);

    inputFile.read(reinterpret_cast<char *>(&size), sizeof(size));
    sumTmp.fileName.resize(size);
    inputFile.read(reinterpret_cast<char *>(&sumTmp.fileName[0]), size);

    inputFile.read(reinterpret_cast<char *>(&size), sizeof(size));
    sumTmp.pathName.resize(size);
    inputFile.read(reinterpret_cast<char *>(&sumTmp.pathName[0]), size);


    inputFile.close();
    return sumTmp;

}



bool Application::createSave(std::string newSaveName, menuPopUps *menuWarning){
    if (availableSaveFiles.size() >= 5){
        *menuWarning = menuPopUps::TooManySaves;
        return false;
    }
    for (gameSaveSummary tmp : availableSaveFiles){
        if (tmp.saveName == newSaveName){
            *menuWarning = menuPopUps::InvalidName;
            return false;
        }
    }
    activeSave = new gameSave(newSaveName);
    return true;
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



////OUT OF SCOPE FUNCTIONS
bool fileExists(const std::string &filename){
    std::ifstream file(filename);
    bool works = file.good();
    file.close();
    return works;
}
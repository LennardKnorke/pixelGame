#include "application.hpp"

////////////////////////////////////////////
//APPLICATION CLASS
////////////////////////////////////////////
Application::Application(void){
    State = MENU;
    initSettings();
    initWindow();

    if (!loadAssets()){
        State = QUIT;
    }
    setUpCursorAssets();
    setUpSaveFolder();

    std::cout << publicAdress << "\t" << localAdress <<  std::endl;

    //Main Game Loop. Switch between gamestates
    while (State != QUIT){
        if (State == MENU){
            State = menuLoop();
        }
        else if (State == GAME)
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


bool Application::fileExists(const std::string &filename){
    std::ifstream file(filename);
    bool works = file.good();
    file.close();
    return works;
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
}


void Application::loadSettings(const std::string &filename){
    std::ifstream inputFile(filename, std::ios::binary);
    inputFile.read(reinterpret_cast<char *>(&userKey), sizeof(userKey));
    inputFile.read(reinterpret_cast<char *>(&resolution), sizeof(sf::Vector2u));
    std::cout   <<"Loaded user: " << userKey << std::endl
                << resolution.x << "\t"
                << resolution.y << std::endl;
    inputFile.close();
}


void Application::createSettings(const std::string &filename){
    //Get Screen Size
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    resolution = sf::Vector2u(desktop.width, desktop.height);
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

    //
    saveSettings(filename);
}


void Application::saveSettings(const std::string &filename){
    std::ofstream outputFile("settings.bin", std::ios::binary);
    outputFile.write(reinterpret_cast<const char *>(&userKey), sizeof(userKey));
    outputFile.write(reinterpret_cast<const char *>(&resolution), sizeof(sf::Vector2u));
    outputFile.close();
}
///

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


void Application::readAllSaveFiles(void){
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
    inputFile.read(reinterpret_cast<char *>(&sumTmp.filename[0]), size);

    inputFile.read(reinterpret_cast<char *>(&sumTmp.initialized), sizeof(bool));

    inputFile.close();
    return sumTmp;

}

bool Application::createSaveFile(std::string newSaveName){
    if (newSaveName == ""){
        return false;
    }
    //find a valid savename
    std::string newFileName = "WORLD_";
    std::string pathName = "sav/"+newFileName;
    int i = 0;
    while (fileExists(pathName+std::to_string(i)+".SAV")){
        i++;
    }
    
    newFileName = newFileName+std::to_string(i);
    pathName = pathName+std::to_string(i);
    std::cout <<pathName <<std::endl;
    //set up the newly created save as the active one
    activeSave.initialized = false;
    activeSave.saveName = newSaveName;
    activeSave.filename = newFileName;
    activeSave.owner = userKey;
    activeSave.playerNumber = 0;

    //save like a normal save file
    saveSave(activeSave, pathName);
    return true;
}
void Application::saveSave(gameSave Save, const std::string &path){
    std::ofstream file(path+".SAV", std::ios::binary);

    size_t size = Save.saveName.size();
    //Save the saveName (name given by player)
    file.write(reinterpret_cast<char *>(&size), sizeof(size));
    file.write(reinterpret_cast<char*>(Save.saveName.data()), size);
    //Save the filename (name of the file)

    //Save whether save has been initialized
    file.write(reinterpret_cast<char*>(&Save.initialized), sizeof(bool));
    if (Save.initialized){
        std::cout<< "SAVING EVERYTHING\n";
    }
    

    file.close();
}
gameSave Application::loadSave(const std::string &filename){
    gameSave tmp;
    return tmp;
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
    cursor.sprite[0].setScale(100/cursor.sprite[0].getGlobalBounds().getSize().x, 100/cursor.sprite[0].getGlobalBounds().getSize().y);
    cursor.changeSprite(0);
}

////////////////////////////////////////////
// CURSOR CLASS
////////////////////////////////////////////
void CursorSprite::changeSprite(int i){
    activeSprite = i;

}
void CursorSprite::update(void){
    sprite[activeSprite].setPosition(sf::Vector2f(sf::Mouse::getPosition()));
    pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

void CursorSprite::draw(sf::RenderWindow *renderwindow){
    renderwindow->draw(sprite[activeSprite]);
}

sf::Vector2f CursorSprite::returnPosition(void){
    return sprite[activeSprite].getPosition();
}
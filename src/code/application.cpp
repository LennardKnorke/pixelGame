#include "application.hpp"
Rectangle createRectangle(int height, int width, int posX, int posY){
    Rectangle rec;
    rec.height = height;
    rec.width = width;
    rec.topLeft.x = posX;
    rec.topLeft.y = posY;

    rec.topRight.x = posX + width;
    rec.topRight.y = posY;

    rec.bottomLeft.x = posX;
    rec.bottomLeft.y = posY + height;

    rec.bottomRight.x = posX + width;
    rec.bottomRight.y = posY + height;

    rec.center.x = posX + (width / 2);
    rec.center.y = posY + (height / 2);

    rec.leftX = posX;
    rec.rightX = posX + width;
    rec.upperY = posY;
    rec.lowerY = posY + height;
    return rec;
}
////////////////////////////////////////////
//APPLICATION CLASS
////////////////////////////////////////////
Application::Application(void){
    applicationState = STATE_MENU;
    initSettings();
    initWindow();

    if (!loadAssets()){
        applicationState = STATE_QUIT;
    }
    connectTexturesWClasses();
    readAllSaveFiles();

    std::cout << publicAdress << "\t" << localAdress <<  std::endl;

    //Main Game Loop. Switch between gamestates
    while (applicationState != STATE_QUIT){
        if (applicationState == STATE_MENU){
            applicationState = menu();
        }
        else if (applicationState == STATE_GAME)
        {
            applicationState = game();
        }
    }
    window.close();
    return;
}


void Application::initWindow(void){
    window.create(sf::VideoMode(resolution.x, resolution.y, 32), "ReFrAcTuReD", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
}


bool Application::fileExists(const std::string &filename){
    std::ifstream file(filename);
    return file.good();
}


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

void Application::readAllSaveFiles(void){
    std::filesystem::path pathDirectory = std::string("sav/");
    if (!std::filesystem::exists(pathDirectory)){
        std::cout<<"Savefolder being created"<<std::endl;
        std::filesystem::create_directory(pathDirectory);
    }
    else {
        std::cout<<"Savefolder available"<<std::endl;
        for (auto &iterator : std::filesystem::recursive_directory_iterator(pathDirectory)){
            if (iterator.path().extension() == std::filesystem::path(".SAVEFILE")){
                availableSaveFiles.push_back(loadSaveSummary(iterator.path().string()));
            }
        }
    }
    
}


gameSaveSummary Application::loadSaveSummary(const std::string &filename){
    std::ifstream inputFile(filename, std::ios::binary);
    gameSaveSummary sumTmp;
    inputFile.read(reinterpret_cast<char *>(&sumTmp.saveName), sizeof(std::string));
    inputFile.read(reinterpret_cast<char *>(&sumTmp.filename), sizeof(std::string));
    inputFile.close();
    return sumTmp;

}

void Application::writeSaveFile(gameSave Save){

}


bool Application::RectangleCollision(Rectangle rec_1, Rectangle rec_2){
    if (true){
        return true;
    }
    else {
        return false;
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
        if (texture_idx == nr_menu_textures){
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

void Application::connectTexturesWClasses(void){
    for (int i = 0; i < nr_cursor_textures; i++){
        cursor.sprite[i].setTexture(textures.cursors[i]);
    }
    cursor.sprite[0].setScale(100/cursor.sprite[0].getGlobalBounds().getSize().x, 100/cursor.sprite[0].getGlobalBounds().getSize().y);
}

////////////////////////////////////////////
// CURSOR CLASS
////////////////////////////////////////////

CursorSprite::CursorSprite(void){

}
void CursorSprite::changeSprite(int i){
    activeSprite = i;

}
void CursorSprite::update(void){
}

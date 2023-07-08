#include "application.hpp"

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
    std::cout   <<"Loaded user: " << this->userKey << "\n"
                << resolution.x << "\t"
                << resolution.y <<"\n";
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
    std::cout   << "Created User:  "<< userKey << "\n"
                << resolution.x << "\t"
                << resolution.y << "\n";

    //
    this->saveSettings(filename);
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

bool Application::RectangleCollision(rectangle rec1, rectangle rec2){
    if (rec1.position.x > rec2.position.x 
            && rec1.position.x < rec2.position.x+rec2.size.width
            && rec1.position.y > rec2.position.y
            && rec1.position.y < rec2.position.y+rec2.size.height
        ||rec1.position.x+rec1.size.width < rec2.position.x+rec2.size.width
            && rec1.position.x+rec1.size.width > rec2.position.x
            && rec1.position.y > rec2.position.y
            && rec1.position.y < rec2.position.y+rec2.size.height
        ||rec1.position.x > rec2.position.x 
            && rec1.position.x < rec2.position.x+rec2.size.width
            && rec1.position.y+rec1.size.height > rec2.position.y
            && rec1.position.y+rec1.size.height < rec2.position.y+rec2.size.height
        ||rec1.position.x+rec1.size.width < rec2.position.x+rec2.size.width
            && rec1.position.x+rec1.size.width > rec2.position.x
            && rec1.position.y+rec1.size.height > rec2.position.y
            && rec1.position.y+rec1.size.height < rec2.position.y+rec2.size.height){
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
    rec.position = sf::Mouse::getPosition();
    activeSprite = 0;
}
void CursorSprite::changeSprite(int i){
    activeSprite = i;
}
void CursorSprite::update(void){
    rec.position = sf::Mouse::getPosition();
    click = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    sprite[activeSprite].setPosition(rec.position.x, rec.position.y);
}

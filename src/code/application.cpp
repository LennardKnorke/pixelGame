#include "application.hpp"

Application::Application(void){
    applicationRunning = true;
    applicationState = STATE_MENU;
    initSettings();
    initWindow();

    std::cout << publicAdress << "\t" << localAdress <<  std::endl;


    while (applicationState != STATE_QUIT){
        if (applicationState == STATE_MENU){
            applicationState = menu();
        }
        else if (applicationState == STATE_GAME)
        {
            applicationState = game();
        }
    }
    return;
}

void Application::initWindow(void){
    window.create(sf::VideoMode(resolution.x, resolution.y, 32), "ReFrAcTuReD", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
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
    std::cout   <<"Loaded user: " << userKey << "\n"
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
    saveSettings(filename);
}

void Application::saveSettings(const std::string &filename){
    std::ofstream outputFile("settings.bin", std::ios::binary);
    outputFile.write(reinterpret_cast<const char *>(&userKey), sizeof(userKey));
    outputFile.write(reinterpret_cast<const char *>(&resolution), sizeof(sf::Vector2u));
    outputFile.close();
}

int Application::game(void){
    std::cout << "Started the game loop!"<< std::endl;
    return 0;
}
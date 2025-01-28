#include "application.hpp"



Application::Application(bool devMode){
    this->stateTracker = new stateInfo();
    this->devMode = devMode;
    stateTracker->setAppState(appState::MENU);
    this->error = errorCodes::NoErr;
    this->settings = new settings_class(std::filesystem::exists("settings.bin"), &devMode);
    this->assets = new asset_manager();
    if (!this->assets->load()){
        std::cout << "Failed to load assets" << std::endl;
        stateTracker->setAppState(appState::EXIT);
        delete this->settings;
        delete this->assets;
        return;
    }
    
    try {
        this->setUpSaveFolder();
        this->initWindow();
    } catch (const std::exception &e){
        std::cout << "Failed to set up save folder or window" << std::endl;
        stateTracker->setAppState(appState::EXIT);
        delete this->settings;
        delete this->assets;
        return;
    }
    this->cursor = new Cursor(window, &settings->res.scaling, &assets->textures.cursors);


    //Main application loop. Switches between the main menu, game and exit
    if (devMode){
        std::cout << "Application started" << std::endl;
    }

    while (stateTracker->state != appState::EXIT){
        if (stateTracker->state == appState::MENU){
            this->menuLoop();
        }
        else if (stateTracker->state == appState::GAME){
            this->gameLoop();
        }
        else {
            std::cout << "Undefined application state. Ending process." << std::endl;
            stateTracker->state = appState::EXIT;
        }
    }
    window->close();
}

Application::~Application(void){
    delete this->settings;
    delete this->assets;
    delete this->window;
    delete this->cursor;
    delete this->stateTracker;
}

void Application::setUpSaveFolder(void){
    std::filesystem::path pathDirectory = std::string("sav/");
    if (!std::filesystem::exists(pathDirectory)){
        std::cout<<"Savefolder being created"<<std::endl;
        std::filesystem::create_directory(pathDirectory);
    }

    // Create a folder that could be filled with notes while running
    if (devMode && !std::filesystem::exists("dev_notes/")){
        std::filesystem::create_directory("dev_notes/");
    }
}

void Application::initWindow(void){
    //Set up window
    window = new sf::RenderWindow();
    if (settings->res.fullscreen){
        window->create(sf::VideoMode({settings->res.width, settings->res.height}, 32), "ReFrAcTuReD", sf::Style::Titlebar | sf::Style::Close, sf::State::Fullscreen);
    }
    else {
        window->create(sf::VideoMode({settings->res.width, settings->res.height}, 32), "ReFrAcTuReD", sf::Style::Titlebar | sf::Style::Close, sf::State::Windowed);
    }

    // Further window settings
    window->setFramerateLimit(settings->fps);
    window->setMouseCursorVisible(false);
    window->setMouseCursorGrabbed(true);
}

void Application::menuLoop(void){
    MainMenu *menuInstance = new MainMenu(window, cursor, settings, assets, stateTracker,devMode);
    menuInstance->run();
    return;
}

void Application::gameLoop(void){
    std::cout << "Game loop" << std::endl;

    stateTracker->state = EXIT;
    return;
}



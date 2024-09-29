#include "application.hpp"

////////////////////////////////////////////
//APPLICATION CLASS


//Application main function
Application::Application(bool dev){
    DEV_MODE = dev;     

    //Set up assets, settings etc.
    State = GAME_STATE::MENU;
    
    settings = new settings_class(std::filesystem::exists("settings.bin"), &DEV_MODE);
    initWindow();

    if (!assets.load()){
        State = GAME_STATE::QUIT;
    }
    else {
        cursor = new Cursor(&window, &settings->res.scaling, &assets.textures.cursors);
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


void Application::initWindow(void){


    //Set up window
    if (settings->res.fullscreen){
        window.create(sf::VideoMode(settings->res.width, settings->res.height, 32), "ReFrAcTuReD", sf::Style::Fullscreen);
    }
    else {
        window.create(sf::VideoMode(settings->res.width, settings->res.height, 32), "ReFrAcTuReD", sf::Style::Titlebar | sf::Style::Close);
    }

    // Further window settings
    window.setFramerateLimit(settings->fps);
    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);
    window.setVerticalSyncEnabled(true);
}



void Application::setUpSaveFolder(void){
    std::filesystem::path pathDirectory = std::string("sav/");
    if (!std::filesystem::exists(pathDirectory)){
        std::cout<<"Savefolder being created"<<std::endl;
        std::filesystem::create_directory(pathDirectory);
    }

    // Create a folder that could be filled with notes while running
    if (DEV_MODE){
        if (!std::filesystem::exists("dev_notes/")){
            std::filesystem::create_directory("dev_notes/");
        }
    }
}
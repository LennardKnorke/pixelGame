#pragma once
#ifndef APPLICATION_HPP
#define APPLICATON_HPP

// Include standard libraries
#include <iostream>
#include <filesystem>
#include <random>

// Include SFML libraries
#include "SFML/Graphics.hpp"
// #include "SFML/Audio.hpp"
// #include "SFML/Network.hpp"

// Include custom headers
#include "cursor.hpp"
#include "save_management.hpp"
#include "settings.hpp"
#include "asset_manager.hpp"
#include "utils.hpp"

#include "menu.hpp"

/// @brief makros: load/display error messages in mainmenu
enum errorCodes{
    NoErr,                   //!< display no error
    Error
};




/// \brief Application class, contains global variables for the local client, booting information and most functions
class Application{
    public:

    /**
     * @brief Default constructor that runs the application.
     */
    Application(bool dev);
    ~Application();
    bool devMode;                   //!< Developer mode. Prints more information to the console
    errorCodes error;               //!< error to display after application has failed. TO DO

    private:
    /**
     * @brief Set up the rendering window.
     */
    void initWindow(void);

    /**
     * @brief Create a folder for save files if it is not present.
     */
    void setUpSaveFolder(void);
    
    /**
     * @brief Run the main menu.
     * @return the next game state (game loop or exit).
     */
    void menuLoop(void);

    /**
     * @brief Run the main game loop.
     * @return the next game state (main menu loop or exit).
     */
    void gameLoop(void);


    // Settings variables
    settings_class *settings;        //!< settings of the application

    // Asset variables
    asset_manager *assets;           //!< asset manager for the application

    // Window variables 
    sf::RenderWindow *window;            //!< window variable to display everything in
    Cursor *cursor;                     //!< application cursor. used throughout the programme
    stateInfo *stateTracker; //!< state tracker for the application
};



#endif //APPLICATION_HPP
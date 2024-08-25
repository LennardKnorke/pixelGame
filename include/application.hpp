#pragma once
#ifndef APPLICATION_HPP
#define APPLICATON_HPP

// Include standard libraries
#include <iostream>
#include <filesystem>
#include <random>

// Include SFML libraries
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Network.hpp"

// Include custom headers
#include "control_structs.hpp"
#include "utils.hpp"
#include "cursor.hpp"
#include "save_management.hpp"
#include "settings.hpp"
#include "asset_manager.hpp"


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
    errorCodes error = NoErr;       //!< error to display after application has failed. TO DO
    
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
    GAME_STATE menuLoop(void);

    /**
     * @brief Run the main game loop.
     * @return the next game state (main menu loop or exit).
     */
    GAME_STATE gameLoop(void);


    // Settings variables
    bool DEV_MODE;                  //!< Developer mode. Prints more information to the console
    settings_class *settings;        //!< settings of the application

    // Asset variables
    asset_manager assets;           //!< asset manager for the application

    // Window variables 
    sf::RenderWindow window;            //!< window variable to display everything in
    Cursor *cursor;                     //!< application cursor. used throughout the programme

    // Loading/Set Up tools for either joining or hosting a game
    struct setUpTools {                             //!< contains info for connecting to host or hosting
        sf::IpAddress ip = sf::IpAddress::None;     //!< ip to connect to (IF JOIN)
        unsigned short port = 0;                    //!< port to connect to (IF JOIN)
        gamesave_summary chosenSave;                //!< (IF HOST/SINGLEPLAYER) path to chosen save
    }loadingTools;                                

    // Controller variables
    GAME_STATE State;               //!< state of game/APPLICATION. game, menu, or exit
    gameMode mode = undefined;      //!< Online, Local, or Alone


    
};



#endif //APPLICATION_HPP
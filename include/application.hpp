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



/// @brief makros: load/display error messages in mainmenu
enum errorCodes{
    NoErr                   //!< display no error
};

#define nr_menu_textures 1      //!< maximum number of menu textures to load
#define nr_backgroundMusic 2    //!< maximum number of music files to be loaded

/// @brief makros: idx menu textures
enum menuTextureIdxS {
    background = 0,     //!< background texture index
};

/// @brief makros: idx background music
enum musicIdx {
    mainMenu = 0,           //!< main Menu
    game_main = 1,          //!< main Game
};


/// \brief Application class, contains global variables for the local client, booting information and most functions
class Application{
    public:

    /**
     * @brief Default constructor that runs the application.
     */
    Application(void);
    errorCodes error = NoErr;       //!< error to display after application has failed. TO DO
    
    private:
    /**
     * @brief Set up the rendering window.
     */
    void initWindow(void);

    /**
     * @brief Load, initialize, or create settings.
     */
    void initSettings(void);

    /**
     * @brief Load an existing settings file.
     */
    void loadSettings(void);

    /**
     * @brief Create a setting file with default settings.
     */
    void createSettings(void);

    /**
     * @brief Save the setting file.
     */
    void saveSettings(void);

    /**
     * @brief Load all assets, such as textures and music.
     * @return true if successful, false otherwise.
     */
    bool loadAssets(void);
    
    /**
     * @brief Load textures into the application.
     * @return true if successful, false otherwise.
     */
    bool loadTextures(void);

    /**
     * @brief Load cursor textures.
     */
    void setUpCursorAssets(void);

    /**
     * @brief Create a folder for save files if it is not present.
     */
    void setUpSaveFolder(void);
    
    /**
     * @brief NOT IMPLEMENTED YET
    */
   void resetHostInfo(void);

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


    // Window variables
    sf::RenderWindow window;    //!< window variable to display everything in
    bool fullscreen = true;     //!< fullscreen mode
    sf::Vector2u resolution;    //!< resolution of the window
    sf::Vector2f res_scaling;   //!< scaling of the resolution
    short FPS;                  //!< frames per second
    
    Cursor cursor;              //!< application cursor. used throughout the programm
    
    float volume;               //!< 0-1

    // Networking variables
    sf::IpAddress machinePublicAdress = sf::IpAddress::getPublicAddress();  //!< local ip address of user machine
    sf::IpAddress machineLocalAdress = sf::IpAddress::getLocalAddress();    //!< public ip address of user machine

    // Loading/Set Up tools for either joining or hosting a game
    struct setUpTools {                             //!< contains info for connecting to host or hosting
        sf::IpAddress ip = sf::IpAddress::None;     //!< ip to connect to (IF JOIN)
        unsigned short port = 0;                    //!< port to connect to (IF JOIN)
        gamesave_summary chosenSave;                //!< (IF HOST/SINGLEPLAYER) path to chosen save
    }loadingTools;                                

    // Controller variables
    GAME_STATE State;               //!< state of game/APPLICATION. game, menu, or exit
    gameMode mode = undefined;      //!< Online, Local, or Alone
    
    std::string localUserID;        //!< Id of active user

    // collection of loadable assets. everything that needs to be loaded and is presented sensory
    struct Assets {
        // Audio assets
        sf::Music backgroundMusic[nr_backgroundMusic];    //!< background music
        
        // Textures assets
        struct Textures {
            sf::Texture cursors[nr_cursor_textures];      //!< cursor textures
            sf::Texture menu[nr_menu_textures];           //!< menu textures
        } textures;

        // Font assets
        sf::Font gameFont;  //!< font used for all text in the game
    } assets;

    // Saved input variables. (TO DO: make adaptable in menu -> save after user changes)
    inGameInputKey inGameControls[n_keyInputOptions];   //!< assigned keys for in-game controls
};



#endif //APPLICATION_HPP
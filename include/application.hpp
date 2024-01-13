#pragma once
#ifndef APPLICATION_HPP
#define APPLICATON_HPP
#include "stdlibs.hpp"
#include "cursor.hpp"


/// @brief makros: load/display error messages in mainmenu
enum errorCodes{
    ConnectErr,             //!<
    FileLoadErr,            //!<
    InvalidSaveNameErr,     //!<
    NoErr                   //!< display no error
};



#define nr_menu_textures 1      //!< maximum number of menu textures to load
#define MAX_N_SAVES 5           //!< maximum number of saves to load/create
#define nr_backgroundMusic 2    //!< maximum number of music files to be loaded

/// @brief makros: idx menu textures
enum menuTextureIdxS {
    background = 0,     //!< background texture index
};

/// @brief makros: idx background music
enum musicIdx {
    mainMenu = 0,           //!< main Menu
    game_main = 1,      //!< main Game
};

/// @brief makros: type of input
enum inputType {
    KEYBOARD,       //!< type: Keyboard input
    MOUSE_BUTTON    //!< type: mouse button input
};


/// @brief used to associate an input function (e.g. user clicked "attack") with a key or mouse input
typedef struct inGameInputKey {
    inputType iType;                    //!< input can be either a mouse key or keyboard key
    union inputUnion{
        sf::Keyboard::Key keyInput;     //!< associated keyboardInput
        sf::Mouse::Button mouseInput;   //!< associated mousebuttonInput
    }input;
}inGameInputKey;




////////////////////////////////////////////////////////////
/// \brief Application class, contains global variables for
///        the local client, booting information and most
///         functions
////////////////////////////////////////////////////////////



class Application{
    public:

    /**
     * @brief Default constructor that runs the application.
     */
    Application(void);

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
     * 
     * @return true if successful, false otherwise.
     */
    bool loadAssets(void);
    
    /**
     * @brief Load textures into the application.
     * 
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
     * @brief Run the main menu.
     * 
     * @return the next game state (game loop or exit).
     */
    GAME_STATE menuLoop(void);

    /**
     * @brief Run the main game loop.
     * 
     * @return the next game state (main menu loop or exit).
     */
    GAME_STATE gameLoop(void);

    /**
     * @brief VARIABLES
     */
    sf::RenderWindow window;    //!< window variable to display everything in
    float volume;               //!< 0-1
    sf::Vector2u resolution;    //!< resolution of the window
    CursorSprite cursor;        //!< application cursor
    
    sf::Font gameFont;          //!< Font used for all sf::Text instances

    struct Textures {                               //!< contains all textures (FILL FOR MORE TEXTURES USED IN GAME!)
        sf::Texture cursors[nr_cursor_textures];    //!< cursor textures
        sf::Texture menu[nr_menu_textures];         //!< menu textures
    }textures;  
    
    //!< local ip address of user machine
    sf::IpAddress machinePublicAdress = sf::IpAddress::getPublicAddress();
    //!< public ip address of user machine
    sf::IpAddress machineLocalAdress = sf::IpAddress::getLocalAddress();
    
    struct socketAdress {                       //!< contains info for connecting to host or hosting
        sf::IpAddress ip = sf::IpAddress::None; //!< ip to connect to
        unsigned short port = 0;                //!< port to connect to
        std::string pathSave;                   //!< (IF HOST) path to save for loading
    }hostAdress;                                

    GAME_STATE State;               //!< state of game/APPLICATION. game, menu, or exit
    gameMode mode = undefined;      //!< Online, Local, or Alone
    errorCodes error = NoErr;       //!< error to display
    std::string localUserID;        //!< Id of active user

    sf::Music backgroundMusic[nr_backgroundMusic];
    inGameInputKey inGameControls[n_keyInputOptions];   //!< assigned keys for in-game controls
};



#endif //APPLICATION_HPP
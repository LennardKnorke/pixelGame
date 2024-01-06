#pragma once
#ifndef APPLICATION_HPP
#define APPLICATON_HPP
#include "stdlibs.hpp"
#include "cursor.hpp"

/// @brief makros: Gamestates
enum GAME_STATE {
    MENU,   //!< in the main menu
    GAME,   //!< in the main game loop
    QUIT    //!< quit application
};


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

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor runs the application
    ////////////////////////////////////////////////////////////
    Application(void);

    ////////////////////////////////////////////////////////////
    /// \brief Set up the rendering window
    ///
    ////////////////////////////////////////////////////////////
    void initWindow(void);

    ////////////////////////////////////////////////////////////
    /// \brief Load/init/create settings
    ////////////////////////////////////////////////////////////
    void initSettings(void);

    ////////////////////////////////////////////////////////////
    /// \brief Load existing settings file
    ////////////////////////////////////////////////////////////
    void loadSettings(void);

    ////////////////////////////////////////////////////////////
    /// \brief create setting file with default settings
    ////////////////////////////////////////////////////////////
    void createSettings(void);

    ////////////////////////////////////////////////////////////
    /// \brief Saves the setting file
    ////////////////////////////////////////////////////////////
    void saveSettings(void);

    ////////////////////////////////////////////////////////////
    /// \brief Load ALL assets, like textures/music etc.
    ///
    /// \return true if succesfull, else false
    ////////////////////////////////////////////////////////////
    bool loadAssets(void);
    
    ////////////////////////////////////////////////////////////
    /// \brief Load textures into application
    ///
    /// \return true if succesfull, else false
    ////////////////////////////////////////////////////////////
    bool loadTextures(void);

    ////////////////////////////////////////////////////////////
    /// \brief Load Cursor textures
    ////////////////////////////////////////////////////////////
    void setUpCursorAssets(void);


    ////////////////////////////////////////////////////////////
    /// \brief Creates folder for savefiles if not present
    ////////////////////////////////////////////////////////////
    void setUpSaveFolder(void);
    
    ////////////////////////////////////////////////////////////
    /// \brief Run the main menu. Definition found in menu.cpp
    ///
    /// \return next game state (game loop or exit)
    ////////////////////////////////////////////////////////////
    GAME_STATE menuLoop(void);

    ////////////////////////////////////////////////////////////
    /// \brief Run the main game loop
    ///
    /// \return next game state (main menu loop or exit)
    ////////////////////////////////////////////////////////////
    GAME_STATE gameLoop(void);

        ////////////////////////////////////////////////////////////
        /// \brief Display a loading screen and attempt to connect 
        /// with host
        ///
        /// \param socket the client socket to connect with host
        ///
        /// \return returns the game loop makro if successful,
        /// menu otherwise
        ////////////////////////////////////////////////////////////
        gameLoopState loadingScreen(sf::TcpSocket &socket);

        ////////////////////////////////////////////////////////////
        /// \brief main gameloop: Draw textures
        ////////////////////////////////////////////////////////////
        void drawGame(void);

        ////////////////////////////////////////////////////////////
        /// \brief main gameloop: register keyinput
        ///
        /// \param s optional to change gameloop state to menu if 
        /// escape button is pressed
        ///
        /// \param controlInput struct which "saves" the current user input
        ////////////////////////////////////////////////////////////
        void registerGameInput(sf::Event &ev, gameInput_msg &playerInput);

        ////////////////////////////////////////////////////////////
        /// \brief main gameloop: exchange info with server, update local client
        ///
        /// \param socket socket through which communicate with host 
        ///
        /// \param packet sfml package to fill and send via client
        ////////////////////////////////////////////////////////////
        void updateGame(sf::TcpSocket &socket, gameInput_msg &controlMessages);

        ////////////////////////////////////////////////////////////
        /// \brief main gameloop: draw menubuttons
        ///
        /// \param v vector with buttons for ingame Menu
        ////////////////////////////////////////////////////////////
        void drawMenu(std::vector<inGameMenuButton*> v);

        ////////////////////////////////////////////////////////////
        /// \brief main gameloop: register key and mosue input
        ///
        /// \param s option to update the gameloop state
        ///
        /// \param v vector with buttons for ingame Menu
        ////////////////////////////////////////////////////////////
        void registerMenuInput(sf::Event &ev, gameLoopState &s, std::vector<inGameMenuButton*> &v);

        ////////////////////////////////////////////////////////////
        /// \brief main gameloop: update menu/buttons in menu 
        ///
        /// \param v vector with buttons for ingame Menu
        ////////////////////////////////////////////////////////////
        void updateMenu(sf::TcpSocket &socket, std::vector<inGameMenuButton*> &v);

        ////////////////////////////////////////////////////////////
        /// \brief main gameloop: draw skilltree (NOT FINISHED)
        ////////////////////////////////////////////////////////////
        void drawTree(void);

        ////////////////////////////////////////////////////////////
        /// \brief main gameloop: register input in tree window (NOT FINISHED)
        ///
        /// \param s potential to go back to main menu/update game state
        ////////////////////////////////////////////////////////////
        void registerTreeInput();

        ////////////////////////////////////////////////////////////
        /// \brief main gameloop: exchange info with host, update local (NOT FINISHED)
        ////////////////////////////////////////////////////////////
        void updateTree(sf::TcpSocket &socket, skillTreeInput_msg &msg);


    ////////////////////////////////////////////////////////////
    ////        VARIABLES
    ////////////////////////////////////////////////////////////
    sf::RenderWindow window;    //!< window variable to display everything in
    float volume;              //!< 0-1
    sf::Vector2u resolution;    //!< resolution of the window
    CursorSprite cursor;        //!< application cursor
    
    sf::Font gameFont;          //!< Font used for all sf::Text instances

    struct Textures {                               //!< contains all textures (FILL FOR MORE TEXTURES USED IN GAME!)
        sf::Texture cursors[nr_cursor_textures];    //!< cursor textures
        sf::Texture menu[nr_menu_textures];         //!< menu textures
    }textures;  
    
    
    std::vector<gameSaveSummary> availableSaveFiles;//!< contains summaries of available saves

    //!< local ip adress of user machine
    sf::IpAddress machinePublicAdress = sf::IpAddress::getPublicAddress();
    //!< public ip adress of user machine
    sf::IpAddress machineLocalAdress = sf::IpAddress::getLocalAddress();
    
    struct socketAdress {                       //!< contains info for connecting to host or hosting
        sf::IpAddress ip = sf::IpAddress::None; //!< ip to connect to
        unsigned short port = 0;                //!< port to connect to
        std::string pathSave;                   //!< (IF HOST) path to save for loading
    }hostAdress;                                

    GAME_STATE State;               //!< state of game/APPLICATION. game, menu or exit
    gameMode mode = undefined;      //!< Online, Local or Alone
    errorCodes error = NoErr;       //!< error to display
    std::string localUserID;        //!< Id of active user

    sf::Music backgroundMusic[nr_backgroundMusic];
    inGameInputKey inGameControls[n_keyInputOptions];   //!< assigned keys for ingame controls
};



#endif //APPLICATION_HPP
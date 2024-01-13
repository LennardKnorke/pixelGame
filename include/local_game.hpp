#pragma once
#ifndef LOCAL_GAME_HPP
#define LOCAL_GAME_HPP
#include "stdlibs.hpp"


/**
 * @class GameClass
 * @brief Represents a game class that manages the local game logic.
 */
class GameClass {
    public:
    /**
     * @brief Constructs a GameClass object.
     * @param appPointer A pointer to the Application object.
     */
    GameClass(Application *appPointer);

    /**
     * @brief Destructs the GameClass object.
     */
    ~GameClass(void);

    /**
     * @brief Runs the game logic.
     * @return The current game state.
     */
    GAME_STATE runGame(void);

    /**
     * @brief Loads and connects the game resources.
     * @return True if successful, false otherwise.
     */
    bool loadAndConnect(void);

    

    private:
    
    void connectToHost(void);

    /**
     * @brief Draws the game.
     */
    void draw(void);
            /**
             * @brief Draws the skill tree.
             */
            void draw_skillTree(void);

            /**
             * @brief Draws the game.
             */
            void draw_game(void);

            /**
             * @brief Draws the pause screen.
             */
            void draw_pause(void);

    /**
     * @brief Handles the user input for the skill tree.
     */
    void userInput_game(void);
    /**
     * @brief Handles the user input for the game.
     */
    void userInput_skilltree(void);
    /**
     * @brief Handles the user input for the pause screen.
     */
    void userInput_pause(void);

    void update_game(void);
    void update_skilltree(void);
    void update_pause(void);

    sf::RenderWindow *window;   /**< Pointer to the game window. */
    sf::Music *backgroundMusic; /**< Pointer to the background music. */
    CursorSprite *cursor;       /**< Pointer to the cursor sprite. */
    /**
     * @struct hostinfo
     * @brief Represents the host information.
     */
    struct hostinfo {
        sf::IpAddress ip;       /**< The IP address of the host. */
        unsigned short port;    /**< The port number of the host. */
    }host;
    std::string playerID;       /**< The player ID. */
    sf::TcpSocket socket;       /**< The TCP socket for network communication. */
    sf::Thread *networkThread;  /**< The thread for network communication. */
    bool connected;             /**< Indicates whether the client is connected to the host. */
    gameLoopState gameState;    /**< The current game loop state. */
    

    std::vector<inGameMenuButton*> menuButtons; /**< Vector of in-game menu buttons. */
};



////////////////////////////////////////////////////////////
/// \brief if host launch command line terminal. Parameters contain the command line arguments
///
/// \param adress pointer to host ip adress to set. used as command line argument
///
/// \param port short in which a port is set for hosting. used as command line argument
///
/// \param HostId command line argument: str with hostId for server command line argument
///
/// \param pathToSave command line argument: gamesave path for server to load 
///
/// \param Mode command line argument: mode for server to launch
///
/// \return true/false if command line launched succesfully
////////////////////////////////////////////////////////////
bool initServerProcess(sf::IpAddress *adress, unsigned short &port, std::string HostId, std::string pathToSave, gameMode Mode);

////////////////////////////////////////////////////////////
/// \brief set appropriate host ip adress to connect to and use in command line
///
/// \param adress pointer to host ip adress to set. used as command line argument
///
/// \param Mode online, local. decides which ip adress to set
///
/// \return true/false if VALID ip adress has been set
////////////////////////////////////////////////////////////
bool setHostIp(sf::IpAddress *adress, gameMode Mode);

////////////////////////////////////////////////////////////
/// \brief set open and appropriate host port to connect to and use in command line
///
/// \param port variable to use as command line
///
/// \param adress pointer to host ip adress to set. will be set to None if no port is found!
///
/// \param Mode online, local. decides which port to set
///
/// \return true/false if VALID port has been set
////////////////////////////////////////////////////////////
bool setHostPort(unsigned short &port, sf::IpAddress *adress, gameMode Mode);


class inGameStateApplication{
    public:
    virtual void draw(sf::RenderWindow &window);
    virtual void sendToHost(sf::TcpSocket &socket, sf::Packet &package);
};



#define n_menugameButtons 3 //!< number of ingame menubuttons
////////////////////////////////////////////////////////////
/**
 * @class inGameMenuButton
 * @brief Represents a button in the in-game menu.
 */
class inGameMenuButton{
    public:
    bool focus;                     /**< Indicates whether the button is currently focused. */
    std::string textString;         /**< The text displayed on the button. */
    sf::Text text;                  /**< The SFML text object used to render the button's text. */
    gameLoopState followUpState;    /**< The game loop state to transition to when the button is clicked. */
    sf::Sprite imageSprite;         /**< The SFML sprite object used to render the button's image. */

    /**
     * @brief Constructor for inGameMenuButton.
     * @param currentIdx The current index of the button.
     * @param app A pointer to the Application object.
     */
    inGameMenuButton(int currentIdx, Application *app);

    /**
     * @brief Updates the button based on the mouse position.
     * @param mousePos The current mouse position.
     */
    void update(sf::Vector2f mousePos);

    /**
     * @brief Draws the button on the specified window.
     * @param window A pointer to the SFML RenderWindow object.
     */
    void draw(sf::RenderWindow *window);

    /**
     * @brief Destructor for inGameMenuButton.
     */
    ~inGameMenuButton(void);
};

#endif //LOCAL_GAME_CLIENT_HPP
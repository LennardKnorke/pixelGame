#pragma once
#ifndef GAME_HPP
#define GAME_HPP

// Include standard libraries
#include <iostream>

// Include SFML libraries
#include <SFML/Graphics.hpp>

// Include project libraries
#include "control_structs.hpp"
#include "cursor.hpp"
#include "utils.hpp"

/// @brief Enumeration of possible return states
enum GameStates {
    RUNNING_GAME,   //!< Game is running
    QUIT_NOERROR,   //!< Quit the game without error (initiated by user)
    MENU_NOERROR,   //!< Return to the main menu without error (initiated by user)
    MENU_ERROR,     //!< Return to the main menu with error (initiated by program)
    QUIT_ERROR      //!< FATAL ERROR. Quiting Application (initiated by program)
};

enum inGameErrors {
    NO_ERROR,               //!< No error
};

class Game{
    public:
    /// @brief Constructor for playing singleplayer game
    Game(gameMode m, sf::RenderWindow *win, Cursor *cur, gamesave_summary save);

    /// @brief Constructor for joining a game
    Game(gameMode m, sf::RenderWindow *win, Cursor *cur, sf::IpAddress ip, unsigned short port);


    /**
     * @brief Runs the game loop
     */
    GAME_STATE run(void);     //!< Main game loop

    private: 

    GameStates gameState = RUNNING_GAME;   //!< Current state of the game
    gameMode mode;                          //!< Mode of the game
    

    struct network_tools {
        unsigned short host_port;                   //!< Port number
        sf::IpAddress host_ip;                      //!< IP address
        sf::TcpSocket socket;                       //!< Socket for communication
    } network_tools_c;                              //!< Network tools for clients    
    struct hosting_tools{
        sf::TcpListener listener;                   //!< Listener for incoming connection
    } hostingTools;


    gamesave_summary info;                   //!< Save file summary (path + name + initial state)

    sf::RenderWindow *window;                //!< Window for the game
    Cursor *cursor;                          //!< Cursor for the game

};


#endif // GAME_HPP
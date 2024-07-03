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
    Game();

    /**
     * @brief Runs the game loop
     */
    GAME_STATE run(void);     //!< Main game loop

    private:
    GameStates gameState = RUNNING_GAME;   //!< Current state of the game

};


#endif // GAME_HPP
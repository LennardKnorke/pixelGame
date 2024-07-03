/// @brief: structs used to define the input of the player

#pragma once
#ifndef CONTROL_STRUCTS_HPP
#define CONTROL_STRUCTS_HPP


#include "SFML/Graphics.hpp"

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


#define n_keyInputOptions 9 //!< number of possible inputs in the game

/// @brief possible inputs in the game
enum keyInputOptions {
    up = 0,             //!< jump
    down = 1,           //!< move down
    left = 2,           //!< move left
    right = 3,          //!< move right
    prevItem = 4,       //!< previous item
    nextItem = 5,       //!< next item
    attack = 6,         //!< attack
    special = 7,        //!< special
    useItem = 8         //!< use item
};


#endif //CONTROL_STRUCTS_HPP
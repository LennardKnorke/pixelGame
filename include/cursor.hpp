#pragma once
#ifndef CURSOR_HPP
#define CURSOR_HPP

#include <iostream>

#include "SFML/Graphics.hpp"

#include "utils.hpp"

#define nr_cursor_textures 2    //!< maximum number of cursor textures to load

/// \brief Enumeration of cursor sprite indexes
enum cursorSpriteIdx {
    menu = 0, 
    melee = 1,
    ranged = 2
};



/// \brief Class containing cursor sprites for the menu, in-game, etc.
class Cursor{    
public:
    Cursor(sf::RenderWindow *win, resolution_tools res, sf::Texture textures[nr_cursor_textures]);  //!< Constructor

    /**
     * @brief Change the active sprite used for the cursor
     * @param i New index of the sprite in the texture
     */
    void changeSprite(cursorSpriteIdx i);

    /**
     * @brief update the cursor position and pressed status
     */
    void update(void);

    /**
     * @brief Draw the cursor in the window
     * @param renderwindow Window to draw in
     */
    void draw(sf::RenderWindow &renderwindow);

    /**
     * @brief (NOT IMPLEMENTED)
     */
    void changeRes(void);

    /**
     * @brief Return the position of the cursor
     * @return The position of the cursor as a sf::Vector2f
     */
    sf::Vector2f returnPosition(void);

    /**
     * @brief Return the size of the cursor
     * @return The size of the cursor as a sf::Vector2f
     */
    sf::Vector2f returnSize(void);
    bool display;                           //!< True/false to show the cursor sprite    
    private:
    sf::RenderWindow *window;               //!< Pointer to the window
    cursorSpriteIdx spriteIdx;              //!< Active sprite for drawing
    sf::Sprite sprites[nr_cursor_textures]; //!< Available sprites
    bool pressed;                           //!< True/false if mouse button is pressed (redundant?)
    resolution_tools resolutions;           //!< Resolution tools
};


#endif // CURSOR_HPP
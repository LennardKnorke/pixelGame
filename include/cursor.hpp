#include "stdlibs.hpp"

#define nr_cursor_textures 2    //!< maximum number of cursor textures to load

/// \brief Enumeration of cursor sprite indexes
enum cursorSpriteIndexes {
    menu = 0, 
    game_base = 1
};

////////////////////////////////////////////////////////////
/// \brief Class containing cursor sprites for the menu,
///        in-game, etc.
////////////////////////////////////////////////////////////
class CursorSprite {    
public:

    ////////////////////////////////////////////////////////////
    /// \brief Change the active sprite used for the cursor
    ///
    /// \param i New index of the sprite in the texture
    ////////////////////////////////////////////////////////////
    void changeSprite(cursorSpriteIndexes i);

    ////////////////////////////////////////////////////////////
    /// \brief Run updates
    ////////////////////////////////////////////////////////////
    void update(void);

    ////////////////////////////////////////////////////////////
    /// \brief Draw the cursor in the window
    ///
    /// \param window Window to draw in
    ////////////////////////////////////////////////////////////
    void draw(sf::RenderWindow &renderwindow);

    ////////////////////////////////////////////////////////////
    /// \brief (NOT IMPLEMENTED)
    ////////////////////////////////////////////////////////////
    void changeRes(void);

    ////////////////////////////////////////////////////////////
    /// \brief Return the position of the cursor
    ///
    /// \return The position of the cursor as a sf::Vector2f
    ////////////////////////////////////////////////////////////
    sf::Vector2f returnPosition(void);

    ////////////////////////////////////////////////////////////
    /// \brief Return the size of the cursor
    ///
    /// \return The size of the cursor as a sf::Vector2f
    ////////////////////////////////////////////////////////////
    sf::Vector2f returnSize(void);

    cursorSpriteIndexes activeSprite;       //!< Active sprite for drawing
    sf::Sprite sprite[nr_cursor_textures];  //!< Available sprites
    bool pressed;                           //!< True/false if mouse button is pressed (redundant?)
    bool print;                             //!< True/false to show the cursor sprite
};
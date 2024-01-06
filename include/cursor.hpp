#include "stdlibs.hpp"

#define nr_cursor_textures 2    //!< maximum number of cursor textures to load

/// @brief makro: index cursorsprites textures
enum cursorSpriteIndexes {
    menu = 0, 
    game_base = 1
};


////////////////////////////////////////////////////////////
/// \brief Class containing cursor sprites for the menu,
///        ingame, etc.
////////////////////////////////////////////////////////////
class CursorSprite {    
    public:

    ////////////////////////////////////////////////////////////
    /// \brief change the active sprite used for cursor
    ///
    /// \param i new index of sprite in the texture
    ////////////////////////////////////////////////////////////
    void changeSprite(cursorSpriteIndexes i);

    ////////////////////////////////////////////////////////////
    /// \brief run updates
    ////////////////////////////////////////////////////////////
    void update(void);

    ////////////////////////////////////////////////////////////
    /// \brief draw in window
    ///
    /// \param window window to draw in
    ////////////////////////////////////////////////////////////
    void draw(sf::RenderWindow &renderwindow);

    ////////////////////////////////////////////////////////////
    /// \brief (NOT IMPLEMENTED)
    ////////////////////////////////////////////////////////////
    void changeRes(void);

    ////////////////////////////////////////////////////////////
    /// \brief return cursor/sprite info
    ///
    /// \return two floats for the postion
    ////////////////////////////////////////////////////////////
    sf::Vector2f returnPosition(void);

    ////////////////////////////////////////////////////////////
    /// \brief return cursor/sprite info
    ///
    /// \return vector of two floats for the size
    ////////////////////////////////////////////////////////////
    sf::Vector2f returnSize(void);

    cursorSpriteIndexes activeSprite;       //!< active sprite for drawing
    sf::Sprite sprite[nr_cursor_textures];  //!< available sprites
    bool pressed;                           //!< true/false if mousebutton is pressed (redundant?)
    bool print;                             //!< true/false show the cursor sprite
};
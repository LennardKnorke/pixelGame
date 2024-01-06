#include "cursor.hpp"
////////////////////////////////////////////
// CURSOR CLASS
////////////////////////////////////////////
void CursorSprite::changeSprite(cursorSpriteIndexes i){
    activeSprite = i;
}



void CursorSprite::update(void){
    sprite[activeSprite].setPosition(sf::Vector2f(sf::Mouse::getPosition()));
    pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
}



void CursorSprite::draw(sf::RenderWindow &renderwindow){
    if (print){
        renderwindow.draw(sprite[activeSprite]);
    }
    
}



sf::Vector2f CursorSprite::returnPosition(void){
    return sprite[activeSprite].getPosition();
}



sf::Vector2f CursorSprite::returnSize(void){
    return sprite[activeSprite].getGlobalBounds().getSize();
}




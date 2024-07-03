#include "cursor.hpp"
////////////////////////////////////////////
// CURSOR CLASS
////////////////////////////////////////////
void Cursor::changeSprite(cursorSpriteIdx i){
    this->spriteIdx = i;
}



void Cursor::update(void){
    this->sprites[this->spriteIdx].setPosition(sf::Vector2f(sf::Mouse::getPosition()));
    this->pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
}



void Cursor::draw(sf::RenderWindow &renderwindow){
    if (display){
        renderwindow.draw(this->sprites[this->spriteIdx]);
    }
    
}



sf::Vector2f Cursor::returnPosition(void){
    return this->sprites[this->spriteIdx].getPosition();
}



sf::Vector2f Cursor::returnSize(void){
    return this->sprites[this->spriteIdx].getGlobalBounds().getSize();
}




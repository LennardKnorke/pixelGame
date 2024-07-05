#include "cursor.hpp"
////////////////////////////////////////////
// CURSOR CLASS
////////////////////////////////////////////

Cursor::Cursor(sf::RenderWindow *win, resolution_tools res, sf::Texture textures[nr_cursor_textures]){
    this->window = win;
    resolutions = res;
    for (int i = 0; i < nr_cursor_textures; i++){
        this->sprites[i].setTexture(textures[i]);
        this->sprites[i].setScale(resolutions.scale.x, resolutions.scale.y);
    }
    this->changeSprite(cursorSpriteIdx::menu);
    this->update();
}

void Cursor::changeSprite(cursorSpriteIdx i){
    this->spriteIdx = i;
}


void Cursor::update(void){
    sf::Vector2i window_pos = window->getPosition();
    sf::Vector2i mouse_pos = sf::Mouse::getPosition();
    this->sprites[this->spriteIdx].setPosition(mouse_pos.x - window_pos.x, mouse_pos.y - window_pos.y);
    this->pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
}



void Cursor::draw(sf::RenderWindow &renderwindow){
    if (display){
        renderwindow.draw(this->sprites[this->spriteIdx]);
    }
    
}


sf::Vector2f Cursor::returnPosition(void){
    return this->sprites[this->spriteIdx].getGlobalBounds().getPosition();
}



sf::Vector2f Cursor::returnSize(void){
    return this->sprites[this->spriteIdx].getGlobalBounds().getSize();
}




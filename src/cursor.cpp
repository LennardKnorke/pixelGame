#include "cursor.hpp"
////////////////////////////////////////////
// CURSOR CLASS
////////////////////////////////////////////

Cursor::Cursor(sf::RenderWindow *win, sf::Vector2f *s, sf::Texture *textures){
    this->window = win;
    w_scale = s;

    for (auto i = 0; i < nr_cursor_textures; i++){
        sprites[i].setTexture(*textures);
        sprites[i].setTextureRect(sf::IntRect(0, 32 * i, 32, 32));
        sprites[i].setOrigin(0, 0);
        sprites[i].setScale(w_scale->x, w_scale->y);
    }
    this->changeSprite(cursorSpriteIdx::menu);
    this->update();
}

void Cursor::changeResolution(bool dev){
    // the scale has been adapted and needs to be updated for the sprite
    for (auto i = 0; i < nr_cursor_textures; i++){
        sprites[i].setScale(w_scale->x, w_scale->y);
    }
    changeSprite(idx);
}

void Cursor::changeSprite(cursorSpriteIdx i){
    this->idx = i;
}


void Cursor::update(void){
    sf::Vector2i window_pos = window->getPosition();
    sf::Vector2i mouse_pos = sf::Mouse::getPosition();
    this->sprites[idx].setPosition(mouse_pos.x - window_pos.x, mouse_pos.y - window_pos.y);
    this->pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
}



void Cursor::draw(sf::RenderWindow &renderwindow){
    if (display){
        renderwindow.draw(sprites[idx]);
    }
    
}


sf::Vector2f Cursor::returnPosition(void){
    return sprites[idx].getGlobalBounds().getPosition();
}



sf::Vector2f Cursor::returnSize(void){
    return sprites[idx].getGlobalBounds().getSize();
}




#include "cursor.hpp"
////////////////////////////////////////////
// CURSOR CLASS
////////////////////////////////////////////

Cursor::Cursor(sf::RenderWindow *win, sf::Vector2f *s, sf::Texture *textures){
    this->window = win;
    w_scale = s;

    for (auto i = 0; i < nr_cursor_textures; i++){
        sprites.push_back(sf::Sprite(*textures));
        sprites[i].setTextureRect(sf::IntRect(sf::Vector2i(0, 32 * i),sf::Vector2i(32, 32)));
        sprites[i].setOrigin(sf::Vector2f(0.0, 0.0));
        sprites[i].setScale(*w_scale);
    }
    this->changeSprite(cursorSpriteIdx::menu);
    this->update();
    
}

void Cursor::changeResolution(bool dev){
    // the scale has been adapted and needs to be updated for the sprite
    for (auto i = 0; i < nr_cursor_textures; i++){
        sprites[i].setScale(*w_scale);
    }
    changeSprite(idx);
}

void Cursor::changeSprite(cursorSpriteIdx i){
    this->idx = i;
}


void Cursor::update(void){
    sf::Vector2i window_pos = window->getPosition();
    sf::Vector2i mouse_pos = sf::Mouse::getPosition();
    std::cout << "Mouse pos: " << mouse_pos.x << " " << mouse_pos.y << std::endl;
    this->sprites[idx].setPosition(sf::Vector2f(mouse_pos.x - window_pos.x, mouse_pos.y - window_pos.y));
    this->pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}



void Cursor::draw(void){
    if (display){
        window->draw(sprites[idx]);
    }
    
}


sf::Vector2f Cursor::returnPosition(void){
    return sprites[idx].getGlobalBounds().position;
}



sf::Vector2f Cursor::returnSize(void){
    return sprites[idx].getGlobalBounds().size;
}




#include "cursor.hpp"
////////////////////////////////////////////
// CURSOR CLASS
////////////////////////////////////////////

Cursor::Cursor(sf::RenderWindow *win, sf::Vector2f *s, sf::Texture *textures){
    this->window = win;
    w_scale = s;
    sprite.setTexture(*textures);
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

    sprite.setScale(w_scale->x, w_scale->y);

    this->changeSprite(cursorSpriteIdx::menu);
    this->update();
}

void Cursor::changeResolution(bool dev){
    // the scale has been adapted and needs to be updated for the sprite
    sprite.setScale(w_scale->x, w_scale->y);
    changeSprite(spriteIdx);
    if (dev){
        std::cout << "New Cursor Rect: ";
        sf::IntRect s = sf::IntRect(0, 32 * (w_scale->y) * spriteIdx, 32 * (w_scale->x), 32 * (w_scale->y));
        std::cout << s.left << " " << s.top << " " << s.width << " " << s.height << std::endl;
    }
}

void Cursor::changeSprite(cursorSpriteIdx i){
    this->spriteIdx = i;
    this->sprite.setTextureRect(sf::IntRect(0, 32 * (w_scale->y) * i, 32 * (w_scale->x), 32 * (w_scale->y)));
    if (i == cursorSpriteIdx::menu){
        sprite.setOrigin(15 * (w_scale->x), 15 * (w_scale->y));
    }
    else {
        sprite.setOrigin(0, 0);
    }
}


void Cursor::update(void){
    sf::Vector2i window_pos = window->getPosition();
    sf::Vector2i mouse_pos = sf::Mouse::getPosition();
    this->sprite.setPosition(mouse_pos.x - window_pos.x, mouse_pos.y - window_pos.y);
    this->pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
}



void Cursor::draw(sf::RenderWindow &renderwindow){
    if (display){
        renderwindow.draw(sprite);
    }
    
}


sf::Vector2f Cursor::returnPosition(void){
    return sprite.getGlobalBounds().getPosition();
}



sf::Vector2f Cursor::returnSize(void){
    return sprite.getGlobalBounds().getSize();
}




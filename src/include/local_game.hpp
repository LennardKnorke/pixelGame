#pragma once
#ifndef LOCAL_GAME_HPP
#define LOCAL_GAME_HPP
#include "stdlibs.hpp"

#define n_menugameButtons 3

class inGameMenuButton{
    public:
    bool focus;
    std::string textString;
    sf::Text text;
    gameLoopState followUpState;
    sf::Sprite imageSprite;

    inGameMenuButton(int currentIdx, Application *app);
    void update(sf::Vector2f mousePos);
    void draw(sf::RenderWindow *window);
    ~inGameMenuButton(void);
};
bool setHostAddress(gameMode mode, sf::IpAddress *ip, unsigned short *p);
#endif //LOCAL_GAME_CLIENT_HPP
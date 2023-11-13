#pragma once
#ifndef LOCAL_GAME_HPP
#define LOCAL_GAME_HPP
#include "stdlibs.hpp"

#define n_menugameButtons 3

bool initServerProcess(sf::IpAddress *adress, unsigned short *port, std::string HostId, std::string pathToSave, gameMode Mode);
bool setHostIp(sf::IpAddress *adress, gameMode Mode);
bool setHostPort(unsigned short *port, sf::IpAddress *adress, gameMode Mode);

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
#endif //LOCAL_GAME_CLIENT_HPP
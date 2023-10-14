#pragma once
#ifndef LOCAL_GAME_HPP
#define LOCAL_GAME_HPP
#include "stdlibs.hpp"
class Application;

enum gameLoopState {
    Game, Menu, SkillTree,QuitGame, QuitMenu
};

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

#endif //LOCAL_GAME_CLIENT_HPP
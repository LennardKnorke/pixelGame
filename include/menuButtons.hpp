#pragma once
#ifndef MENUBUTTONS_HPP
#define MENUBUTTONS_HPP
#include "stdlibs.hpp"

/// Annoying implementation.

////////////////////////////////////////////////////////////
void setUpMenuButtons(std::vector<button*> &buttonList, Application *applicationPointer);
void drawMenuButtons(std::vector<button*> &MenuButtons, mainMenuLayerId currentLayer, sf::RenderWindow &window);

//Base class of a text button
class button{
    public:
    mainMenuLayerId nextLayer;
    mainMenuLayerId layer;
    std::string stringText;
    sf::Text text;
    sf::Sprite imageSprite;
    bool focus;
    bool pressed;
    int n_button;
    virtual void draw(sf::RenderWindow &window);
    virtual void update(sf::Vector2f mousePos);
    virtual ~button();
    virtual void changeRes(Application *applicationPointer, int maxButt, int currButt);
};



//child class of a clickable button
class ClickButton : public button {
    
    public:
    ClickButton(std::string t, mainMenuLayerId followLayer, Application *applicationPointer, int maxButt, int currButt, mainMenuLayerId currentLayer);
    void draw(sf::RenderWindow &window);
    void update(sf::Vector2f mousePos);

    void changeRes(Application *applicationPointer, int maxButt, int currButt);
    
    ~ClickButton(void);
};



//
class GraphicButton : public button{
    public:
    sf::Vector2u newResolution;
    GraphicButton(std::string t, mainMenuLayerId followLayer, Application *applicationPointer, int maxButt, int currButt, sf::Vector2u newRes, mainMenuLayerId currentLayer);
    ~GraphicButton(void);
    void draw(sf::RenderWindow &window);
    void update(sf::Vector2f mousePos);

    void changeRes(Application *applicationPointer, int maxButt, int currButt);

};

//childclass for a button which accepts text input
class newSafeButton : public button {
    public:
    std::string userText = "";
    float ogPosition[2];
    bool activeInput;
    newSafeButton(std::string t, mainMenuLayerId followLayer, Application *applicationPointer, int maxButt, int currButt, mainMenuLayerId given_layer);
    void update(sf::Vector2f mousePos);
    void draw(sf::RenderWindow &window);

    void changeRes(Application *applicationPointer, int maxButt, int currButt);
    
    void addInput(sf::Uint32 input, sf::Vector2u res);
    void delLastInput(sf::Vector2u res);
    void resetInput(void);
    ~newSafeButton(void);
};
class adressButton : public button {
    public:
    std::string userText = "";
    float ogPosition[2];
    bool activeInput;
    adressButton(std::string t, mainMenuLayerId followLayer, Application *applicationPointer, int maxButt, int currButt, mainMenuLayerId given_layer);
    void update(sf::Vector2f mousePos);
    void draw(sf::RenderWindow &window);

    void changeRes(Application *applicationPointer, int maxButt, int currButt);
    
    void addInput(sf::Uint32 input, sf::Vector2u res);
    void delLastInput(sf::Vector2u res);
    void resetInput(void);
    ~adressButton(void);
};


class ProfileButton : public button {
    public:
    ProfileButton(std::string t, mainMenuLayerId followLayer, Application *applicationPointer, int maxButt, int currButt, mainMenuLayerId currentLayer);
    void draw(sf::RenderWindow &window);
    void update(sf::Vector2f mousePos);

    void changeRes(Application *applicationPointer, int maxButt, int currButt);
    
    ~ProfileButton(void);
};
#endif
#pragma once
#ifndef MENU_HPP
#define MENU_HPP
class Application;


//Makros
#define MAX_LENGTH_KEY 14
#define MAX_LENGTH_SAVENAME 12
#define MAX_LENGTH_IP_PUBLIC 15

//Makros for Menu Layers
#define MaxVisibleLayers 8
enum layersId{
    leave = -1, 
    Base = 0, 
    Settings = 1,
    GameMode = 2, 
    HostVsClient = 3,
    Joining = 4,
    Hosting = 5, 
    Graphic = 6,
    Controls = 7,
    final = 12
};
layersId getPreviousLayer(layersId currentLayer);

//Makros for menu pop up/ error messages
enum menuPopUps {
    NoPopUp,
    InvalidName,
    TooManySaves,
    deleteSave
};


//Base class of a text button
class button{
    public:
    layersId nextLayer;
    layersId layer;
    std::string stringText;
    sf::Text text;
    sf::Sprite imageSprite;
    bool focus;
    bool pressed;
    virtual void draw(sf::RenderWindow *window);
    virtual void update(sf::Vector2f mousePos);

    virtual void changeRes(Application *applicationPointer, int maxButt, int currButt);
};



//child class of a clickable button
class ClickButton : public button {
    
    public:
    ClickButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt, layersId currentLayer);
    void draw(sf::RenderWindow *window);
    void update(sf::Vector2f mousePos);

    void changeRes(Application *applicationPointer, int maxButt, int currButt);
    
    ~ClickButton(void);
};



//
class GraphicButton : public button{
    public:
    sf::Vector2u newResolution;
    GraphicButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt, sf::Vector2u newRes, layersId currentLayer);
    ~GraphicButton(void);
    void draw(sf::RenderWindow *window);
    void update(sf::Vector2f mousePos);

    void changeRes(Application *applicationPointer, int maxButt, int currButt);

};



//childclass for a button which accepts text input
class WriteButton : public button {
    public:
    std::string userText = "";
    float ogPosition[2];
    bool activeInput;
    WriteButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt, layersId given_layer);
    void update(sf::Vector2f mousePos);
    void draw(sf::RenderWindow *window);

    void changeRes(Application *applicationPointer, int maxButt, int currButt);
    
    void addInput(sf::Uint32 input, sf::Vector2u res);
    void delLastInput(sf::Vector2u res);
    void resetInput(void);
    ~WriteButton(void);
};


void drawMenuButtons(std::vector<button*> *MenuButtons, layersId currentLayer, sf::RenderWindow *window);
void drawMenuPopUp(menuPopUps PopUp, sf::RenderWindow *window, sf::Text *warningMessage);
void setUpMenuButtons(std::vector<button*> *buttonList, Application *applicationPointer);
#endif //MENU_HPP
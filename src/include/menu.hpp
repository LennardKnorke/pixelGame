#pragma once
#ifndef MENU_HPP
#define MENU_HPP
class Application;



//Makros for Menu Layers
#define MaxVisibleLayers 6
enum layersId{
    leave = -1, 
    Base = 0, 
    Settings = 1, 
    Type = 2, 
    Multiplayer = 3, 
    Join = 4, 
    Host = 5, 
    final = 6
};



//Makros for menu pop up/ error messages
enum menuPopUps {
    NoPopUp,
    InvalidName,
    TooManySaves,
    deleteSave
};



#define InputMax_IpAdress 15
#define InputMax_Port 5



//Summarizes information about each menu layer, used to construct instances of layer class
typedef struct layerInformation{
    layersId lay;
    layersId prevLay;
    int nButtons;//Only default buttons. In host Layer, nondefault buttons are loaded
    std::vector<std::string> buttonTexts;
    std::vector<layersId>followUpLay;
}layerInformation;



//Base class of a text button
class button{
    public:
    layersId nextLayer;
    std::string stringText;
    sf::Text text;
    sf::IntRect rec;
    bool focus;
    bool pressed;
    virtual void draw(sf::RenderWindow *window);
    virtual void update(sf::Vector2f mousePos);
};



//child class of a clickable button
class ClickButton : public button {
    public:
    ClickButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt);
    void draw(sf::RenderWindow *window);
    void update(sf::Vector2f mousePos);
    ~ClickButton(void);
};



//childclass for a button which accepts text input
class WriteButton : public button {
    public:
    std::string userText = "";
    float ogPosition[2];
    bool activeInput;
    WriteButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt);
    void update(sf::Vector2f mousePos);
    void draw(sf::RenderWindow *window);
    void addInput(sf::Uint32 input, sf::Vector2u res);
    void delLastInput(sf::Vector2u res);
    void resetInput(void);
    ~WriteButton(void);
};



//menu layer class. contains
class menuLayer {
    public:
    layersId layerType;
    layersId previousLayer;
    std::vector<button*> LayerButtons;
    
    menuLayer(layersId assignedLayer, Application *applicationPointer);//For most basic layers
    void init(layerInformation setUpInfo, Application *applicationPointer);
    void update(sf::Vector2f mousePos);
    
    ~menuLayer(void);
};



#endif //MENU_HPP
#pragma once
#ifndef MENU_HPP
#define MENU_HPP
class Application;
class menuLayer;


//Makros for Menu Layers
#define MaxVisibleLayers 8
enum layersId{
    leave = -1, 
    Base = 0, 
    Settings = 1,
    Type = 2, 
    Multiplayer = 3, 
    Join = 4, 
    Host = 5, 
    Graphic = 6,
    Controls = 7,
    final = 8
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
    ClickButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt);
    void draw(sf::RenderWindow *window);
    void update(sf::Vector2f mousePos);

    void changeRes(Application *applicationPointer, int maxButt, int currButt);
    
    ~ClickButton(void);
};



//
class GraphicButton : public button{
    public:
    sf::Vector2u newResolution;
    GraphicButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt, sf::Vector2u newRes);
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
    WriteButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt);
    void update(sf::Vector2f mousePos);
    void draw(sf::RenderWindow *window);

    void changeRes(Application *applicationPointer, int maxButt, int currButt);
    
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

    void changeRes(Application *applicationPointer);
    
    ~menuLayer(void);
};



#endif //MENU_HPP
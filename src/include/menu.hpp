#pragma once
#ifndef MENU_HPP
#define MENU_HPP
class Application;

#define MaxVisibleLayers 6
enum layersId{
    leave = -1, 
    Base = 0, 
    Settings = 1, 
    Play = 2, 
    Type = 3, 
    Join = 4, 
    Host = 5, 
    final = 6
};

#define InputMax_IpAdress 15
#define InputMax_Port 5

typedef struct layerInformation{
    layersId lay;
    layersId prevLay;
    int nButtons;//Only default buttons. In host Layer, nondefault buttons are loaded
    std::vector<std::string> buttonTexts;
    std::vector<layersId>followUpLay;
}layerInformation;



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
class ClickButton : public button {
    public:
    ClickButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt);
    void draw(sf::RenderWindow *window);
    void update(sf::Vector2f mousePos);
    ~ClickButton(void);
};

//Or you are able to write
class WriteButton : public button {
    public:
    int maxInput;
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
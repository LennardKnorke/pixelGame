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
    std::string stringText;
    sf::Text text;
    sf::IntRect rec;
    virtual void draw(sf::RenderWindow *window);
    virtual void update(void);
};
class ClickButton : public button {
    public:
    ClickButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt);
    void draw(sf::RenderWindow *window);
    void update(void);
    ~ClickButton(void);
};

//Or you are able to write
class WriteButton : public button {
    WriteButton(std::string t, layersId followLayer, Application *applicationPointer);
    void update(void);
    void draw(sf::RenderWindow *window);
    ~WriteButton(void);
};


class menuLayer {
    public:
    layersId layerType;
    layersId previousLayer;
    std::vector<button*> LayerButtons;

    menuLayer(layersId assignedLayer, Application *applicationPointer);//For most basic layers
    void init(layerInformation setUpInfo, Application *applicationPointer);
    void update(void);
    ~menuLayer(void);
};

#endif //MENU_HPP
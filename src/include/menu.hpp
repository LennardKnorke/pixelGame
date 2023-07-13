#pragma once
#ifndef MENU_HPP
#define MENU_HPP

//MAKROS
//Used for assigning the right layers to each button
#define Layer_leave -1
#define LAYER_BASE 0
#define LAYER_SETTINGS 1
#define LAYER_PLAY 2
#define LAYER_JOIN 3
#define LAYER_HOST 4
#define LAYER_HOST_SAVE 5
#define LAYER_HOST_NEWSAVE 6
#define LAYER_HOST_OLDSAVE 7
#define Layer_final 8
//Play, Settings, Exit (LAYER_BASE 3 buttons, 3 CLICKING)
//                      (LAYER_SETTINGS ?Button ? ?)
//Host, Join            (LAYER_PLAY, 2 Button, 2 CLICKING)
//EnterIp, Enter Port   (LAYER_JOIN, 2 Button, 2 WRITING)
//Online, Local         (LAYER_HOST 2 Button, 2 CLICKING)
//CreateSave, LoadSave  (LAYER_HOST_SAVE 2 Button, 2CLICKING)
//Name                  (LAYER_HOST_NEWSAVE 1 Button, 1 WRITE)
//                      (LAYER_HOST_OLDSAVE....SAVE FILE BUTTONS == unknown)
//Helps creating the buttons
#define MAX_DEFAULTBUTTONS 12
#define MAX_DEFAULTBUTTONS_WRITING 3
#define MAX_DEFAULTBUTTONS_CLICK 9


#define InputMax_IpAdress 15
#define InputMax_Port 5
//Help to load menu textures
#define nr_menu_textures 1

class Button {
    public:
    std::string stringText;
    sf::Text text;
    sf::IntRect rec;
    bool focus;
    bool pressed;
};

//Either you can click a buttons and you go to the next layer
class Menu_Button_Click : public Button {
    private:
    int layer;
    int nextLayer;
    public:
    int idx;
    int maxIdx;
    void update(sf::Vector2f mouseposition, bool click);
    void init(  std::string display_text,
                int textureIdx, 
                int currentlayer, 
                int buttonIdx, 
                int maxButtonIdx,
                int followUpLayer
                );
    void updatePosition(sf::Vector2u res);
    int getLayer(void);
    int getNextLayer(void);
    void draw(sf::RenderWindow *window);
};
//Or you are able to write
class Menu_Button_Write : public Button{
    public:
    bool active_input = false;
    int currentInput;
};

//Basically just structures the buttons in some way.
typedef struct MenuButtons {
    public:
    std::vector<Menu_Button_Click> SaveFileButtons;
    Menu_Button_Write WriteButtons[MAX_DEFAULTBUTTONS_WRITING];
    Menu_Button_Click ClickButtons[MAX_DEFAULTBUTTONS_CLICK];
}MenuButtons;


class menuLayer {

};

class menu{
    public:
    menu(void);
    void draw(void);
    void handleInput(void);
    void update(void);
    void updateResolution(void);
    

};
#endif //MENU_HPP
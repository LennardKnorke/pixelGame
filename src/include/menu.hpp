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
//                      (LAYER_HOST_OLDSAVE ?Button)
//Helps creating the buttons
#define MAX_DEFAULTBUTTONS 12
#define MAX_DEFAULTBUTTONS_WRITING 3
#define MAX_DEFAULTBUTTONS_CLICK 9

//Help to load menu textures
#define nr_menu_textures 1

class Button {
    public:
    std::string displayText;
    Rectangle rec;
};

class Menu_Button_Click : public Button {
};

class Menu_Button_Write : public Button{
    public:
    bool active_input = false;
};


typedef struct MenuButtons {
    Menu_Button_Click *SaveFileButton;
    Menu_Button_Write WriteButtons[MAX_DEFAULTBUTTONS_WRITING];
    Menu_Button_Click ClickButtons[MAX_DEFAULTBUTTONS_CLICK];
} MenuButtons;
#endif //MENU_HPP
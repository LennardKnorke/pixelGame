#pragma once
#ifndef MENU_HPP
#define MENU_HPP

//LAYER MAKROS
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
//Play, Settings, Exit (LAYER_BASE 3 buttons)
//                      (LAYER_SETTINGS ?Button)
//Host, Join            (LAYER_PLAY, 2 Button)
//EnterIp, Enter Port   (LAYER_JOIN, 2 Button)
//Online, Local         (LAYER_HOST 2 Button)
//CreateSave, LoadSave  (LAYER_HOST_SAVE 2 Button)
//Name                  (LAYER_HOST_NEWSAVE 1 Button)
//                      (LAYER_HOST_OLDSAVE ?Button)
#define MAX_DEFAULTBUTTONS 12

#define nr_menu_textures 1

class Button {
    public:

    bool focused;
    bool clicked;

    int next_layer;
    int layer;

    std::string text_as_string;
    sf::Text text;
    sf::FloatRect text_size;
    void create(std::string displayText, int followLayer,int layer, int displayIndex, sf::Vector2u res, sf::Font font);    
    void update(sf::Vector2i mouse_position, bool click);
};

#endif //MENU_HPP
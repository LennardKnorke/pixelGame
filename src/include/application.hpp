#pragma once
#ifndef APPLICATION_HPP
#define APPLICATON_HPP
#include "stdlibs.hpp"
#include "menu.hpp"


#define MAX_LENGTH_KEY 14
#define STATE_MENU 1
#define STATE_GAME 2
#define STATE_QUIT 0 

#define nr_cursor_textures 2
Rectangle createRectangle(int height, int width, int posX, int posY);

typedef struct Textures {
    sf::Texture cursors[nr_cursor_textures];
    sf::Texture menu[nr_menu_textures];
    //sf::Texture character[nr_character_textures];
    //sf::
} Textures;


class CursorSprite {
    public:
    bool click;
    Rectangle rec;
    sf::Sprite sprite[nr_cursor_textures];
    int activeSprite;
    CursorSprite(void);
    void changeSprite(int i);
    void update(void);
};

class Application{
    public:
    Application(void);
    //Window related variables
    sf::RenderWindow window;
    sf::Vector2u resolution;
    CursorSprite cursor;
    
    //////
    //Grafik Variables
    sf::Font gameFont;
    Textures textures;
    
    //Menu related
    MenuButtons Buttons_Menu;
    std::vector<gameSaveSummary> availableSaveFiles;
    //Socket variables
    sf::IpAddress publicAdress = sf::IpAddress::getPublicAddress();
    sf::IpAddress localAdress = sf::IpAddress::getLocalAddress();


    //Application variables
    int applicationState;   //0=quit, 1= run menu, 2=run game
    int errorHandling;      //0= no error...
    bool applicationFocused;
    char userKey[MAX_LENGTH_KEY + 1];

    //SavefileManagement

    ////////////////FUNCTIONS////////////////
    //Boot ups
    void initWindow(void);
    void initSettings(void);
    bool loadAssets(void);
    bool loadTextures(void);
    void connectTexturesWClasses(void);

    void readAllSaveFiles(void);
    gameSaveSummary loadSaveSummary(const std::string &filename);
    void writeSaveFile(gameSave Save);
    //Orga Stuff
    void loadSettings(const std::string &filename);
    void createSettings(const std::string &filename);
    void saveSettings(const std::string &filename);

    bool fileExists(const std::string &filename);

    bool RectangleCollision(Rectangle rec_1, Rectangle rec_2);
    //Menu functions
    int menu(void);
    void initializeMenu(void);


    //game(client) functions
    int game(void);


    //?server functions?
};


#endif //APPLICATION_HPP
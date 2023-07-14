#pragma once
#ifndef APPLICATION_HPP
#define APPLICATON_HPP
#include "stdlibs.hpp"
#include "menu.hpp"


#define MAX_LENGTH_KEY 14

enum GAME_STATE {
    MENU, GAME, QUIT
};
enum errorCodes{
    ConnectErr, FileLoadErr, InvalidSaveNameErr, NoErr
};

//overview of how much textures we want to load
#define nr_cursor_textures 2
#define nr_menu_textures 1
typedef struct Textures {
    sf::Texture cursors[nr_cursor_textures];
    sf::Texture menu[nr_menu_textures];
    //sf::Texture character[nr_character_textures];
    //sf::
} Textures;


class CursorSprite {
    private:
    int activeSprite;
    public:
    sf::Sprite sprite[nr_cursor_textures];
    bool pressed;
    void changeSprite(int i);
    void update(void);
    void draw(sf::RenderWindow *renderwindow);
    sf::Vector2f returnPosition(void);
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
    
    std::vector<gameSaveSummary> availableSaveFiles;
    //Socket variables
    sf::IpAddress publicAdress = sf::IpAddress::getPublicAddress();
    sf::IpAddress localAdress = sf::IpAddress::getLocalAddress();


    //Application variables
    GAME_STATE State;
    errorCodes error = NoErr;

    char userKey[MAX_LENGTH_KEY + 1];

    //SavefileManagement

    ////////////////FUNCTIONS////////////////
    //Boot ups
    void initWindow(void);
    void initSettings(void);
    bool loadAssets(void);
    bool loadTextures(void);
    void setUpCursorAssets(void);

    void readAllSaveFiles(void);
    gameSaveSummary loadSaveSummary(const std::string &filename);
    void createSaveFile(gameSave Save);
    //Orga Stuff
    void loadSettings(const std::string &filename);
    void createSettings(const std::string &filename);
    void saveSettings(const std::string &filename);

    bool fileExists(const std::string &filename);

    //Menu functions
    

    //game(client) functions
    GAME_STATE menuLoop(void);
    GAME_STATE gameLoop(void);


    //?server functions?
};


#endif //APPLICATION_HPP
#pragma once
#ifndef APPLICATION_HPP
#define APPLICATON_HPP
#include "stdlibs.hpp"
#include "menu.hpp"


#define MAX_LENGTH_KEY 14
#define MAX_LENGTH_SAVENAME 12
#define MAX_LENGTH_IPPORT 20

enum GAME_STATE {
    MENU, GAME, QUIT
};
enum errorCodes{
    ConnectErr, FileLoadErr, InvalidSaveNameErr, NoErr
};
enum gameMode{
    Single, Local, Online
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

#define GAMESTATE_QUIT 0
class CursorSprite {    
    public:
    int activeSprite;
    sf::Sprite sprite[nr_cursor_textures];
    bool pressed;
    void changeSprite(int i);
    void update(void);
    void draw(sf::RenderWindow *renderwindow);
    sf::Vector2f returnPosition(void);
    sf::Vector2f returnSize(void);
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
    
    std::string hostIp = "";
    std::string hostPort = "";
    //Application variables
    GAME_STATE State;
    errorCodes error = NoErr;
    gameSave activeSave;
    bool wantsHost;
    gameMode mode;
    char userKey[MAX_LENGTH_KEY + 1];

    //SavefileManagement

    ////////////////FUNCTIONS////////////////
    //Boot ups
    void initWindow(void);
    void initSettings(void);
    bool loadAssets(void);
    //assets loading
    bool loadTextures(void);
    void setUpCursorAssets(void);
    //save functions
    void setUpSaveFolder(void);
    void readAllSaveFiles(void);
    gameSaveSummary loadSaveSummary(const std::string &filename);
    bool createSaveFile(std::string newSaveName);
    void saveSave(gameSave Save, const std::string &path);
    gameSave loadSave(const std::string &filename);
    //Orga Stuff
    void loadSettings(const std::string &filename);
    void createSettings(const std::string &filename);
    void saveSettings(const std::string &filename);

    bool fileExists(const std::string &filename);

    //Menu functions
    void getMenuPicks(sf::Vector2f cursorPosition, layersId currentLayer, menuLayer *Lay);
    bool checkCharacterInput(layersId activeLayer, sf::Uint32 c, int activeLength);
    
    //game(client) functions
    GAME_STATE menuLoop(void);
    GAME_STATE gameLoop(void);


    //?server functions?
};


#endif //APPLICATION_HPP
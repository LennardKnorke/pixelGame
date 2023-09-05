#pragma once
#ifndef APPLICATION_HPP
#define APPLICATON_HPP
#include "stdlibs.hpp"
#include "menu.hpp"
#include "gamesave.hpp"



//checks if a relative file exists
bool fileExists(const std::string &filename);



//Makros
#define MAX_LENGTH_KEY 14
#define MAX_LENGTH_SAVENAME 12
#define MAX_LENGTH_IPPORT 20



//makros about the gamestate
enum GAME_STATE {
    MENU, GAME, QUIT
};



//makro for additional error codes (unused so far)
enum errorCodes{
    ConnectErr, FileLoadErr, InvalidSaveNameErr, NoErr
};



//tracks whether the user wants to play locally, online or alone
enum gameMode{
    Single, Local, Online
};



//overview of how much textures we want to load
#define nr_cursor_textures 2
#define nr_menu_textures 1
//makro the menu textures
enum menuTextureIdxS {
    background =0
};



//One instance for all the textures
typedef struct Textures {
    sf::Texture cursors[nr_cursor_textures];
    sf::Texture menu[nr_menu_textures];
    sf::Sprite menu_sprites[nr_menu_textures];
    //sf::
} Textures;



//makro to index the cursorsprites
enum cursorSpriteIndexes {
    menu = 0, game_base = 1
};



//class for the cursor
class CursorSprite {    
    public:
    cursorSpriteIndexes activeSprite;
    sf::Sprite sprite[nr_cursor_textures];
    bool pressed;
    void changeSprite(cursorSpriteIndexes i);
    void update(void);
    void draw(sf::RenderWindow *renderwindow);
    sf::Vector2f returnPosition(void);
    sf::Vector2f returnSize(void);
};



//Application. Main thing
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
    gameSave *activeSave;
    bool wantsHost;
    gameMode mode;
    char userKey[MAX_LENGTH_KEY + 1];

    //SavefileManagement

    ////////////////FUNCTIONS////////////////
    //game(client) functions
    GAME_STATE menuLoop(void);
    GAME_STATE gameLoop(void);
    //Boot ups
    void initWindow(void);
    void initSettings(void);
    bool loadAssets(void);
    //assets loading
    bool loadTextures(void);
    void setUpCursorAssets(void);
    void setUpTextureAssets(void);
    //save functions
    void setUpSaveFolder(void);
    void readAllSaveSummaries(void);
    gameSaveSummary loadSaveSummary(const std::string &filename);
    bool createSave(std::string newSaveName, menuPopUps *menuWarning);
    //Orga Stuff
    void loadSettings(const std::string &filename);
    void createSettings(const std::string &filename);
    void saveSettings(const std::string &filename);


    //Menu functions. found in menu.cpp
    void getMenuPicks(sf::Vector2f cursorPosition, layersId currentLayer, menuLayer *Lay);
    bool checkCharacterInput(layersId activeLayer, sf::Uint32 c, int activeLength);
    void setActiveSafe(std::string saveName);
    void setMenuPopUpMessage(menuPopUps PopUp, sf::Text *warningMessage, sf::Vector2u res);
    void drawMenuPopUp(menuPopUps PopUp, sf::RenderWindow *window, sf::Text *warningMessage);
    


    //?server functions?
};



#endif //APPLICATION_HPP
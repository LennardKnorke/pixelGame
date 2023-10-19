#pragma once
#ifndef APPLICATION_HPP
#define APPLICATON_HPP
#include "stdlibs.hpp"


//checks if a relative file exists
bool fileExists(const std::string &filename);


//makros about the gamestate
enum GAME_STATE {
    MENU, GAME, QUIT
};


//makro for additional error codes (unused so far)
enum errorCodes{
    ConnectErr, FileLoadErr, InvalidSaveNameErr, NoErr
};


//overview of how much textures we want to load
#define nr_cursor_textures 2
#define nr_menu_textures 4
#define MAX_N_SAVES 5


//makro the menu textures
enum menuTextureIdxS {
    background = 0,
    button_silver = 1,
    button_red = 2,
    button_green = 3
};


//One instance for all the textures
typedef struct Textures {
    sf::Texture cursors[nr_cursor_textures];
    sf::Texture menu[nr_menu_textures];
} Textures;


enum inputType {
    KEYBOARD, MOUSE_BUTTON
};


typedef struct inGameInputKey {
    inputType iType;
    union inputUnion{
        sf::Keyboard::Key keyInput;
        sf::Mouse::Button mouseInput;
    }input;
}inGameInputKey;


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
    bool print;
    void changeSprite(cursorSpriteIndexes i);
    void update(void);
    void draw(sf::RenderWindow *renderwindow);
    void changeRes(void);
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
    float ratioScaling;
    CursorSprite cursor;
    //////
    //Grafik Variables
    sf::Font gameFont;
    Textures textures;
    
    std::vector<gameSaveSummary> availableSaveFiles;
    //Socket variables
    sf::IpAddress machinePublicAdress = sf::IpAddress::getPublicAddress();
    sf::IpAddress machineLocalAdress = sf::IpAddress::getLocalAddress();
    
    struct socketAdress {
        sf::IpAddress ip = sf::IpAddress::None;   
        unsigned short port = 0;
        std::string pathSave;
    }hostAdress;
    //Application variables
    GAME_STATE State;               //Main Menu, Gameloop or Ending Game
    gameMode mode = undefined;      //Online, Local or Alone
    errorCodes error = NoErr;

    inGameInputKey inGameControls[7];
    std::string localUserID;

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
    
    //Orga Stuff
    void loadSettings(const std::string &filename);
    void createSettings(const std::string &filename);
    void saveSettings(const std::string &filename);


    //Menu functions. found in menu.cpp
    GAME_STATE menuLoop(void);

    //GameLoopFunctions
    GAME_STATE gameLoop(void);
        gameLoopState loadingScreen(Clients *socket);
        void drawGame(void);
        void registerGameInput(gameLoopState *s, bool *gamePlayInput);
            void readUserGameInput(sf::Vector2i origin, bool *gamePlayInput);
        void updateGame(void);

        void drawMenu(std::vector<inGameMenuButton*> v);
        void registerMenuInput(gameLoopState *s, std::vector<inGameMenuButton*> v);
        void updateMenu(std::vector<inGameMenuButton*> v);

        void drawTree(void);
        void registerTreeInput(gameLoopState *s);
        void updateTree(void);
};



#endif //APPLICATION_HPP
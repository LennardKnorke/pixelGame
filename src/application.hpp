//For the Game Class object
//Application.cpp will contain the functions
#pragma once
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "stdlibs.hpp"
#include "menu.hpp"


#define MAX_LENGTH_KEY 14


#define APPLICATIONSTATE_QUIT 0
#define APPLICATIONSTATE_MENU 1
#define APPLICATIONSTATE_GAME 2

#define N_MENU_TEXTURES 6
#define N_CHARACTER_TEXTURES 1
#define N_TILE_TEXTURES 1
#define N_WEAPON_TEXTURES 1
#define N_SFX_TEXTURES 1

#define idx_Menu_Background 0
#define idx_Menu_Cursor 1
#define idx_Menu_Button 2
#define idx_Menu_ButtonClick 3
#define idx_Menu_World 4
#define idx_Menu_WorldClick 5



typedef struct windowFeatures{
    bool fullScreen;
    int screenHeight;
    int screenWidth;
    int windowHeight;
    int windowWidth;
} windowFeatures;

typedef struct characterSprites {
    Texture2D idle;
    Texture2D walking;
    Texture2D jumping;
    Texture2D falling;
} characterSprites;

typedef struct spriteGroup {
    Texture2D menuSprites[N_MENU_TEXTURES];
    characterSprites character;
} spriteFolder;

class GameApplication {
    public:
    void runApplication(void);

    private:
    //Variables for the state of the application
    int applicationState;   //0=quit, 1= run menu, 2=run game
    int errorHandling;      //0= no error...
    bool applicationRunning;
    bool applicationFocused;
    Vector2 mousePosition;

    windowFeatures windowSettings;
    char userKey[MAX_LENGTH_KEY + 1];
    bool isHost;
    
    spriteFolder sprites;
    
    int runMenu(void);
    int runGame(void);
    int runServer(void);
    void loadSettings(void);
    void createSettings(void);
    void loadSprites(void);
    void unloadSprites(void);
    void changeResolution(void);
};


#endif //APPLICATION_HPP
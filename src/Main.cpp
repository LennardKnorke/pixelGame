
////GAME
//Standart libs
#include <iostream>
#include <fstream>
#include <thread>
//External libs
#include "nlohmann/json.hpp"
#include <raylib.h>
//UP here include setting type of functions and game functions and classes at the bottom
//Open and returns json file
nlohmann::json read_json_file (const std::string &path){
    std::ifstream input_file(path);
    nlohmann::json j;
    input_file >> j;
    return j;
}
//NOTES:
//2 options for the network. either the host does all the operations as part of his game cycle
//or the og server idea.

//How about a list, containing the following struct, is always given to the draw function
//Every picture


int main (int argc, char *args[]){
    
    nlohmann::json configs = read_json_file("../config.json");
    std::string currentProfile = configs["currentProfile"];
    //Load settings, screen info 
    int screenWidth = 0, screenHeight = 0, screenNumber = 0;
    bool fullScreen;
    InitWindow(screenWidth, screenHeight, "Fuck yeah I'm a god");
    //If its the first time starting the game, look for default options and save them in the config
    if (configs["first"] == true){
        screenHeight = GetScreenHeight();
        screenWidth = GetScreenWidth();
        screenNumber = GetCurrentMonitor();
        configs["first"] = false;
        configs["screen_settings"]["width"] = screenWidth;
        configs["screen_settings"]["height"] = screenHeight;
        configs["screen_settings"]["fullScreen"] = true;
        fullScreen = true;
        std::ofstream output_file("config.json");
        output_file << configs.dump(1) << std::endl;
    }
    //Or read out information of the config file
    else {
        screenHeight = configs["screen_settings"]["height"];
        screenWidth = configs["screen_settings"]["width"];
        screenNumber = configs["screen_settings"]["monitorId"];
        if (configs["screen_settings"]["fullScreen"] == true){
            fullScreen = true;
        } else {
            fullScreen = false;
        }
    }
    //Finalize Starting Window
    SetWindowMonitor(screenNumber);
    SetWindowSize(screenWidth, screenHeight);
    if ((IsWindowFullscreen() == true && fullScreen == false) || (IsWindowFullscreen() == false && fullScreen == true)){
        ToggleFullscreen();
    }
    SetTargetFPS(60);

    //STOP IF ANYTHING FAILED!!!
    if (IsWindowReady() == false){
        std::cout << "Error setting up application window" << std::endl;
        system("pause");
        return 0;
    }
    Image menuBackground = LoadImage("../sprites/menu/bgmenu.png");
    ImageResize(&menuBackground, 1920, 1080);
    Texture2D menuBackground2 = LoadTextureFromImage(menuBackground);

    //initial idea. if
    //0:    load menu (if necessary)
    //1:    Menu
    //2:    load world
    //3:    world
    Texture2D currentlyDisPlayedSprites[1];
    while (!WindowShouldClose()){
        
        BeginDrawing();
            ClearBackground(BLACK);
            
            DrawTexture(menuBackground2, 0, 0, WHITE);
        EndDrawing();

    }
    UnloadTexture(menuBackground2);
    

    
    return 0;
}

Texture2D* loadMenuSprites(void){

}

void printSprites (Texture2D *Sprites){
    BeginDrawing();
    for (int i = 1; i < sizeof(Sprites) / sizeof(Texture2D); i++){

    }
    EndDrawing();
}
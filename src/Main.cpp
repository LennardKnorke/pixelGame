////GAME
//Standart libs
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <thread>
#include <random>
#include <vector>

//External libs
#include "nlohmann/json.hpp"
#include <raylib.h>
//#include "SFML/Network.hpp"

//Own functions
#include "declr.h"

namespace gV {
    int screenWidth;
    int screenHeight;
    int screenNumber;
    float screenRatio; //can all be reduced to one number if we limit to 16:9
    bool fullScreen;
    int GAME_STATE;
    std::string own_Ip_Adress;
    std::string host_Ip_Adress;
    int client_Port;
    int host_Port;
    std::string activeProfileName;
    std::string activeProfileKey;
}


//Main script. loads default settings, creates configs and loads menu texture before going into the game cycle
//Within the game cycle one may switch between 1. the menu, 2. loading (connecting to server etc..) and 3.The actual game.
int main (int argc, char *args[]){
    
    //Load settings, screen infos and stuff
    nlohmann::json configs = read_json_file("../config.json");
    
    //init audio and make window minimizeable
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    
    InitWindow(0, 0, "Fuck yeah I'm a god");
    //If its the first time starting the game, look for default options and save them in the json
    if (configs["first"] == true){
        
        gV::screenHeight = GetScreenHeight();
        gV::screenWidth = GetScreenWidth();
        gV::screenNumber = GetCurrentMonitor();
        //standardize into 16:9? just in case
        if (gV::screenHeight >= 1080 && gV::screenWidth >= 1920){
            gV::screenHeight = 1080;
            gV::screenWidth = 1920;
        } else if (gV::screenHeight >= 720 && gV::screenWidth >= 1280){
            gV::screenHeight = 1080;
            gV::screenWidth = 1920;
        } else {
            gV::screenHeight = 540;
            gV::screenWidth = 960;
        }
        //save in config and then in json
        configs["first"] = false;
        configs["screen_settings"]["width"] = gV::screenWidth;
        configs["screen_settings"]["height"] = gV::screenHeight;
        configs["screen_settings"]["fullScreen"] = true;
        gV::fullScreen = true;
        std::ofstream output_file("../config.json");
        output_file << configs.dump(1) << std::endl;
        output_file.close();
    }
    //Or read out information of the config file
    else {
        gV::screenHeight = configs["screen_settings"]["height"];
        gV::screenWidth = configs["screen_settings"]["width"];
        gV::screenNumber = configs["screen_settings"]["monitorId"];
        gV::fullScreen = configs["screen_settings"]["fullScreen"];
    }
    gV::screenRatio = (float)gV::screenWidth / (float)1920;


    //Set Up Window
    SetWindowMonitor(gV::screenNumber);
    SetWindowSize(gV::screenWidth, gV::screenHeight);
    if ((IsWindowFullscreen() == true && gV::fullScreen == false) || (IsWindowFullscreen() == false && gV::fullScreen == true)){
        ToggleFullscreen();
    }
    SetTargetFPS(60);HideCursor();

    //STOP IF ANYTHING FAILED!!!
    if (IsWindowReady() == false){
        std::cout << "Error setting up application window" << std::endl;
        system("pause");
        return 0;
    } else {
        gV::GAME_STATE = 0;
    }
    //Load images necessary for the menu
    std::string menuItemList[NUMBER_OF_DIFFERENT_MENU_TEXTURES] = {"bgmenu", "curs", "default_button", "default_button_clicked", "world", "world_clicked"};
    Image preprocessedMenuImages[NUMBER_OF_DIFFERENT_MENU_TEXTURES];
    for (int i = 0 ; i < NUMBER_OF_DIFFERENT_MENU_TEXTURES; i++){
        std::string fileName = ("../sprites/menu/"+menuItemList[i]+".png");
        const char *cFileName = fileName.c_str();
        preprocessedMenuImages[i] = LoadImage(cFileName);
    }


    //DO THE SAME FOR CHAR AND INGAME SPRITES
/*
    Image preprocessedWorldSprites[NUMBER_WORLD_TEXTURES];
    Image preprocessedCharSprites[NUMBER_CHAR_TEXTURES];
    Image preprocessedenemySprites[NUMBER_ENEMY_TEXTURES];
*/
    
    
    
    /*Prepare here:  two sockets which either through threading or sync stuff are being prepared during loading phase
        One client socket and one server socket.
        Load images and textures of the loading screen but only load images into cpu waiting and during loading resize and load into gpu
        Add a small note on what could happen during the initial socket connection. No game stuff or shit. just to figure out if it worked.  
    */

   //Main main main loop. switch between menu, loading/connecting and game cycle
    while (gV::GAME_STATE != -1){ 
        switch (gV::GAME_STATE){
            //Menu
            case 0:
                gV::GAME_STATE = run_Menu(preprocessedMenuImages, configs);
            //Connect client socket with host and start host
            case 1:
                gV::GAME_STATE = load_World();
            //Game Loop
            case 2:
                gV::GAME_STATE = play_Game();
        }
    }


    //Unload all images, close window and close all modules
    CloseWindow();
    for (int i = 0; i < NUMBER_OF_DIFFERENT_MENU_TEXTURES; i++){
        UnloadImage(preprocessedMenuImages[i]);
    }  

    
    return 0;
}
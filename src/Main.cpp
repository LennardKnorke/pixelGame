
////GAME
//Standart libs
#include <iostream>
#include <fstream>
#include <thread>
#include <string.h>
//External libs
#include "nlohmann/json.hpp"
#include <raylib.h>
#include <thread>

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





void resizeImageReferenceList (Image imgRefList[6], int newx, int newy);
int main (int argc, char *args[]){
    //test. does not do anything /just to see if the thread works DELETE LATER:
    //SERVER_FUNCTIONS_H::run_server_thread(0);
    
    //Load settings, screen infos and stuff
    nlohmann::json configs = read_json_file("../config.json");
    std::string currentProfile = configs["currentProfile"];
    
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
    float screenRatios [2] = {(float)screenWidth / (float)1920, (float)screenHeight / (float)1080};
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
    //Menu Buttons
    std::string menuItemList[6] = {"bgmenu", "curs", "default_button", "default_button_clicked", "world", "world_clicked"};
    Image menuImage_reference[6];
    Texture2D menuTextures[6];
    for (int i = 0 ; i < 6; i++){
        std::string fileName = ("../sprites/menu/"+menuItemList[i]+".png");
        const char *cFileName = fileName.c_str();
        menuImage_reference[i] = LoadImage(cFileName);
        int s1,s2;
        if (i == 0){
            s1 = 1920; s2 = 1080;
        } else if (i == 1){
            s1 = 50 * screenRatios[0]; s2 = 50 * screenRatios[1];
        } else if (i == 2 || i == 3){
            s1 = 300 * screenRatios[0]; s2 = 100 * screenRatios[1];
        } else if (i == 4 || i == 5){
            s1 = 300 * screenRatios[0]; s2 = 300 * screenRatios[1];
        }
        ImageResize(&menuImage_reference[i], s1, s2);
        menuTextures[i] = LoadTextureFromImage(menuImage_reference[i]);
    }

    clock_t timer = clock();
    while (!WindowShouldClose()){

        
        BeginDrawing();
            ClearBackground(GRAY);
            DrawTexture(menuTextures[0], 0,  0, WHITE);
            DrawTexture(menuTextures[2], (screenWidth / 2) - (menuTextures[2].width / 2), (screenHeight / 2) - (menuTextures[2].height / 2), WHITE);
        EndDrawing();
        
    }


    for (int i = 0; i < 6; i++){
        UnloadImage(menuImage_reference[i]);
        UnloadTexture(menuTextures[i]);
    }  

    
    return 0;
}


void resizeMenuTextures (Image imgRefList[6], int newx, int newy, Texture2D loadedTextures[6]){
    for (int i = 0; i < 6; i++){
        ImageResize(&imgRefList[i], newx, newy);
        UnloadTexture(loadedTextures[i]);
        loadedTextures[i] = LoadTextureFromImage(imgRefList[i]);
    }
}

int runMenu(void){
    return 0;
}
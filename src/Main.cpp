
////GAME
//Standart libs
#include <iostream>
#include <fstream>
#include <thread>
//External libs
#include "nlohmann/json.hpp"
#include <raylib.h>
#include <thread>
#include <sys/socket.h>
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

class imageSprite {
    public:
        int id; 
        Image imageReference;
        int sizeX;
        int sizeY;
        void imageResize(int newsizeX, int newsizeY){
            sizeX = newsizeX;
            sizeY = newsizeY;
            ImageResize(&imageReference, sizeX, sizeY);
        }
        imageSprite(const char * path, int imageid, int x, int y){
            sizeX = x;
            sizeY = y;
            id = imageid;
            //id 0 == Background Menu
            if (id == 0){
                imageReference = LoadImage(path);
            }
            ImageResize(&imageReference, sizeX, sizeY);
        }
};

class textureSprite {
    public:
        Texture2D loadedTexture;
        int posX, posY, textureId;
        textureSprite(Image img, int x, int y){
            loadedTexture = LoadTextureFromImage (img);
            posX = x;
            posY = y;
        }
};

class gameSprite : textureSprite {

};





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
    imageSprite bgmenu(".../sprites/menu/bgmenu.png", 0 , (float)screenWidth * screenRatios[0], (float)screenHeight * screenRatios[1]);
    textureSprite bgmenusprite(bgmenu.imageReference, 0,0);


    clock_t timer = clock();
    while (!WindowShouldClose() && ((clock() - timer) / CLOCKS_PER_SEC) <= 10){
        if (IsWindowFocused() == false){
            MinimizeWindow();
        }

        BeginDrawing();
        DrawTexture(bgmenusprite.loadedTexture, bgmenusprite.posX, bgmenusprite.posY, BLANK);
        EndDrawing();
        
    }
    UnloadTexture(bgmenusprite.loadedTexture);
    

    
    return 0;
}


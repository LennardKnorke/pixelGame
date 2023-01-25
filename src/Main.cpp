
////GAME
//Standart libs
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <thread>
//External libs
#include "nlohmann/json.hpp"
#include <raylib.h>
//#include "SFML/Network.hpp"

//prep functions from other files
extern  void run_server_thread(int port);

//Ease scalability of texture/ button loading
#define NUMBER_OF_DIFFERENT_MENU_TEXTURES 6 
#define NUMBER_OF_DEFAULT_MENU_BUTTONS 12

#define NUMBER_WORLD_TEXTURES 0
#define NUMBER_CHAR_TEXTURES 0
#define NUMBER_ENEMY_TEXTURES 0

//SOME GLOBAL VARIABLES
//relevant for the window:
int screenWidth = 0;
int screenHeight = 0;
int screenNumber = 0;
float screenRatios[2]; //can all be reduced to one number if we limit to 16:9
bool fullScreen;
int GAME_STATE = 0;
//relevant for the network communicaiton
std::string own_Ip_Adress;
std::string host_Ip_Adress;
int client_Port;
int host_Port;
std::string activeProfile;
std::string profileKey;

//Structures and Classes



//UP here include setting type of functions and game functions and classes at the bottom
//Open and returns json file
nlohmann::json read_json_file (const std::string &path){
    std::ifstream input_file(path);
    nlohmann::json j;
    input_file >> j;
    return j;
}




//Other fthen the main Functions
int run_Menu(Image imageRefList[NUMBER_OF_DIFFERENT_MENU_TEXTURES], nlohmann::json configFile);
int play_Game();
int load_World(void);

//Main script. loads default settings, creates configs and loads menu texture before going into the game cycle
//Within the game cycle one may switch between 1. the menu, 2. loading (connecting to server etc..) and 3.The actual game.
int main (int argc, char *args[]){
    
    //Load settings, screen infos and stuff
    nlohmann::json configs = read_json_file("../config.json");
    
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitAudioDevice();
    
    InitWindow(screenWidth, screenHeight, "Fuck yeah I'm a god");
    //If its the first time starting the game, look for default options and save them in the json
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
        fullScreen = configs["screen_settings"]["fullScreen"];
    }
    screenRatios[0] = (float)screenWidth / (float)1920;
    screenRatios[1] = (float)screenHeight / (float)1080;

    //Set Up Window
    SetWindowMonitor(screenNumber);
    SetWindowSize(screenWidth, screenHeight);
    if ((IsWindowFullscreen() == true && fullScreen == false) || (IsWindowFullscreen() == false && fullScreen == true)){
        ToggleFullscreen();
    }
    SetTargetFPS(60);HideCursor();

    //STOP IF ANYTHING FAILED!!!
    if (IsWindowReady() == false){
        std::cout << "Error setting up application window" << std::endl;
        system("pause");
        return 0;
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
    while (GAME_STATE != -1){ 
        switch (GAME_STATE){
            //Menu
            case 0:
                GAME_STATE = run_Menu(preprocessedMenuImages, configs);
            //Connect client socket with host and start host
            case 1:
                GAME_STATE = load_World();
            //Game Loop
            case 2:
                GAME_STATE = play_Game();
        }
    }
    //Unload all images, close window and close all modules
    CloseWindow();
    CloseAudioDevice();
    for (int i = 0; i < NUMBER_OF_DIFFERENT_MENU_TEXTURES; i++){
        UnloadImage(preprocessedMenuImages[i]);
    }  

    
    return 0;
}




int run_Menu(Image imageRefList[NUMBER_OF_DIFFERENT_MENU_TEXTURES], nlohmann::json configFile){
    //Prepare json with active profile information
    nlohmann::json profileFile;
    //Scale images and load into GPU
    Texture2D scaledMenuTextures[NUMBER_OF_DIFFERENT_MENU_TEXTURES];
    int textureSizes [NUMBER_OF_DIFFERENT_MENU_TEXTURES][2] = {{screenWidth, screenHeight}, {50, 50}, {300, 100}, {300, 100}, {300, 100}, {300, 100}};
    for (int m = 0; m < NUMBER_OF_DIFFERENT_MENU_TEXTURES; m++){
        ImageResize(&imageRefList[m], textureSizes[m][0] * screenRatios[0], textureSizes[m][1]* screenRatios[1]);
        scaledMenuTextures[m] = LoadTextureFromImage(imageRefList[m]);
    }

    //Button information. How many default button, how many variable buttons. Then their Text
    int nDefaultButtonPerLayer[6] = {1,3,4,2,1,2};
    int nNonDefaultButtonPerLayer[6] = {configFile["nprofiles"],0,0,0,0,0};
    int nTotalButtons[6] = {(nDefaultButtonPerLayer[0] + nNonDefaultButtonPerLayer[0]), (nDefaultButtonPerLayer[1] + nNonDefaultButtonPerLayer[1]), (nDefaultButtonPerLayer[2] + nNonDefaultButtonPerLayer[2]), (nDefaultButtonPerLayer[3] + nNonDefaultButtonPerLayer[3]), (nDefaultButtonPerLayer[4] + nNonDefaultButtonPerLayer[4]), (nDefaultButtonPerLayer[5] + nNonDefaultButtonPerLayer[5])};
    std::string displayText[6][4] = {{"New Profile", "", ""}, {"Play", "Options", "Exit"}, {"Toggle FullScreen", "High Res", "Mid Res", "Low Res"},{"Host", "Join", ""}, {"New World", "" , ""}, {"Enter Host Ip", "Enter Host Port", ""}};
    //Idx will be used display a maximum number of 4 profiles/ worlds. adapt idx to a maximum of 4 if exactly 4 are available (no new profile/ world available)
    if (nNonDefaultButtonPerLayer[0] >= 4){
        nTotalButtons[0] = 4;
    } else if (nNonDefaultButtonPerLayer[4] >= 4){

    }
    //Info for non-default buttons (profile and world buttons)
    std::string availableProfiles[(int)configFile["nprofiles"]];
    std::string *availableWorlds;

    //Other on screen info
    std::string userInput;
    std::string instructions = "Use Arrow Buttons, 'Entf' and 'Enter' to navigate";

    //Load in available profiles
    for (int i = 0; i < configFile["nprofiles"]; i++){
        std::cout << configFile["profiles"][i]["name"] << std::endl;
        availableProfiles[i] = configFile["profiles"][i]["name"];
    }

    //Navigating through the menu
    int menu_running = 1;
    int Current_Layer = 0;
    int pickedIdx = 0;
    bool userInputBool = false;

    //MENU LOOP
    while (menu_running == 1) {
        //FIRST DRAW
        BeginDrawing();
            ClearBackground(GRAY);
            DrawTexture(scaledMenuTextures[0], 0, 0, WHITE);
            //Draw default buttons available for the current layer
            for (int i = 0 ; i < nDefaultButtonPerLayer[Current_Layer]; i++){
                if (nTotalButtons[Current_Layer] <= 4){
                    if (pickedIdx == i){
                        DrawTexture(scaledMenuTextures[3], (screenWidth / 2) - (imageRefList[2].width / 2), ((screenHeight / (nTotalButtons[Current_Layer] + 1)) * (1 + i)) - (imageRefList[2].height / 2), WHITE);
                    } else {
                        DrawTexture(scaledMenuTextures[2], (screenWidth / 2) - (imageRefList[2].width / 2), ((screenHeight / (nTotalButtons[Current_Layer] + 1)) * (1 + i)) - (imageRefList[2].height / 2), WHITE);
                    }
                    DrawText(displayText[Current_Layer][i].c_str(), (screenWidth / 2) - (MeasureText(displayText[Current_Layer][i].c_str(), 30 * screenRatios[1]) / 2), ((screenHeight / (nTotalButtons[Current_Layer] + 1)) * (1 + i)) - ((30 * screenRatios[1])/2), 30*screenRatios[1], BLACK);

                }
            }

            //Draw non-default buttons available for the current layer
            for (int i = 0 ; i < nNonDefaultButtonPerLayer[Current_Layer]; i++){
                if (pickedIdx == i + nDefaultButtonPerLayer[Current_Layer]){
                    DrawTexture(scaledMenuTextures[3], (screenWidth / 2) - (imageRefList[2].width / 2), ((screenHeight / (nTotalButtons[Current_Layer] + 1)) * (1 + nDefaultButtonPerLayer[Current_Layer] + i)) - (imageRefList[2].height / 2), WHITE);
                } else {
                    DrawTexture(scaledMenuTextures[2], (screenWidth / 2) - (imageRefList[2].width / 2), ((screenHeight / (nTotalButtons[Current_Layer] + 1)) * (1 + nDefaultButtonPerLayer[Current_Layer] + i)) - (imageRefList[2].height / 2), WHITE);
                }
                if (Current_Layer == 0){
                    DrawText(availableProfiles[i].c_str(), (screenWidth / 2) - (MeasureText(availableProfiles[i].c_str(), 30 * screenRatios[1]) / 2), ((screenHeight / (nTotalButtons[Current_Layer] + 1)) * (1 + nDefaultButtonPerLayer[Current_Layer] + i)) - ((30 * screenRatios[1])/2), 30*screenRatios[1], BLACK);
                }
            }

            //Draw menu instructions. like use arrows and enter and hist.
            if (Current_Layer == 0){
                DrawText(instructions.c_str(), (screenWidth / 2) - (MeasureText(instructions.c_str(), 18 * screenRatios[1]) / 2), screenHeight - ((18 * screenRatios[1])), 18 * screenRatios[1], BLACK);
            }

            //Draw developmental tools
            DrawFPS(0,0);
            DrawLine(0, screenHeight/2, screenWidth, screenHeight/2, ORANGE);
            DrawLine(screenWidth/2, 0, screenWidth/2, screenHeight, ORANGE);

        EndDrawing();

        //KEY INPUT
        if (IsKeyPressed(KEY_ESCAPE) == true){
            if (Current_Layer == 0){
                menu_running = -1;
            } else if (userInputBool == true){
                userInputBool = false;
            } else {
                Current_Layer -= 1;
            }
        } else if (IsKeyPressed(KEY_UP)){
            pickedIdx -= 1;
            if (pickedIdx < 0){
                pickedIdx = nTotalButtons[Current_Layer] - 1;
            }
        } else if (IsKeyPressed(KEY_DOWN)){
            pickedIdx += 1;
            if (pickedIdx >= nTotalButtons[Current_Layer]){
                pickedIdx = 0;
            }
        } else if (IsKeyPressed(KEY_ENTER)){
            if (Current_Layer == 0){
                if (pickedIdx == 0 && nTotalButtons[Current_Layer] <= 4){
                    //CREATE NEW PROFILE
                    Current_Layer = 1;
                    pickedIdx = 0;
                } else {
                    if (nTotalButtons[Current_Layer] <= 4){
                        activeProfile = availableProfiles[pickedIdx - 1];
                    }
                    Current_Layer = 1;
                    pickedIdx = 0;
                }
            } else if (Current_Layer == 1){
                if (pickedIdx == 0){
                    Current_Layer = 3;
                } else if (pickedIdx == 1){
                    Current_Layer = 2;
                } else if (pickedIdx == 2){
                    menu_running = -1;
                }
            } else if (Current_Layer == 2){
                if (pickedIdx == 0){

                } else if (pickedIdx == 1){

                } else if (pickedIdx == 2){
                    
                }else if (pickedIdx == 3){
                    
                }
            } else if (Current_Layer == 3){
                if (pickedIdx == 0){
                    Current_Layer = 4;
                    pickedIdx = 0;
                } else if (pickedIdx == 1){
                    Current_Layer = 5;
                    pickedIdx = 0;
                }
            }
        }
        
        //Minimize window if not in focus
        //Who thought, not making this a configuration set at the beginning, is a good idea...
        if (IsWindowFocused() == false){
            MinimizeWindow();
        } 
        
    }

    //Unload Texture.    
    for (int i = 0; i < NUMBER_OF_DIFFERENT_MENU_TEXTURES; i++){
        UnloadTexture(scaledMenuTextures[i]);
    }

    //initiate new part of the game loop
    return menu_running;
}


//connect client and load, scale images
int load_World(void){
    return -1;
}

//GAME LOOOOOP
int play_Game(void){
    return -1;
}
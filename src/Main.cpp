
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

//Call prepare functions from other scripts
extern  void run_server_thread(int port);
//Ease scalability of texture/ button loading

#define NUMBER_OF_DIFFERENT_MENU_TEXTURES 6 
#define NUMBER_OF_DEFAULT_MENU_BUTTONS 9

#define NUMBER_WORLD_TEXTURES 0
#define NUMBER_CHAR_TEXTURES 0
#define NUMBER_ENEMY_TEXTURES 0

//SOME GLOBAL VARIABLES
int screenWidth = 0;
int screenHeight = 0;
int screenNumber = 0;
float screenRatios[2];
int GAME_STATE = 0;
//
std::string own_Ip_Adress;
std::string host_Ip_Adress;
int client_Port;
int host_Port;
std::string activeProfile;
std::string profileKey;

//Structures and Classes
class Layer {
    public:
    int LayerNr;
    int nDefaultButtons;
    int nNonDefaultButtons;
    int nButtons;
    Image *imageReferences;
    Texture2D *layerLoadedTextures;
    int *positions_X;
    int *positions_Y;
    void initiate (int lay, int nProfiles, int nWorlds, Image UnprocessedImages[], int fontSize){
        LayerNr = lay;
        const char *textToDraw[7];
        int buttonindexing = 0;
        //STARTING/ PICK PROFILE
        if (LayerNr == 0){
            nDefaultButtons = 1;
            nNonDefaultButtons = nProfiles;
            textToDraw[0] = "NEW PROFILE";
        //MAIN SCREEN
        } else if (LayerNr == 1){
            nDefaultButtons = 3;
            nNonDefaultButtons = 0;
            textToDraw[0] = "PLAY";
            textToDraw[1] = "OPTIONS";
            textToDraw[2] = "EXIT";
        //OPTIONS
        } else if (LayerNr == 2){
            nDefaultButtons = 0;
            nNonDefaultButtons = 0;
        //HOST OR JOIN
        } else if (LayerNr == 3){
            nDefaultButtons = 2;
            nNonDefaultButtons = 0;
            textToDraw[0] = "HOST";
            textToDraw[1] = "JOIN";
        //HOSTING. CREATE OR PICK WORLD
        } else if (LayerNr == 4){
            nDefaultButtons = 1;
            nNonDefaultButtons = nWorlds;
            textToDraw[0] = "NEW WORLD";
        //JOINING. ENTER IP AND PORT
        } else if (LayerNr == 5){
            nDefaultButtons = 2;
            nNonDefaultButtons = 0;
            textToDraw[0] = "HOST IP";
            textToDraw[1] = "HOST PORT";
        }
        nButtons  = nDefaultButtons + nNonDefaultButtons;
        
        positions_X = (int *)malloc(sizeof(int) * nButtons);
        positions_Y = (int *)malloc(sizeof(int) * nButtons);
        imageReferences = (Image *)malloc(sizeof(Image) * (nButtons * 2));//One clicked and an unclicked version
        layerLoadedTextures = (Texture2D *)malloc(sizeof(Image) * (nButtons * 2));
        
        buttonindexing = 0;
        for (int i = 0 ; i < nDefaultButtons; i ++){
            imageReferences[buttonindexing] = UnprocessedImages[2];
            imageReferences[buttonindexing + 1] = UnprocessedImages[3];
            ImageDrawText(&imageReferences[buttonindexing], textToDraw[i], ((imageReferences[buttonindexing].width / 2) - (MeasureText("PLAY", fontSize) / 2)), ((imageReferences[buttonindexing].height / 2) - (fontSize / 2)), fontSize, BLACK);
            ImageDrawText(&imageReferences[buttonindexing + 1], textToDraw[i], ((imageReferences[buttonindexing + 1].width / 2) - (MeasureText("PLAY", fontSize) / 2)), ((imageReferences[buttonindexing].height / 2) - (fontSize / 2)), fontSize, BLACK);
            positions_X[i] = ((screenWidth / 2) - (imageReferences[buttonindexing].width / 2));
            positions_Y[i] = ((screenHeight / (nButtons+1)) * (i + 1));
            layerLoadedTextures[buttonindexing] = LoadTextureFromImage(imageReferences[buttonindexing]);
            layerLoadedTextures[buttonindexing + 1] = LoadTextureFromImage(imageReferences[buttonindexing]);


            buttonindexing += 2;
        }
        for (int i = 0; i < nNonDefaultButtons; i++){

        }

    }
    void drawThisShit(int mX, int mY){
        for (int i = 0; i < nButtons; i += 2){
            if ((positions_X[i] < mX) && ((positions_X[i] + imageReferences[i].width) > mX)){
                DrawTexture(layerLoadedTextures[i + 1], positions_X[i], positions_Y[i], WHITE);
            } else {
                DrawTexture(layerLoadedTextures[i], positions_X[i + 1], positions_Y[i + 1], WHITE);
            }
        }
    }

    void destroy(void){
        for (int i = 0; i < nButtons * 2; i++){
            UnloadTexture(layerLoadedTextures[i]);
            UnloadImage(imageReferences[i]);
        }
        free(positions_X);
        free(positions_Y);
        free(imageReferences);
        free(layerLoadedTextures);
    }

};


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
int play_Game(void);
int load_World(void);
void resizeMenuTextures (Image imgRefList[NUMBER_OF_DIFFERENT_MENU_TEXTURES], int newx, int newy, Texture2D loadedTextures[NUMBER_OF_DIFFERENT_MENU_TEXTURES]);



//Main script. loads default settings, creates configs and loads menu texture before going into the game cycle
//Within the game cycle one may switch between 1. the menu, 2. loading (connecting to server etc..) and 3.The actual game.
int main (int argc, char *args[]){
    
    //Load settings, screen infos and stuff
    nlohmann::json configs = read_json_file("../config.json");
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    bool fullScreen;
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
        if (configs["screen_settings"]["fullScreen"] == true){
            fullScreen = true;
        } else {
            fullScreen = false;
        }
    }
    screenRatios[0] = (float)screenWidth / (float)1920;
    screenRatios[1] = (float)screenHeight / (float)1080;
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
    //Load and size Menu Buttons depending on current screen settings. Images and textures
    std::string menuItemList[NUMBER_OF_DIFFERENT_MENU_TEXTURES] = {"bgmenu", "curs", "default_button", "default_button_clicked", "world", "world_clicked"};
    Image preprocessedMenuImages[NUMBER_OF_DIFFERENT_MENU_TEXTURES];


    
    for (int i = 0 ; i < NUMBER_OF_DIFFERENT_MENU_TEXTURES; i++){
        std::string fileName = ("../sprites/menu/"+menuItemList[i]+".png");
        const char *cFileName = fileName.c_str();
        preprocessedMenuImages[i] = LoadImage(cFileName);
        int s1,s2;
        if (i == 0){
            s1 = screenWidth; s2 = screenHeight;
        } else if (i == 1){
            s1 = 50 * screenRatios[0]; s2 = 50 * screenRatios[1];
        } else if (i == 2 || i == 3){
            s1 = 300 * screenRatios[0]; s2 = 100 * screenRatios[1];
        } else if (i == 4 || i == 5){
            s1 = 300 * screenRatios[0]; s2 = 300 * screenRatios[1];
        }
        ImageResize(&preprocessedMenuImages[i], s1, s2);
    }
    //DO THE SAME FOR CHAR AND INGAME SPRITES
/*
    Image preprocessedWorldSprites[NUMBER_WORLD_TEXTURES];
    Image preprocessedCharSprites[NUMBER_CHAR_TEXTURES];
    Image preprocessedenemySprites[NUMBER_ENEMY_TEXTURES];
*/
    
    HideCursor();
    
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
            //Load Texture and Connect client socket with host
            case 1:
                GAME_STATE = load_World();
            case 2:
                GAME_STATE = play_Game();
            case 3:
                GAME_STATE = play_Game();
        }

        
    }
    CloseWindow();

    for (int i = 0; i < NUMBER_OF_DIFFERENT_MENU_TEXTURES; i++){
        UnloadImage(preprocessedMenuImages[i]);
    }  

    
    return 0;
}


void resizeMenuTextures (Image imgRefList[NUMBER_OF_DIFFERENT_MENU_TEXTURES], int newx, int newy, Texture2D loadedTextures[NUMBER_OF_DIFFERENT_MENU_TEXTURES]){
    for (int i = 0; i < NUMBER_OF_DIFFERENT_MENU_TEXTURES; i++){
        ImageResize(&imgRefList[i], newx, newy);
        UnloadTexture(loadedTextures[i]);
        loadedTextures[i] = LoadTextureFromImage(imgRefList[i]);
    }
}

int run_Menu(Image imageRefList[NUMBER_OF_DIFFERENT_MENU_TEXTURES], nlohmann::json configFile){
    //Info used to create default buttons
    int fontGroesse = 35 * screenRatios[1];
    int nProfiles = configFile["nprofiles"];
    //Now the default buttons are created. png suited for buttons are given, their displayed text, loaded for each of their two states [clicked or not clicked] and at last moved into gpu
    
    //0-5 layers... 0: pick profile. 1:play, options, exit. 2:options content. 3:Host or Join. 4:hosting, create or pick world. 5:joining enter host ip and port

    Layer Layers[6];
    
    for (int k = 0; k < 6; k++){
        Layers[k].initiate(k, nProfiles, 0, imageRefList, fontGroesse);
    }



    //add cursor and background
    Texture2D CurserSprite = LoadTextureFromImage(imageRefList[1]);
    Texture2D BackGroundSprite = LoadTextureFromImage(imageRefList[0]);

    int mouseX = GetMouseX();
    int mouseY = GetMouseY();
    int menu_running = 1;
    int Current_Layer = 1;
    while (menu_running == 1) {
        mouseX = GetMouseX();
        mouseY = GetMouseY();
        BeginDrawing();
            ClearBackground(GRAY);
            DrawTexture(BackGroundSprite, 0, 0, WHITE);
            //Draw default buttons available for the current layer
            Layers[Current_Layer].drawThisShit(mouseX, mouseY);
            DrawFPS(0,0);
            DrawTexture(CurserSprite, mouseX + (CurserSprite.width / 2), mouseY + (CurserSprite.height / 2), WHITE);
        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE) == true){
            if (Current_Layer == 0 || Current_Layer == 1){
                menu_running = -1;
            } else {
                Current_Layer -= 1;
            }
        }
        
    }

    //Unload Texture and Images. Cause why do we need them...
    UnloadTexture(BackGroundSprite);
    UnloadTexture(CurserSprite);
    for (int k = 0; k < 6; k++){
        Layers[k].destroy();
    }
    return menu_running;
}

int load_World(void){
    return -1;
}

int play_Game(void){
    return -1;
}
////GAME
//Main File. Here the basic settings are loaded. And the game loop switches between menu, loading, playing or exiting properly.
#include "declr.hpp"

namespace gV 
{
    int screenWidth;
    int screenHeight;
    int screenNumber;
    float screenRatio; 
    bool fullScreen;

    //Game information
    int GAME_STATE;
    int ERROR_HANDLING;
    std::string activeProfileName;
    std::string activeProfileKey;

    //relevant for the network communicaiton
    std::string own_Ip_Adress;
    std::string host_Ip_Adress;
    int client_Port;
    int host_Port;

    //Information in case of hosting
    bool wantsHost;
    std::string hosting_chosen_world;
    bool newWorld;
}


//Main script. loads default settings, creates configs and loads menu texture before going into the game cycle
//Within the game cycle one may switch between 1. the menu, 2. loading (connecting to server etc..) and 3.The actual game.
int main (int argc, char *args[])
{
    //Load general access settings set by the last user
    nlohmann::json configs = read_json_file("../config.json");
    gV::wantsHost = false;

    //init audio and make window minimizeable
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(0, 0, "Fuck yeah I'm a god");


    //If its the first time starting the game, look for default options and save them in the json
    if (configs["first"] == true)
    {
        gV::screenHeight = GetScreenHeight();
        gV::screenWidth = GetScreenWidth();
        gV::screenNumber = GetCurrentMonitor();
        //standardize into 16:9? just in case
        if (gV::screenHeight >= 1080 && gV::screenWidth >= 1920)
        {
            gV::screenHeight = 1080;
            gV::screenWidth = 1920;
        } 
        else if (gV::screenHeight >= 720 && gV::screenWidth >= 1280)
        {
            gV::screenHeight = 720;
            gV::screenWidth = 1280;
        } 
        else
        {
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
    else 
    {
        gV::screenHeight = configs["screen_settings"]["height"];
        gV::screenWidth = configs["screen_settings"]["width"];
        gV::screenNumber = configs["screen_settings"]["monitorId"];
        gV::fullScreen = configs["screen_settings"]["fullScreen"];
    }
    //THE.. Most important grafik information. especially if we stick to 16:9
    gV::screenRatio = (float)gV::screenWidth / (float)1920;


    //Set Up Window
    SetWindowMonitor(gV::screenNumber);
    SetWindowSize(gV::screenWidth, gV::screenHeight);
    if ((IsWindowFullscreen() == true && gV::fullScreen == false) || (IsWindowFullscreen() == false && gV::fullScreen == true))
    {
        ToggleFullscreen();
    }
    SetTargetFPS(60);HideCursor();

    //LOAD SOUND!!


    //STOP IF ANYTHING FAILED!!!
    if (IsWindowReady() == false)
    {
        std::cout << "Error setting up application window" << std::endl;
        system("pause");
        return 0;
    } 
    else 
    {
        gV::GAME_STATE = 0;
        gV::ERROR_HANDLING = 0;
    }


    //Loading Menu Images
    std::string menuItemList[NR_MENU_TEXTURES] = {"bgmenu", "curs", "default_button", "default_button_clicked", "world", "world_clicked"};
    Image preprocessedMenuImages[NR_MENU_TEXTURES];
    for (int i = 0 ; i < NR_MENU_TEXTURES; i++)
    {
        std::string fileName = ("../sprites/menu/"+menuItemList[i]+".png");
        const char *cFileName = fileName.c_str();
        preprocessedMenuImages[i] = LoadImage(cFileName);
    }

    //DO THE SAME FOR ingamesprites and loading screen textures!
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
    while (gV::GAME_STATE != -1)
    { 
        switch (gV::GAME_STATE)
        {
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
    for (short i = 0; i < NR_MENU_TEXTURES; i++)
    {
        UnloadImage(preprocessedMenuImages[i]);
    }  

    
    return 0;
}
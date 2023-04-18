////GAME
//Main File. 
#include "declr.hpp"
#include "tilemakros.hpp"
//See declr.hpp
namespace gV {
    int windowWidth;
    int windowHeight;
    int screenNumber;
    float screenRatio; 
    int GAME_STATE;
    int ERROR_HANDLING;
    std::string activeProfileName;
    std::string activeProfileKey;
    std::string own_Ip_Adress;
    std::string host_Ip_Adress;
    int client_Port;
    int host_Port;
    bool wantsHost;
    std::string hosting_chosen_world;
    bool newWorld;
}

//Main script. loads default settings, creates configs and loads menu texture before going into the game cycle
//Within the game cycle one may switch between 1. the menu, 2. loading (connecting to server etc..) and 3.The actual game.
int main (int argc, char *args[]){
    //Load general access settings set by the last user
    nlohmann::json configs = read_json_file("../config.json");

    gV::wantsHost = false;

    //If its the first time starting the game, look for default options and save them in the json
    if (configs["first"] == true){
        gV::screenNumber = GetCurrentMonitor();
        gV::windowHeight = GetScreenHeight();
        gV::windowWidth = GetScreenWidth();
        //standardize into 16:9? just in case
        if (gV::windowHeight >= 1080 && gV::windowWidth >= 1920){
            gV::windowHeight = 1080;
            gV::windowWidth = 1920;
        } 
        else if (gV::windowHeight >= 720 && gV::windowWidth >= 1280){
            gV::windowHeight = 720;
            gV::windowWidth = 1280;
        } 
        else{
            gV::windowHeight = 540;
            gV::windowWidth = 960;
        }
        //save in config and then in json
        configs["first"] = false;
        configs["screen_settings"]["width"] = gV::windowWidth;
        configs["screen_settings"]["height"] = gV::windowHeight;
        configs["monitorId"] = gV::screenNumber;
        std::ofstream output_file("../config.json");
        output_file << configs.dump(1) << std::endl;
        output_file.close();
    }
    //Or read out information of the config file
    else {
        gV::screenNumber = configs["screen_settings"]["monitorId"];
        gV::windowHeight = configs["screen_settings"]["height"];
        gV::windowWidth = configs["screen_settings"]["width"];

        //Stop cheating of the resolution system
        if (!(gV::windowHeight == 1080 && gV::windowWidth == 1920) && !(gV::windowHeight == 720 && gV::windowWidth == 1280) && !(gV::windowHeight == 960 && gV::windowWidth == 540)){
            gV::windowHeight = 540;
            gV::windowWidth = 960;
            configs["screen_settings"]["width"] = gV::windowWidth;
            configs["screen_settings"]["height"] = gV::windowHeight;
            std::ofstream output_file("../config.json");
            output_file << configs.dump(1) << std::endl;
            output_file.close();
        }
    }
    //THE.. Most important grafik information. especially if we stick to 16:9
    gV::screenRatio = (float)1920/ (float)gV::windowWidth;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Dig a Grave, Togetha");
    //Set Up Window
    SetWindowMonitor(gV::screenNumber);
    SetWindowMinSize(1280, 720);
    SetWindowSize(gV::windowWidth, gV::windowHeight);
    SetTargetFPS(60);HideCursor();

    //LOAD SOUND!!


    //STOP IF ANYTHING FAILED!!!
    if (IsWindowReady() == false){
        std::cout << "Error setting up application window" << std::endl;
        system("pause");
        return 0;
    } 
    else {
        gV::GAME_STATE = 0;//Go to menu
        gV::ERROR_HANDLING = 0;//without errors
    }


    //Loading Menu Images
    std::string menuItemList[NR_MENU_TEXTURES] = {"bgmenu", "curs", "default_button", "default_button_clicked", "world", "world_clicked"};
    Image preprocessedMenuImages[NR_MENU_TEXTURES];
    for (int i = 0 ; i < NR_MENU_TEXTURES; i++){
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
    for (short i = 0; i < NR_MENU_TEXTURES; i++){
        UnloadImage(preprocessedMenuImages[i]);
    }  

    
    return 0;
}
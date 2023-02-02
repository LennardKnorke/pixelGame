
////GAME
//Standart libs
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <thread>
#include <random>
//External libs
#include "nlohmann/json.hpp"
#include <raylib.h>
//#include "SFML/Network.hpp"

//prep functions from other files
extern  void run_server_thread(int port);

//Ease scalability of texture/ button loading
#define NUMBER_OF_DIFFERENT_MENU_TEXTURES 6 

#define NUMBER_WORLD_TEXTURES 0
#define NUMBER_CHAR_TEXTURES 0
#define NUMBER_ENEMY_TEXTURES 0

//SOME GLOBAL VARIABLES
//relevant for the window:
int screenWidth = 0;
int screenHeight = 0;
int screenNumber = 0;
float screenRatio; //can all be reduced to one number if we limit to 16:9
bool fullScreen;
int GAME_STATE = 0;
//relevant for the network communicaiton
std::string own_Ip_Adress;
std::string host_Ip_Adress;
int client_Port;
int host_Port;
std::string activeProfileName;
std::string activeProfileKey;

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
std::string generateKey(void);
bool checkFile (void);
int run_Menu(Image imageRefList[NUMBER_OF_DIFFERENT_MENU_TEXTURES], nlohmann::json configFile);
int play_Game();
int load_World(void);

//Main script. loads default settings, creates configs and loads menu texture before going into the game cycle
//Within the game cycle one may switch between 1. the menu, 2. loading (connecting to server etc..) and 3.The actual game.
int main (int argc, char *args[]){
    
    //Load settings, screen infos and stuff
    nlohmann::json configs = read_json_file("../config.json");
    
    //init audio and make window minimizeable
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitAudioDevice();
    
    InitWindow(screenWidth, screenHeight, "Fuck yeah I'm a god");
    //If its the first time starting the game, look for default options and save them in the json
    if (configs["first"] == true){
        screenHeight = GetScreenHeight();
        screenWidth = GetScreenWidth();
        screenNumber = GetCurrentMonitor();
        //standardize into 16:9? just in case
        if (screenHeight >= 1080 && screenWidth >= 1920){
            screenHeight = 1080;
            screenWidth = 1920;
        } else if (screenHeight >= 720 && screenWidth >= 1280){
            screenHeight = 1080;
            screenWidth = 1920;
        } else {
            screenHeight = 540;
            screenWidth = 960;
        }
        //save in config and then in json
        configs["first"] = false;
        configs["screen_settings"]["width"] = screenWidth;
        configs["screen_settings"]["height"] = screenHeight;
        configs["screen_settings"]["fullScreen"] = true;
        fullScreen = true;
        std::ofstream output_file("../config.json");
        output_file << configs.dump(1) << std::endl;
        output_file.close();
    }
    //Or read out information of the config file
    else {
        screenHeight = configs["screen_settings"]["height"];
        screenWidth = configs["screen_settings"]["width"];
        screenNumber = configs["screen_settings"]["monitorId"];
        fullScreen = configs["screen_settings"]["fullScreen"];
    }
    screenRatio = (float)screenWidth / (float)1920;


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

   //Main main main loop. switch between menu, loading/connecting and game cycle
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
        ImageResize(&imageRefList[m], textureSizes[m][0] * screenRatio, textureSizes[m][1]* screenRatio);
        scaledMenuTextures[m] = LoadTextureFromImage(imageRefList[m]);
    }

    //Button information. How many default button, how many variable buttons. Then their Text
    int nDefaultButtonPerLayer[6] = {1,3,3,2,1,2};
    int nNonDefaultButtonPerLayer[6] = {configFile["nprofiles"],0,0,0,0,0};
    int nTotalButtons[6] = {(nDefaultButtonPerLayer[0] + nNonDefaultButtonPerLayer[0]), (nDefaultButtonPerLayer[1] + nNonDefaultButtonPerLayer[1]), (nDefaultButtonPerLayer[2] + nNonDefaultButtonPerLayer[2]), (nDefaultButtonPerLayer[3] + nNonDefaultButtonPerLayer[3]), (nDefaultButtonPerLayer[4] + nNonDefaultButtonPerLayer[4]), (nDefaultButtonPerLayer[5] + nNonDefaultButtonPerLayer[5])};
    std::string displayText[6][3] = {{"New Profile", "", ""}, {"Play", "Options", "Exit"}, {"High Res", "Mid Res", "Low Res"},{"Host", "Join", ""}, {"New World", "" , ""}, {"Enter Host Ip", "Enter Host Port", ""}};
    
    
    //Idx will be used display a maximum number of 4 profiles/ worlds. adapt idx to a maximum of 4 if exactly 4 are available (no new profile/ world available)
    /*
    if (nNonDefaultButtonPerLayer[0] >= 4){
        nTotalButtons[0] = 4;
    } else if (nNonDefaultButtonPerLayer[4] >= 4){
        nTotalButtons[4] = 4;
    }*/


    //Info for non-default buttons (profile and world buttons)
    std::string availableProfiles[(int)configFile["nprofiles"]];
    std::string availableProfileKeys[(int)configFile["nprofiles"]];
    std::string *availableWorlds;
    bool Max_Profiles = false;
    bool Max_Worlds = false;
    if (nNonDefaultButtonPerLayer[0] >= 4){
        Max_Profiles = true;
    }

    //Other on screen info
    std::string userInput = "";
    std::string instructions = "Use Arrow Buttons, 'Entf' and 'Enter' to navigate";

    
    //Load in available profiles
    for (int i = 0; i < configFile["nprofiles"]; i++){
        std::cout << "Found Profile: " << configFile["profiles"][i]["name"] << std::endl;
        availableProfiles[i] = configFile["profiles"][i]["name"];
        availableProfileKeys[i] = configFile["profiles"][i]["key"];
    }

    //Navigating through the menu
    int menu_running = 1;//int because we can return it to indicate next step in the Main main main loop
    int Current_Layer = 0;
    int pickedIdx = 0;

    //Used for managing char input 
    bool userInputBool = false;
    bool userDeletingBool = false;
    int MAX_LENGTH = 20;
    int nUserInput = 0;

    //MENU LOOP
    while (menu_running == 1) {
        //FIRST DRAW
        BeginDrawing();
            ClearBackground(GRAY);
            DrawTexture(scaledMenuTextures[0], 0, 0, WHITE);

            //Draw default buttons available for the current layer
            for (int i = 0 ; i < nDefaultButtonPerLayer[Current_Layer]; i++){
                //Draw everthing for basically every layer. one more if for layer 0 and 4 in which a maximum of profiles and worlds are drawn
                if ((Current_Layer == 0 && Max_Profiles == false) || (Current_Layer != 0 && Current_Layer != 4) || (Current_Layer == 4 && Max_Worlds == false)){
                    //Draw button background
                    if (pickedIdx == i){
                        DrawTexture(scaledMenuTextures[3], (screenWidth / 2) - (imageRefList[2].width / 2), ((screenHeight / (nTotalButtons[Current_Layer] + 1)) * (1 + i)) - (imageRefList[2].height / 2), WHITE);
                    } else {
                        DrawTexture(scaledMenuTextures[2], (screenWidth / 2) - (imageRefList[2].width / 2), ((screenHeight / (nTotalButtons[Current_Layer] + 1)) * (1 + i)) - (imageRefList[2].height / 2), WHITE);
                    }
                    

                    //Draw button Text
                    if (Current_Layer == 0 || Current_Layer == 4 || Current_Layer == 5){
                        if (userInputBool == false){
                            DrawText(displayText[Current_Layer][i].c_str(), (screenWidth / 2) - (MeasureText(displayText[Current_Layer][i].c_str(), 30 * screenRatio) / 2), ((screenHeight / (nTotalButtons[Current_Layer] + 1)) * (1 + i)) - ((30 * screenRatio)/2), 30*screenRatio, BLACK);
                        } else if ((Current_Layer == 0) && userInputBool == true){
                            DrawText(userInput.c_str(), (screenWidth / 2) - (MeasureText(userInput.c_str(), 30 * screenRatio) / 2), ((screenHeight / (nTotalButtons[Current_Layer] + 1)) * (1 + i)) - ((30 * screenRatio)/2), 30*screenRatio, BLACK);
                        }
                    } else {
                        DrawText(displayText[Current_Layer][i].c_str(), (screenWidth / 2) - (MeasureText(displayText[Current_Layer][i].c_str(), 30 * screenRatio) / 2), ((screenHeight / (nTotalButtons[Current_Layer] + 1)) * (1 + i)) - ((30 * screenRatio)/2), 30*screenRatio, BLACK);
                    }
                }
            }

            //Draw non-default buttons available for the current layer
            for (int i = 0 ; i < nNonDefaultButtonPerLayer[Current_Layer]; i++){
                if (pickedIdx == i + nDefaultButtonPerLayer[Current_Layer]){
                    DrawTexture(scaledMenuTextures[3], (screenWidth / 2) - (imageRefList[2].width / 2), ((screenHeight / (nTotalButtons[Current_Layer] + 1)) * (1 + nDefaultButtonPerLayer[Current_Layer] + i)) - (imageRefList[2].height / 2), WHITE);
                    if (userDeletingBool == true){
                        Rectangle re;
                        re.x = (screenWidth / 2) - (imageRefList[2].width / 2);
                        re.y = ((screenHeight / (nTotalButtons[Current_Layer] + 1)) * (1 + nDefaultButtonPerLayer[Current_Layer] + i)) - (imageRefList[2].height / 2);
                        re.width = imageRefList[2].width;
                        re.height = imageRefList[2].height;
                        const char* warningMsg = "Confirm profile delete with delete or enter. Cancel using ESC.";
                        DrawRectangleLinesEx(re, 10 * screenRatio, RED);
                        DrawText(warningMsg, (screenWidth / 2) - (MeasureText(warningMsg, 18 * screenRatio) / 2), ((screenHeight / (nTotalButtons[Current_Layer] + 1)) * (1 + nDefaultButtonPerLayer[Current_Layer] + i)) + (imageRefList[2].height / 2), 18 * screenRatio, BLACK); 
                    }
                } else {
                    DrawTexture(scaledMenuTextures[2], (screenWidth / 2) - (imageRefList[2].width / 2), ((screenHeight / (nTotalButtons[Current_Layer] + 1)) * (1 + nDefaultButtonPerLayer[Current_Layer] + i)) - (imageRefList[2].height / 2), WHITE);
                }
                if (Current_Layer == 0){
                    DrawText(availableProfiles[i].c_str(), (screenWidth / 2) - (MeasureText(availableProfiles[i].c_str(), 30 * screenRatio) / 2), ((screenHeight / (nTotalButtons[Current_Layer] + 1)) * (1 + nDefaultButtonPerLayer[Current_Layer] + i)) - ((30 * screenRatio)/2), 30*screenRatio, BLACK);
                }
                
            }

            //Draw menu instructions. like use arrows and enter and hist.
            DrawText(instructions.c_str(), (screenWidth / 2) - (MeasureText(instructions.c_str(), 18 * screenRatio) / 2), screenHeight - ((18 * screenRatio)), 18 * screenRatio, BLACK);

            //Draw helpful tools. fps and two lines in the middle of the screen
            DrawFPS(0,0);
            DrawLine(0, screenHeight/2, screenWidth, screenHeight/2, ORANGE);
            DrawLine(screenWidth/2, 0, screenWidth/2, screenHeight, ORANGE);

        EndDrawing();

        //KEY INPUT
        //Escape button 
        if (IsKeyPressed(KEY_ESCAPE) == true){
            if (Current_Layer == 0 && userInputBool == false && userDeletingBool == false){
                menu_running = -1;
            } else if ((Current_Layer == 0|| Current_Layer == 3 || Current_Layer == 4  || Current_Layer == 5) && (userInputBool == true || userDeletingBool == true)){
                if (userDeletingBool == true){
                    userDeletingBool = false;
                } else if (userInputBool == true){
                    userInputBool = false;
                    userInput = "";
                    nUserInput = 0;
                }
                
            } else if (Current_Layer == 5){
                Current_Layer = 3;
            } else if (Current_Layer == 3){
                Current_Layer = 1;
            }else {
                Current_Layer -= 1;
            }
        //pick upper button. ONLY if allowed
        } else if (IsKeyPressed(KEY_UP) && userInputBool == false && userDeletingBool == false){
            pickedIdx -= 1;
            if (pickedIdx < 0){
                pickedIdx = nTotalButtons[Current_Layer] - 1;
            }
        //pick lower button. only if now user input is going on
        } else if (IsKeyPressed(KEY_DOWN) && userInputBool == false && userDeletingBool == false){
            pickedIdx += 1;
            if (pickedIdx >= nTotalButtons[Current_Layer]){
                pickedIdx = 0;
            }
        //Enter button
        } else if (IsKeyPressed(KEY_ENTER)){
            //from the first layer
            if (Current_Layer == 0){
                //if available. create a new profile and go to layer 1
                if (pickedIdx == 0 && Max_Profiles == false){
                    //If no input has been given yet. allow input
                    if (userInputBool == false){
                        userInputBool = true;
                    //Or create the profile given the name
                    } else {
                        userInputBool = false;
                        //create profile key. Make sure it's not a duplicate!
                        bool tmpCheck = false;
                        do {
                            tmpCheck = false;
                            activeProfileKey = generateKey();
                            for (int i = 0; i < configFile["nprofiles"]; i++){
                                if (strcmp(activeProfileKey.c_str(), availableProfileKeys[i].c_str()) == 0){
                                    tmpCheck = true;
                                }
                            }
                        } while (tmpCheck == true);
                        //Change number of profiles! and save in json config
                        configFile["nprofiles"] = configFile["nprofiles"].get<int>() + 1;
                        nlohmann::json newElement = {{"name", userInput.c_str()}, {"key", activeProfileKey.c_str()}};
                        configFile["profiles"].push_back(newElement);
                        std::ofstream output_file ("../config.json");
                        output_file << configFile.dump(1) << std::endl;

                        //Next layer
                        Current_Layer = 1;
                        pickedIdx = 0;
                    }
                    
                } else/* alligns active profile and associated key with the currently selected profile*/ {
                    if (nNonDefaultButtonPerLayer[Current_Layer] <= 4){
                        activeProfileName = availableProfiles[pickedIdx - 1];
                        activeProfileKey = availableProfileKeys[pickedIdx - 1];
                    } else {
                        activeProfileName = availableProfiles[pickedIdx];
                        activeProfileKey = availableProfileKeys[pickedIdx];
                    }
                    Current_Layer = 1;
                    pickedIdx = 0;
                }
            //Boring. Main Layer from which you can go play, see options or exit;
            } else if (Current_Layer == 1){
                if (pickedIdx == 0){
                    Current_Layer = 3;
                } else if (pickedIdx == 1){
                    Current_Layer = 2;
                } else if (pickedIdx == 2){
                    menu_running = -1;
                }
            //OPTIONS. Implement resize function and save new sizes in config
            } else if (Current_Layer == 2){
                if (pickedIdx == 0){

                } else if (pickedIdx == 1){

                } else if (pickedIdx == 2){
                    
                }
            //Choose to host or Join. Also easy and boring
            } else if (Current_Layer == 3){
                if (pickedIdx == 0){
                    Current_Layer = 4;
                } else if (pickedIdx == 1){
                    Current_Layer = 5;
                }
                pickedIdx = 0;
            }
        } else if (IsKeyPressed(KEY_DELETE)){
            userDeletingBool = true;
        //Any other button
        } else {
            //Only in specific layers if they are not filled out the proper field has been selected. 
            if (userInputBool == true && ((Current_Layer == 0 && pickedIdx == 0 && Max_Profiles == false) || (Current_Layer == 4 && pickedIdx == 0 && Max_Worlds == false) || (Current_Layer == 5 && (pickedIdx == 0 || pickedIdx == 1)))){
                int key = GetCharPressed();
                char keyInput[2]; keyInput[0] = key; keyInput[1] = '\0';
                if (key >= 32 && key <= 125 && nUserInput <= MAX_LENGTH){
                    userInput.append(keyInput);
                    nUserInput += 1;
                }
                if (IsKeyPressed(KEY_BACKSPACE)){
                    userInput.pop_back();
                    nUserInput -= 1;
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

std::string generateKey(void){
    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, alphabet.length() - 1);
    std::string result;
    for (int i = 0 ; i < 15; i++){
        result += alphabet[distribution(generator)];
    }
    return result;
}

bool checkFile (void){
    return true;
}


//connect client and load, scale images
int load_World(void){
    return -1;
}

//GAME LOOOOOP
int play_Game(void){
    return -1;
}
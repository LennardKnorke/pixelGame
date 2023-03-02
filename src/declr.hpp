//Maybe this should contain only declerations important for the main loop, menu and loading application to avoid confusion and create a new decleration file

#ifndef DECLR_H
#define DECLR_H
//Standart libs
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <thread>
#include <random>
#include <vector>
#include <time.h>
#include <filesystem>
//External libs
#include "nlohmann/json.hpp"
#include <raylib.h>

//MACROS
//Texture overview
#define NR_MENU_TEXTURES 6     //Currently only 4 in Use
#define NR_TILE_TEXTURES 0
#define NR_CHAR_TEXTURES 0
#define NR_NPC_TEXTURES 0
#define NR_BACKGROUND_TEXTURES 0 //First one is for loading screen.

//World features
#define WORLD_WIDTH_C 500
#define WORLD_DEPTH_C 1000

//easy makros to set tile state? will be a long list tho
#define EMPTY_TILE 0    //Tile is empty
#define ROCK_TILE 1
#define WOOD_TILE 2
#define IRON_TILE 3

//SOME GLOBAL VARIABLES (gV)
namespace gV 
{
    //screen/window variables
    extern int screenWidth;
    extern int screenHeight;
    extern int screenNumber;
    extern float screenRatio; //can all be reduced to one number if we limit to 16:9
    extern bool fullScreen;

    //Game information
    extern int GAME_STATE;
    extern int ERROR_HANDLING;
    extern std::string activeProfileName;
    extern std::string activeProfileKey;

    //relevant for the network communicaiton
    extern std::string client_Ip_Adress;
    extern std::string host_Ip_Adress;
    extern int client_Port;
    extern int host_Port;

    //Information in case of hosting
    extern bool wantsHost;
    extern std::string hosting_chosen_world;
    extern bool newWorld;
    
}
//in menu.cpp
typedef struct ProfileData ProfileData;
    //Stores name and key and worldData for a profile
typedef struct WorldData WorldData;
    //Stores the name. But can be extended if needed

//in overallFunction.cpp
typedef struct worldTileInformationTemplate SaveFile;
    //Stores an owner name and their their key
    //whether the world has been generated
    // saves about all the players that have joined
    //Aand the saveable tile states
typedef struct PlayerInformation PlayerInformation;
    //for each saved name and associated key, keep track of all the saved stats of the character!

//Menu functions in overallFunction.cpp
nlohmann::json read_json_file (const std::string &path);    
    //Use to read and return a json files content

std::string generateString(void);
    //generate a key for player

int findStringIndex(std::string &target, std::vector<std::string> &stringArray);    
    //find index of string in a vector of strings

void addProfileToConfig(nlohmann::json &configFileToSave, std::string &newProfileKey, std::string &newProfileName);
    //updates the config file with a new profile

void createWorldFile(std::string ownerName, std::string ownerKey, std::string worldName); 
    //create a NEW uninitiliazed savefile

SaveFile openOwnedSave(std::string ownerKey, std::string fileName);
    //open a file and if the owner key cooincide return the saveFile

//not finished!
void deleteProfile(nlohmann::json &configFile, std::string &ProfileKey, std::string &ProfileName);
    //updates config to remove a profile. Given an ownerkey, delete all worlds associated with this key

//not finished!
void deleteSpecificWorld(void);
    //Delete a specific world of a profile


///////////////////////////////////////////////
/*
GAME_STATE FUNCTIONS
Running in Main.cpp
*/
//////////////////////////////////////////////
int run_Menu(Image imageRefList[NR_MENU_TEXTURES], nlohmann::json configFile);
    //Runs the menu and return true if a user requested to play
    
//Unfinished!
void run_server_thread(int port);       
    //Start in seperate thread and run the server there

//Unfinished!
int play_Game(void);                    
    //Game loop. draws, reads input, sends input, receives and updates drawings

//Unfinished!
int load_World(void);                   
    //Connect to host



#endif
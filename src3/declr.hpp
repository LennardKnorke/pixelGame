//Maybe this should contain only declerations important for the main loop, menu and loading application to avoid confusion and create a new decleration file
#pragma once

#ifndef DECLR_HPP
#define DECLR_HPP
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
#include "tilemakros.hpp"
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


#define MAX_KEY_LENGTH 15
#define MAX_INPUT_LENGTH 16
//easy makros to set tile state? will be a long list tho

//SOME GLOBAL VARIABLES (gV)
namespace gV {
    extern int windowWidth;
    extern int windowHeight;
    extern int screenNumber;
    extern float screenRatio; //can all be reduced to one number if we limit to 16:9

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

typedef struct ProfileData{
    std::string Name;
    std::string Key;
    std::vector<std::string> Worlds;
} ProfileData;

//in filemanagement.cpp
class Player {
    private:
    char *name;               //PlayerName
    char key[MAX_KEY_LENGTH];                //PlayerKey
    int level;                      //what is the level?
    bool alive;

    public:
    void createPlayer(std::string name, std::string key);
    void readPlayer(std::ifstream &inputFile);
    void writePlayer(std::ofstream &outputFile);
    void freePlayer(void);

};

//in filemanagement.cpp
class SaveFile{
    private:
    char *worldName;
    char owner_Key[MAX_KEY_LENGTH];
    bool initialized;
    int seed;
    char worldTileInformation[WORLD_WIDTH_C][WORLD_DEPTH_C] = {EMPTY_TILE};
    std::vector<Player> joinedPlayers;

    public:
    void createNewSave(std::string name, std::string ownerName, std::string ownerKey);
    void initialize(void);

    void writeIntoFile(std::ofstream &outputFile);

    void readFromFile(std::ifstream &inputFile);
    void freeSaveFile(void);
};




//in filemanagement.cpp
nlohmann::json read_json_file (const std::string &path);    
    //Use to read and return a json files content

//in orga.cpp
std::string generateString(void);
    //generate a key for player

//in orga.cpp
int findStringIndex(std::string &target, std::vector<std::string> &stringArray);    
    //find index of string in a vector of strings

//in orga.cpp
void addProfileToConfig(nlohmann::json &configFileToSave, std::string &newProfileKey, std::string &newProfileName);
    //updates the config file with a new profile

//in filemanagement.cpp
void createWorldFile(std::string ownerName, std::string ownerKey, std::string worldName); 
    //create a NEW uninitiliazed savefile

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
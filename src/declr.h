#ifndef DECLR_H
#define DECLR_H

//Ease scalability of texture/ button loading
#define NUMBER_OF_DIFFERENT_MENU_TEXTURES 6 

#define NUMBER_WORLD_TEXTURES 0
#define NUMBER_CHAR_TEXTURES 0
#define NUMBER_ENEMY_TEXTURES 0

//SOME GLOBAL VARIABLES (gV)
namespace gV {
    //screen/window variables
    extern int screenWidth;
    extern int screenHeight;
    extern int screenNumber;
    extern float screenRatio; //can all be reduced to one number if we limit to 16:9
    extern bool fullScreen;

    //Game information
    extern int GAME_STATE;
    extern std::string activeProfileName;
    extern std::string activeProfileKey;

    //relevant for the network communicaiton
    extern std::string own_Ip_Adress;
    extern std::string host_Ip_Adress;
    extern int client_Port;
    extern int host_Port;

    //Information in case of hosting
    extern bool wantsHost;
    extern std::string hosting_chosen_world;
    extern bool newWorld;
    
}



//Declare Functions
//
//Overall Functions
nlohmann::json read_json_file (const std::string &path);                            //Use to read and return a json files content
std::string generateString(void);                                                   //generate a key for player
int findStringIndex(std::string &target, std::vector<std::string> &stringArray);
void addProfileToConfig(nlohmann::json &configFileToSave, std::string &newProfileKey, std::string &newProfileName);

//not finished!
bool checkFile(void);
//not finished!
void createWorldFile(void);
//not finished!
void removeProfileFromConfig(void);
//not finished!
void deleteProfileWorlds(void);


//Game Loop Functions
//Use to run either the menu, load world/ connect with host, run client, run host_thread
void run_server_thread(int port);
int play_Game(void);
int load_World(void);
int run_Menu(Image imageRefList[NUMBER_OF_DIFFERENT_MENU_TEXTURES], nlohmann::json configFile);


#endif
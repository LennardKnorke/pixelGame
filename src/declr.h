//Maybe this should contain only declerations important for the main loop, menu and loading application to avoid confusion and create a new decleration file

#ifndef DECLR_H
#define DECLR_H

//Ease scalability of loading textures
#define NUMBER_OF_DIFFERENT_MENU_TEXTURES 6     //Currently only 4 in Use
#define NUMBER_WORLD_TEXTURES 0
#define NUMBER_CHAR_TEXTURES 0
#define NUMBER_ENEMY_TEXTURES 0

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


typedef struct worldTileInformationTemplate SaveFile;
typedef struct PlayerInformation PlayerInformation;

//Overall Functions

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
void removeProfileFromConfig(void);
    //updates config to remove a profile. Given an ownerkey, delete all worlds associated with this key

//not finished!
void deleteSpecificWorld(void);
    //Delete a specific world of a profile


//Game Loop Functions
//Use to run either the menu, load world/ connect with host, run client, run host_thread

int run_Menu(Image imageRefList[NUMBER_OF_DIFFERENT_MENU_TEXTURES], nlohmann::json configFile);
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
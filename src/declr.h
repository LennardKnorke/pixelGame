#ifndef DECLR_H
#define DECLR_H

//Ease scalability of texture/ button loading
#define NUMBER_OF_DIFFERENT_MENU_TEXTURES 6 

#define NUMBER_WORLD_TEXTURES 0
#define NUMBER_CHAR_TEXTURES 0
#define NUMBER_ENEMY_TEXTURES 0

//SOME GLOBAL VARIABLES (gV)
//relevant for the window:
namespace gV {
    extern int screenWidth;
    extern int screenHeight;
    extern int screenNumber;
    extern float screenRatio; //can all be reduced to one number if we limit to 16:9
    extern bool fullScreen;
    extern int GAME_STATE;

    //relevant for the network communicaiton
    extern std::string own_Ip_Adress;
    extern std::string host_Ip_Adress;
    extern int client_Port;
    extern int host_Port;
    extern std::string activeProfileName;
    extern std::string activeProfileKey;
}



//Declare Functions
//
//Overall Functions
nlohmann::json read_json_file (const std::string &path);
void resizeTextureAndImages (Image imageList[], Texture2D textureList[],  int sizes[][2], int numberImages);
bool checkFile(void);
std::string generateKey(void);


//Game Loop Functions
void run_server_thread(int port);
int play_Game(void);
int load_World(void);
int run_Menu(Image imageRefList[NUMBER_OF_DIFFERENT_MENU_TEXTURES], nlohmann::json configFile);


#endif
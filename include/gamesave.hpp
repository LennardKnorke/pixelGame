#pragma once
#ifndef GAMESAVE_HPP
#define GAMESAVE_HPP
#include "stdlibs.hpp"


//struct to summarize relevant player information
typedef struct Player{
    std::string name;
    std::string key;
}Player;

class NPC;

//game save class. manipulate, load, create or safe a world/game instance
class gameSave {
    private:
    //Variable of uninitialized saves
        //three names relevant for the gameSaveSummary
    std::string saveName;
    std::string fileName;
    std::string pathName;
    bool initialized;
    //Others
    Tile tileMap[10000][10000];
    std::vector<Player*> activePlayers;
    std::vector<Player> clientsData;
    std::vector<NPC*> npcs;
    std::vector<int> seed;
    //Variables which do no get saved in the savefile
    bool loadedComplete = false;

    bool loadSave(std::string savePath);
    bool loadCompleteSafe(std::ifstream *file);
    bool initUninitialized(void);

    bool saveSave(void);
    


    //

    public:
    
    gameSave(std::string savePath);
    Player addPlayer(char userKey[MAX_LENGTH_KEY]);
    //Return functions to get private variables
    //saveName
    std::string getSaveName(void);
    std::string getSavePath(void);
    std::string getFileName(void);
    
};



#endif //GAMESAVE_HPP
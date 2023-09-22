#pragma once
#ifndef GAMESAVE_HPP
#define GAMESAVE_HPP
#include "stdlibs.hpp"

#define MAX_LENGTH_KEY 14

//checks if a relative file exists
bool fileExists(const std::string &filename);



//struct to summarize relevant player information
typedef struct clientPlayer{
    std::string name;
    std::string key;
}Player;



//struct:saves the filename, savename, pathname
typedef struct gameSaveSummary{
    std::string saveName;
    std::string fileName;
    std::string pathName;
}gameSaveSummary;



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
    std::vector<clientPlayer> clientsData;
    std::vector<int> seed;
    //Variables which do no get saved in the savefile
    bool loadedComplete;


    public:
    
    gameSave(std::string newSafeName);
    gameSave(std::string SafeName, const std::string &path);
    gameSave(void);
    void loadByHost(void);
    bool saveSave(void);
    void loadCompleteSafe(void);

    //Initialize save
    void initUninitialized(void);


    //
    clientPlayer addPlayer(char userKey[MAX_LENGTH_KEY]);

    //Return functions to get private variables
    //saveName
    std::string getSaveName(void);
    std::string getSavePath(void);
    std::string getFileName(void);
    
};



#endif //GAMESAVE_HPP
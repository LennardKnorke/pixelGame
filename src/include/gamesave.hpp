#pragma once
#ifndef GAMESAVE_HPP
#define GAMESAVE_HPP
#include "stdlibs.hpp"

bool fileExists(const std::string &filename);

typedef struct clientPlayer{
    std::string name;
    std::string key;
}Player;

typedef struct gameSaveSummary{
    std::string saveName;
    std::string fileName;
    std::string pathName;
}gameSaveSummary;


class gameSave {
    private:
    //Variable of uninitialized saves
        //three names relevant for the gameSaveSummary
        std::string saveName;
        std::string fileName;
        std::string pathName;
    bool initialized;
    //Others
    std::string owner;
    std::vector<clientPlayer> clientsData;

    //Variables which do no get saved in the savefile
    bool loadedComplete;

    public:
    
    gameSave(std::string newSafeName);
    gameSave(std::string SafeName, const std::string &path);
    gameSave(void);
    void loadByHost(void);
    bool saveSave(void);
    void loadCompleteSafe(void);
    std::string getSaveName(void);
    
};


#endif //GAMESAVE_HPP
#include "gamesave.hpp"

//Create an instance: existing save file
gameSave::gameSave(std::string savePath){

    loadedComplete = loadSave(savePath);

    
    
}

bool gameSave::loadSave(std::string savePath){
    
    std::ifstream inputFile(savePath, std::ios::binary);

    size_t size;

    inputFile.read(reinterpret_cast<char *>(&size), sizeof(size));
    saveName.resize(size);
    inputFile.read(reinterpret_cast<char *>(&saveName[0]), size);

    inputFile.read(reinterpret_cast<char *>(&size), sizeof(size));
    fileName.resize(size);
    inputFile.read(reinterpret_cast<char *>(&fileName[0]), size);

    inputFile.read(reinterpret_cast<char *>(&size), sizeof(size));
    pathName.resize(size);
    inputFile.read(reinterpret_cast<char *>(&pathName[0]), size);

    inputFile.read(reinterpret_cast<char *>(&initialized), sizeof(bool));

    if (initialized){
        return loadCompleteSafe(&inputFile);
    }
    else {
        inputFile.close();
        return initUninitialized();
    }
}

//loads all the data of an already initialzed gamesave
bool gameSave::loadCompleteSafe(std::ifstream *file){
    //LOAD TILES

    //LOAD PLAYERS

    //Load NPCS
    file->close();
    return true;
}


bool gameSave::initUninitialized(){
    unsigned short nPlayers = 0;
    return false;
}

bool gameSave::saveSave(void){
    return true;
}


//Return of information from private variables
std::string gameSave::getSaveName(void){return saveName;}
std::string gameSave::getSavePath(void){return pathName;}
std::string gameSave::getFileName(void){return fileName;}
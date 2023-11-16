#include "gamesave.hpp"

//Create an instance: existing save file
gameSave::gameSave(std::string savePath){

    if (!fileExists(savePath)){
        std::cout << "save cant load because path invalid!\n";
        loadedComplete = false;
        return;
    }
    if (!loadSave(savePath)){
        std::cout << "Failed to load or initialize save\n";
        loadedComplete = false;
        return;
    }
    
}

bool gameSave::loadSave(std::string savePath){

    std::ifstream inputFile(savePath, std::ios::binary);
    readStrOfFile(inputFile, saveName);
    readStrOfFile(inputFile, fileName);
    readStrOfFile(inputFile, pathName);
    inputFile.read(reinterpret_cast<char *>(&initialized), sizeof(bool));

    if (initialized){
        loadCompleteSafe(inputFile);
        inputFile.close();
    }
    else {
        inputFile.close();
        initUninitialized();
        //saveSave();
    }


    if (loadedComplete && initialized){
        return true;
    }
    else {
        return false;
    }
}

//loads all the data of an already initialzed gamesave
void gameSave::loadCompleteSafe(std::ifstream &file){
    //LOAD TILES

    //LOAD PLAYERS

    //Load NPCS
    loadedComplete = true;

}


void gameSave::initUninitialized(){
    unsigned short nPlayers = 0;
    //Set seed


    // Magic


    initialized = true;
    loadedComplete = true;
    return;
}

bool gameSave::saveSave(void){
    return true;
}


//Return of information from private variables
bool gameSave::getSaveLoadedState(void){return loadedComplete;}
std::string gameSave::getSaveName(void){return saveName;}
std::string gameSave::getSavePath(void){return pathName;}
std::string gameSave::getFileName(void){return fileName;}
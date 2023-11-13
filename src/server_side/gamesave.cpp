#include "gamesave.hpp"

//Create an instance: existing save file
gameSave::gameSave(std::string savePath){
    std::string s = savePath;
}


//loads all the data of an already initialzed gamesave
void gameSave::loadCompleteSafe(void){
    return;
}



bool gameSave::saveSave(void){
    return true;
}


//Return of information from private variables
std::string gameSave::getSaveName(void){return saveName;}
std::string gameSave::getSavePath(void){return pathName;}
std::string gameSave::getFileName(void){return fileName;}
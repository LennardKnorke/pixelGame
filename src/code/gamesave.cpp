#include "gamesave.hpp"

//Create an instance: new 'raw' safe
gameSave::gameSave(std::string newSafeName){
    //Find a new name available for the file.
    std::string newFileName = "WORLD_";
    std::string newpathName = "sav/"+newFileName;
    int i = 0;
    while (fileExists(newpathName+std::to_string(i)+".SAV")){
        i++;
    }
    newFileName = newFileName+std::to_string(i);
    newpathName = newpathName+std::to_string(i);
    std::cout <<newpathName <<std::endl;

    //Set up variables for uninitialized World save files
    saveName = newSafeName;
    fileName = newFileName;
    pathName = newpathName;
    initialized = false;
    //Save the uninitiliazed file (only needs to be done once)
    //As soon as the file is opened and modified for the first time will initialize the save
    std::ofstream file(newpathName+".SAV", std::ios::binary);

    size_t size = saveName.size();
    //Save the saveName (name given by player)
    file.write(reinterpret_cast<char *>(&size), sizeof(size));
    file.write(reinterpret_cast<char*>(saveName.data()), size);
    //Save the filename (name of the file)
    size = fileName.size();
    file.write(reinterpret_cast<char*>(&size), sizeof(size));
    file.write(reinterpret_cast<char*>(fileName.data()), size);
    //Save the path (just in case?)
    size = pathName.size();
    file.write(reinterpret_cast<char*>(&size), sizeof(size));
    file.write(reinterpret_cast<char*>(pathName.data()), size);
    //Save whether save has been initialized
    file.write(reinterpret_cast<char*>(&initialized), sizeof(bool));

    file.close(); 
}

//Create an instance: existing save file
gameSave::gameSave(std::string SafeName, const std::string &path){
    std::ifstream file(path+".SAV", std::ios::binary);
    
    size_t size;
    file.read(reinterpret_cast<char *>(&size), sizeof(size));
    saveName.resize(size);
    file.read(reinterpret_cast<char *>(&saveName[0]), size);

    file.read(reinterpret_cast<char *>(&size), sizeof(size));
    fileName.resize(size);
    file.read(reinterpret_cast<char *>(&fileName[0]), size);

    file.read(reinterpret_cast<char *>(&size), sizeof(size));
    pathName.resize(size);
    file.read(reinterpret_cast<char *>(&pathName[0]), size);

    file.read(reinterpret_cast<char*>(&initialized), sizeof(bool));

    if (initialized){
        loadCompleteSafe();
    }
    file.close();
}

//Create an instance: through the host (fill with necessary parameters)
gameSave::gameSave(void){
    return;
}

//loads all the data of an already initialzed gamesave
void gameSave::loadCompleteSafe(void){
    return;
}



bool gameSave::saveSave(void){
    return true;
}

std::string gameSave::getSaveName(void){
    return saveName;
}



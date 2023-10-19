#include "gameSaveSummary.hpp"
#include "menu.hpp"
bool fileExists(const std::string &filename);


void readAllSaveSummaries(std::vector<gameSaveSummary> *summaryPointer){
    std::filesystem::path pathDirectory = std::string("sav/");
    for (auto &iterator : std::filesystem::recursive_directory_iterator(pathDirectory)){
        if (iterator.path().extension() == std::filesystem::path(".SAV")){
            summaryPointer->push_back(loadSaveSummary(iterator.path().string()));
        }
    }
}

gameSaveSummary loadSaveSummary(const std::string &filename){
    std::ifstream inputFile(filename, std::ios::binary);
    gameSaveSummary sumTmp;
    size_t size;

    inputFile.read(reinterpret_cast<char *>(&size), sizeof(size));
    sumTmp.saveName.resize(size);
    inputFile.read(reinterpret_cast<char *>(&sumTmp.saveName[0]), size);

    inputFile.read(reinterpret_cast<char *>(&size), sizeof(size));
    sumTmp.fileName.resize(size);
    inputFile.read(reinterpret_cast<char *>(&sumTmp.fileName[0]), size);

    inputFile.read(reinterpret_cast<char *>(&size), sizeof(size));
    sumTmp.pathName.resize(size);
    inputFile.read(reinterpret_cast<char *>(&sumTmp.pathName[0]), size);


    inputFile.close();
    return sumTmp;
}

bool createNewSafeFile(std::string newSavename, std::string *path){
    gameSaveSummary newSave;
    std::string newFileName = "WORLD_";
    std::string newpathName = "sav/"+newFileName;
    int i = 0;
    while (fileExists(newpathName+std::to_string(i)+".SAV")){
        i++;
    }
    newFileName = newFileName+std::to_string(i);
    newpathName = newpathName+std::to_string(i);
    std::cout <<newpathName <<std::endl;
    *path = newpathName;
    //Set up variables for uninitialized World save files
    newSave.saveName = newSavename;
    newSave.fileName = newFileName;
    newSave.pathName = newpathName;
    bool initialized = false;
    //Save the uninitiliazed file (only needs to be done once)
    //As soon as the file is opened and modified for the first time will initialize the save
    std::ofstream file(newpathName+".SAV", std::ios::binary);

    size_t size = newSavename.size();
    //Save the saveName (name given by player)
    file.write(reinterpret_cast<char *>(&size), sizeof(size));
    file.write(reinterpret_cast<char*>(newSave.saveName.data()), size);
    //Save the filename (name of the file)
    size = newSave.fileName.size();
    file.write(reinterpret_cast<char*>(&size), sizeof(size));
    file.write(reinterpret_cast<char*>(newSave.fileName.data()), size);
    //Save the path (just in case?)
    size = newSave.pathName.size();
    file.write(reinterpret_cast<char*>(&size), sizeof(size));
    file.write(reinterpret_cast<char*>(newSave.pathName.data()), size);
    //Save whether save has been initialized
    file.write(reinterpret_cast<char*>(&initialized), sizeof(bool));

    file.close(); 
    return true;
}

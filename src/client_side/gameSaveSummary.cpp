#include "gameSaveSummary.hpp"
#include "menu.hpp"
bool fileExists(const std::string &filename);


void readAllSaveSummaries(std::vector<gameSaveSummary> &saves){
    std::filesystem::path pathDirectory = std::string("sav/");
    for (auto &iterator : std::filesystem::recursive_directory_iterator(pathDirectory)){
        if (iterator.path().extension() == std::filesystem::path(".SAV")){
            saves.push_back(loadSaveSummary(iterator.path().string()));
        }
    }
}

gameSaveSummary loadSaveSummary(const std::string &filename){
    std::ifstream inputFile(filename, std::ios::binary);
    gameSaveSummary sumTmp;

    readStrOfFile(inputFile, sumTmp.game);

    readStrOfFile(inputFile, sumTmp.file);

    readStrOfFile(inputFile, sumTmp.path);

    inputFile.close();
    return sumTmp;
}

bool createNewSafeFile(std::string newSavename, std::string &path){
    gameSaveSummary newSave;
    std::string newFileName = "WORLD_";
    std::string newpathName = "sav/"+newFileName;
    int i = 0;
    while (fileExists(newpathName+std::to_string(i)+".SAV")){
        i++;
    }
    newFileName = newFileName+std::to_string(i);
    newpathName = newpathName+std::to_string(i);
    newFileName = newFileName +  ".SAV";
    newpathName = newpathName +  ".SAV";
    std::cout <<newpathName <<std::endl;
    path = newpathName;
    //Set up variables for uninitialized World save files
    newSave.game = newSavename;
    newSave.file = newFileName;
    newSave.path = newpathName;
    bool initialized = false;


    //Save the uninitiliazed file (only needs to be done once)
    //As soon as the file is opened and modified for the first time will initialize the save
    std::ofstream file(newpathName, std::ios::binary);

    writeStrToFile(file,newSave.game);

    writeStrToFile(file,newSave.file);

    writeStrToFile(file,newSave.path);

    //Save whether save has been initialized
    file.write(reinterpret_cast<char*>(&initialized), sizeof(bool));

    file.close(); 
    return true;
}

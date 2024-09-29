#include "save_management.hpp"


std::vector<gamesave_summary> read_all_save_summaries (void){
    std::vector<gamesave_summary> saveFiles;

    std::filesystem::path pathDirectory = std::string("sav/");
    for (auto &iterator : std::filesystem::recursive_directory_iterator(pathDirectory)){
        if (iterator.path().extension() == std::filesystem::path(".SAV")){
            saveFiles.push_back(load_summary(iterator.path().string()));
        }
    }
    return saveFiles;
}


gamesave_summary load_summary(const std::string &filename){
    std::ifstream inputFile(filename, std::ios::binary);
    gamesave_summary sumTmp;
    sumTmp.path = filename;

    readStrOfFile(inputFile, sumTmp.name);

    inputFile.read(reinterpret_cast<char*>(&sumTmp.initialized), sizeof(bool));

    inputFile.close();

    
    return sumTmp;

}


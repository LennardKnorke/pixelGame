#pragma once
#ifndef GAMESAVESUMMARY_HPP
#define GAMESAVESUMMARY_HPP
#include "stdlibs.hpp"

//struct:saves the filename, savename, pathname
typedef struct gameSaveSummary{
    std::string saveName;
    std::string fileName;
    std::string pathName;
}gameSaveSummary;


void readAllSaveSummaries(std::vector<gameSaveSummary> *summaryPointer);
gameSaveSummary loadSaveSummary(const std::string &filename);
bool createNewSafeFile(std::string newSavename, std::string *path);

#endif //GAMESAVESUMMARY_HPP
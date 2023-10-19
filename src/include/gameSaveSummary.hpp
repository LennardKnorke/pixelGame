#pragma once
#ifndef GAMESAVESUMMARY_HPP
#define GAMESAVESUMMARY_HPP
#include "stdlibs.hpp"

void readAllSaveSummaries(std::vector<gameSaveSummary> *summaryPointer);
gameSaveSummary loadSaveSummary(const std::string &filename);
bool createNewSafeFile(std::string newSavename, std::string *path);

#endif //GAMESAVESUMMARY_HPP
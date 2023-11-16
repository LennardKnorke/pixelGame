#pragma once
#ifndef GAMESAVESUMMARY_HPP
#define GAMESAVESUMMARY_HPP
#include "stdlibs.hpp"

////////////////////////////////////////////////////////////
/// \brief will open and read all basic game summaries of all available saves
///
/// \param saves vector to fill with available save file
////////////////////////////////////////////////////////////
void readAllSaveSummaries(std::vector<gameSaveSummary> &saves);

////////////////////////////////////////////////////////////
/// \brief opens a a valid savefile and returns a summary
///
/// \param filename path/file to open and read
////////////////////////////////////////////////////////////
gameSaveSummary loadSaveSummary(const std::string &filename);

////////////////////////////////////////////////////////////
/// \brief creates a new UNINITIALIZED savefile
///
/// \param newSavename name given to save
///
/// \param path UNKNOWN! CORRECT. What am I doing with it?!
///
/// \return true if successful, false otherwise
////////////////////////////////////////////////////////////
bool createNewSafeFile(std::string newSavename, std::string &path);

#endif //GAMESAVESUMMARY_HPP
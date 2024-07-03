#pragma once
#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <fstream>
#include <regex>


#define GAMEFONT_SIZE 30    //!< Font size for the game
#define MAX_SAVES 5         //!< Maximum number of saves load or create

/// @brief Macros: Game states
enum GAME_STATE {
    MENU,   //!< In the main menu
    GAME,   //!< In the main game loop
    QUIT    //!< Quit application
};

/// @brief Macros: Identify required networking settings
enum gameMode {
    Single,                             //!< Index for playing alone
    Host,                               //!< Index for hosting a game
    Join,                               //!< Index for joining a game
    undefined                           //!< INVALID
};


/// @brief Maximum lengths various check ups
enum maxInputLengths {
    userId = 15,        //!< maximum length of user identifer strings
    ip = 15,            //!< maximum length of ip strings
    port = 5,           //!< maximum length of port strings
    SaveName = 12       //!< maximum length of save file names
};


/**
 * @brief Checks if a file exists.
 * @param filename String with filename/relative location + type. e.g. "settings.bin"
 * @return True if it exists, else false 
 */
bool fileExists(const std::string& filename);


/**
 * @brief Reads size and then string out of an open binary file.
 * @param file Opened file to read out of.
 * @param str String variable to read into.
 */
void readStrOfFile(std::ifstream& file, std::string& str);



/**
 * \brief Writes size and then string into an open binary file.
 * \param file Opened file to write into.
 * \param str String to save into binary.
 */
void writeStrToFile(std::ofstream& file, std::string& str);


/**
 * @brief Checks if a given string is a valid IP address.
 * @param ip The IP address to check.
 * @return True if the IP address is valid, else false. 
 */
bool validIP(const std::string &ip);

#endif //UTILS_HPP
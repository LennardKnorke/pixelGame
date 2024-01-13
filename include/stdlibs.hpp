/**
 * @file stdlibs.hpp
 * @brief Header file that includes standard libraries, SFML libraries, and defines macros and structs.
 * 
 * This file includes various standard libraries such as algorithm, assert, cctype, cstdlib, etc.
 * It also includes SFML libraries for graphics, audio, network, window, and system.
 * Additionally, it defines macros and structs for game states, file operations, game modes, menu layers, input options, and message packages.
 * 
 * @note This file is part of the pixel_game project.
 */
#pragma once
#ifndef STDLIBS_HPP
#define STDLIBS_HPP
////////////////////////////////////////////////////////////
/// Standart libraries
////////////////////////////////////////////////////////////
#include <algorithm>
#include <assert.h>
#include <cassert>
#include <cctype>
#include <cstdlib> 
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <random>
#include <vector>

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
    #include <signal.h>
#endif


////////////////////////////////////////////////////////////
/// SFML libraries
////////////////////////////////////////////////////////////
// Downloaded from https://www.sfml-dev.org/files/SFML-2.6.0-windows-gcc-13.1.0-mingw-64-bit.zip for mingw (w64, over mysys)
// Copied files according to https://www.youtube.com/watch?v=rZE700aaT5I
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

////////////////////////////////////////////////////////////
/// OWN libraries (OPTIONAL!)
////////////////////////////////////////////////////////////
#include "tilesMakros.hpp"

////////////////////////////////////////////////////////////
/// Optional macros and structs
////////////////////////////////////////////////////////////
#define GAMEFONT_SIZE 35

// Allows them to be used in other header files!
class Application;
class Clients;
class inGameMenuButton;
class button;

/// @brief Macros: maximum length for various user text inputs
enum maxInputLengths {
    localIp = 15,   //!< Local IPv4 address
    publicIp = 15,  //!< Public IPv6 address
    port = 6,       //!< Port
    saveName = 12,  //!< Given during creation
    userId = 14     //!< Generated if no settings.bin file is created
};

/// @brief Macros: Game states
enum GAME_STATE {
    MENU,   //!< In the main menu
    GAME,   //!< In the main game loop
    QUIT    //!< Quit application
};

////////////////////////////////////////////////////////////
/// \brief Checks if a file exists.
///
/// \param filename String with filename/relative location + type. e.g. "settings.bin"
///
/// \return True if it exists, else false
////////////////////////////////////////////////////////////
bool fileExists(const std::string& filename);

////////////////////////////////////////////////////////////
/// \brief Reads size and then string out of an open binary file.
///
/// \param file Opened file to read out of.
///
/// \param str String variable to read into.
////////////////////////////////////////////////////////////
void readStrOfFile(std::ifstream& file, std::string& str);

////////////////////////////////////////////////////////////
/// \brief Writes size and then string into an open binary file.
///
/// \param file Opened file to write into.
///
/// \param str String to save into binary.
////////////////////////////////////////////////////////////
void writeStrToFile(std::ofstream& file, std::string& str);

/// \brief Contains the name given to a save, the path and filename (redundant?).
typedef struct gameSaveSummary {
    std::string game;   //!< Game/save name
    std::string file;   //!< File name
    std::string path;   //!< Full path
} gameSaveSummary;

/// @brief Macros: State for the playable INGAMELOOP
enum gameLoopState {
    Game,               //!< Index the main game window
    Pause,              //!< Index the ingame Menu
    SkillTree,          //!< Index the skilltree 
    QuitGame, QuitMenu  //!< Index quitting the INGAMELOOP
};

/// @brief Macros: Identify the current layer or the layer of buttons within the MAIN MENU
enum mainMenuLayerId {
    leave,              //!< Leave application
    Base,               //!<
    Settings,           //!< Display settings options
    GameMode,           //!< Decide between single, local or online
    HostVsClient,       //!< Decide between hosting joining (for local or online)
    Joining,            //!< About to enter an IP + port
    Hosting,            //!< About to choose/create a save
    Graphic,            //!< NOTHING
    Controls,           //!< NOTHING
    final               //!< Leave menu for gameLoop
};

/// @brief Macros: Identify required networking settings
enum gameMode {
    Single,                             //!< Index for playing alone
    local, Local_client, Local_host,    //!< Indicate local gameplay
    online, Online_client, Online_host, //!< Indicate online gameplay
    undefined                           //!< INVALID
};

////////////////////////////////////////////////////////////
/// \brief Identifies a gameMode, whether it is one for hosting.
///
/// \param mode Mode in question.
///
/// \return True or false if mode indicates hosting.
////////////////////////////////////////////////////////////
bool mode_Host(gameMode mode);

////////////////////////////////////////////////////////////
/// \brief Identifies a gameMode, whether it is one for online gameplay.
///
/// \param mode Mode in question.
///
/// \return True or false if mode indicates online gameplay.
////////////////////////////////////////////////////////////
bool mode_Online(gameMode mode);

/// @brief Macros: For different warning messages in menu
enum menuPopUps {
    NoPopUp,        //!< No error
    InvalidName,    //!< Invalid name picked for a new gamesave
    TooManySaves,   //!< Too many saves already created
    deleteSave      //!< About to delete an available save
};

#define n_keyInputOptions 7 //!< Maximum number of keyboard/mouse input options for the gameplay
/// @brief Macros: For keyboard/mouse gameplay input options. Defined for indexing!
enum gameInputOptions {
    up = 0,         //!< Up/w
    down = 1,       //!< Down/s
    left = 2,       //!< Left/a
    right = 3,      //!< Right/d
    nextItem = 4,   //!< 
    prevItem = 5,   //!<
    attack = 6      //!<
};

/// @brief Structure to track player input for various gameplay input options
/**
 * @brief Structure representing player controls.
 * 
 * This structure holds information about the player's control inputs and mouse aim.
 */
typedef struct playerControl {
    bool keyInput[n_keyInputOptions] = { false }; //!< Bools of different keyInput
    sf::Vector2i aim;                           //!< Mouse aim for weapons
} playerControl;

// Structure containing all information necessary for communication
typedef struct menuInput_msg {
    bool tmp;
} menuInput_msg;

typedef struct skillTreeInput_msg {
    bool tmp;
} skillTreeInput_msg;

/// @brief Contains input commands given by player. Send to server
typedef struct gameInput_msg {
    playerControl controls;
} gameInput_msg;

typedef struct playerMessagePackage {
    menuInput_msg menu;
    skillTreeInput_msg tree;
    gameInput_msg game;
} playerMessagePackage;

typedef struct serverMessagePackage {
    bool serverRuns;
} serverMessagePackage;

/// \brief One copy is run for each client on server and the respective client side.
/// Contains the information sent by the client and information to be sent by the server.
typedef struct combinedMsgPackage {
    serverMessagePackage server;
    playerMessagePackage client;
} combinedMsgPackage;


#endif // STDLIBS_HPP
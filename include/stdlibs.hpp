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
//Downloaded from https://www.sfml-dev.org/files/SFML-2.6.0-windows-gcc-13.1.0-mingw-64-bit.zip. for mingw (w64, over mysys)
//copied files according to https://www.youtube.com/watch?v=rZE700aaT5I
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
/// optional shit to allow other headers to mention these
////////////////////////////////////////////////////////////
#define GAMEFONT_SIZE 35
//Allows them to be used in other headerfiles!
class Application;
class Clients;
class inGameMenuButton;
class button;


/// @brief makros: maximum length for various user text inputs
enum maxInputLengths{
    localIp = 15,   //!< Local ipv4 adress
    publicIp = 15,  //!< public ipv6 adress
    port = 6,       //!< port
    saveName = 12,  //!< given during creation
    userId = 14     //!< generated if no settings.bin file is created
};


////////////////////////////////////////////////////////////
/// \brief checks if a file exist
///
/// \param filename string with filename/relative location + type. e.g. "settings.bin"
///
/// \return true if it exists, else false
////////////////////////////////////////////////////////////
bool fileExists(const std::string &filename);

////////////////////////////////////////////////////////////
/// \brief read size and then string out of an open binary file
///
/// \param file opened file to read out of
///
/// \param str string variable to read into
////////////////////////////////////////////////////////////
void readStrOfFile(std::ifstream &file, std::string &str);

////////////////////////////////////////////////////////////
/// \brief write size and then string into an open binary file
///
/// \param file opened file to write into
///
/// \param str string save into binary
////////////////////////////////////////////////////////////
void writeStrToFile(std::ofstream &file, std::string &str);

/// \brief contains: the name given to a save, the path and filename(redundant?)
typedef struct gameSaveSummary{
    std::string game;   //!< game/save name
    std::string file;   //!< file name
    std::string path;   //!< full path
}gameSaveSummary;


/// @brief makros: state for the playable INGAMELOOP
enum gameLoopState {
    Game,               //!< idx the main game window
    Menu,               //!< idx the ingame Menu
    SkillTree,          //!< idx the skilltree 
    QuitGame, QuitMenu  //!< idx quitting the INGAMELOOP
};


/// @brief makros: identify the current layer, or the layer of buttons within the MAIN MENU
enum mainMenuLayerId{
    leave,              //!< Leave application
    Base,               //!<
    Settings,           //!< display settings options
    GameMode,           //!< decide between single, local or online
    HostVsClient,       //!< decide between hosting joining (for local or online)
    Joining,            //!< about to enter an ip + port
    Hosting,            //!< about to choose/ create a safe
    Graphic,            //!< NOTHING
    Controls,           //!< NOTHING
    final               //!< leave menu for gameLoop
};


/// @brief makros: to identify required networking settings
enum gameMode {
    Single,                             //!< idx for playing alone
    local, Local_client, Local_host,    //!< indicate local gameplay
    online, Online_client, Online_host, //!< indicate online gameplay
    undefined                           //!< INVALID
};

////////////////////////////////////////////////////////////
/// \brief identifies a gameMode, whether it one for hosting
///
/// \param mode mode in question
///
/// \return true,false if mode indicates hosting
////////////////////////////////////////////////////////////
bool mode_Host(gameMode mode);

////////////////////////////////////////////////////////////
/// \brief identifies a gameMode, whether it one for online gameplay
///
/// \param mode mode in question
///
/// \return true,false if mode indicates online gameplay
////////////////////////////////////////////////////////////
bool mode_Online(gameMode mode);

/// @brief makros: for different warning messages in menu
enum menuPopUps {
    NoPopUp,        //!< No error
    InvalidName,    //!< Invalid name picked for a new gamesave
    TooManySaves,   //!< too many saves already created
    deleteSave      //!< about to delete an available save!
};


#define n_keyInputOptions 7 //!< maximum number of keyboard/mouse input options for the gameplay
/// @brief makros: for keyboard/mosue gameplay input options. Defined for indexing!
enum gameInputOptions {
    up = 0,         //!< up/w
    down = 1,       //!< down/s
    left = 2,       //!< left/a
    right  =3,      //!< right/d
    nextItem = 4,   //!< 
    prevItem = 5,   //!<
    attack = 6      //!<
};

/// @brief structure to track playerinput for various gameplay Input options
typedef struct playerControl{
    bool keyInput[n_keyInputOptions] = {false}; //!< bools of different keyInput
    sf::Vector2i aim;                           //!< mouse aim for weapons
}playerControl;


// Structure containing all information necessary for communication
typedef struct menuInput_msg{
    bool tmp;
}menuInput_msg;

typedef struct skillTreeInput_msg{
    bool tmp;
}skillTreeInput_msg;

/// @brief contains input commands given by player. send to server
typedef struct gameInput_msg{
    playerControl controls;
}gameInput_msg;

typedef struct playerMessagePackage {
    menuInput_msg menu;
    skillTreeInput_msg tree;
    gameInput_msg game;
}playerMessagePackage;


typedef struct serverMessagePackage{
    bool serverRuns;
}serverMessagePackage;


/// \brief one copy is run for each client on server and the respective client side
/// contains the information send by the client and information to be send by the server
typedef struct combinedMsgPackage {
    serverMessagePackage server;
    playerMessagePackage client;
}combinedMsgPackage;
#endif //STDLIBS_HPP
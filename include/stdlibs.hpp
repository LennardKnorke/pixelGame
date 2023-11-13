#pragma once
#ifndef STDLIBS_HPP
#define STDLIBS_HPP
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



//Downloaded from https://www.sfml-dev.org/files/SFML-2.6.0-windows-gcc-13.1.0-mingw-64-bit.zip. for mingw (w64, over mysys)
//copied files according to https://www.youtube.com/watch?v=rZE700aaT5I


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "tilesMakros.hpp"

#define MAX_LENGTH_KEY 14

//Allows them to be used in other headerfiles!
class Application;
class Clients;
class inGameMenuButton;
class button;

//struct saves: the name given, the pathname and filename(redundant?)
typedef struct gameSaveSummary{
    std::string saveName;
    std::string fileName;
    std::string pathName;
}gameSaveSummary;

enum gameLoopState {
    Game, Menu, SkillTree,QuitGame, QuitMenu
};

enum mainMenuLayerId{
    leave, Base, Settings,GameMode,HostVsClient,Joining,Hosting, Graphic,Controls,final
};

enum gameMode {
    Single,local, Local_client, online, Online_client,Local_host, Online_host, undefined
};
bool fileExists(const std::string &filename);
bool mode_Host(gameMode mode);
bool mode_Online(gameMode mode);

enum menuPopUps {
    NoPopUp,
    InvalidName,
    TooManySaves,
    deleteSave
};


#define n_gameInput 7
enum gameInputOptions {
    up = 0, down = 1, left = 2, right  =3, nextItem = 4, prevItem = 5, attack = 6
};
//structure to track playerinput
typedef struct playerControl{
    bool keyInput[n_gameInput] = {false};
    sf::Vector2i aim;//indicates the position of the cursor relative to the position of the controlled char
}playerControl;


// Structure containing all information necessary for communication
typedef struct menuStateInput_msg{
    playerControl controls;
}menuStateInput_msg;
typedef struct skillTreeInput_msg{
    int i;
}skillTreeInput_msg;
typedef struct mainGameInput_msg{
    int i;
}mainGameInput_msg;

typedef struct playerMessagePackage {
    gameLoopState state;
    menuStateInput_msg menu;
    skillTreeInput_msg tree;
    mainGameInput_msg game;
}playerMessagePackage;


typedef struct serverMessagePackage{
    bool serverRuns;
}serverMessagePackage;

typedef struct combinedMsgPackage {
    serverMessagePackage server;
    playerMessagePackage client;
}combinedMsgPackage;
#endif //STDLIBS_HPP
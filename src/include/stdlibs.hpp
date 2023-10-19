#pragma once
#ifndef STDLIBS_HPP
#define STDLIBS_HPP
#include <iostream>
#include <fstream>
#include <cassert>
#include <thread>
#include <string>
#include <cstring>
#include <thread>
#include <random>
#include <vector>
#include <filesystem>
#include <cstdlib> 
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

//structure to track playerinput
typedef struct userInputData{
    bool Jump;
    bool Down;
    bool Left;
    bool Right;
    bool nextItem;
    bool prevItem;
    bool attack;
    sf::Vector2i aim;//indicates the position of the cursor relative to the position of the controlled char
}userInputData;

//struct saves: the name given, the pathname and filename(redundant?)
typedef struct gameSaveSummary{
    std::string saveName;
    std::string fileName;
    std::string pathName;
}gameSaveSummary;

//User set controls
enum gameInputOptions {
    up = 0, down = 1, left = 2, right  =3, nextItem = 4, prevItem = 5, attack = 6
};
enum gameLoopState {
    Game, Menu, SkillTree,QuitGame, QuitMenu
};

enum mainMenuLayerId{
    leave, Base, Settings,GameMode,HostVsClient,Joining,Hosting, Graphic,Controls,final
};

enum gameMode{
    Single,local, Local_client, online, Online_client,Local_host, Online_host, undefined
};

enum menuPopUps {
    NoPopUp,
    InvalidName,
    TooManySaves,
    deleteSave
};

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#endif //STDLIBS_HPP
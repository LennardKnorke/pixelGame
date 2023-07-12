#pragma once
#ifndef STDLIBS_HPP
#define STDLIBS_HPP
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <cstring>
#include <thread>
#include <random>
#include <vector>
#include <filesystem>

//Downloaded from https://www.sfml-dev.org/files/SFML-2.6.0-windows-gcc-13.1.0-mingw-64-bit.zip. for mingw (w64, over mysys)
//copied files according to https://www.youtube.com/watch?v=rZE700aaT5I
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

typedef struct coordinates{
    int x;
    int y;
}coordinates;

typedef struct Rectangle {
    int height;
    int width;
    coordinates topLeft;
    coordinates topRight;
    coordinates bottomLeft;
    coordinates bottomRight;
    int leftX;
    int rightX;
    int upperY;
    int lowerY;
    coordinates center;
} Rectangle;

typedef struct Player{
    std::string name;
    std::string key;
}Player;

typedef struct gameSaveSummary{
    std::string saveName;
    std::string filename;
}gameSaveSummary;

typedef struct gameSave{
    std::string saveName;
    std::string filename;
    std::string owner;
    int playerNumber;
    std::vector<Player> PlayerData;
}gameSave;
#endif //STDLIBS_HPP
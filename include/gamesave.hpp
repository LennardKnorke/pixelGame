#pragma once
#ifndef GAMESAVE_HPP
#define GAMESAVE_HPP

#include <iostream>
#include <fstream>
#include <string>

#include "map_loader.hpp"
#include "utils.hpp"


class GameSave{
    private:
    std::string savePath;
    Map_Generator *map;

    public:
    GameSave(std::string path);
    ~GameSave(void);

    bool load(void);
    bool save(void);
};


#endif //GAMESAVE_HPP
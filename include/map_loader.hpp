/**
 * @file tiles_makros.hpp
 * @brief contains makros for the tile system
 */

#pragma once
#ifndef MAP_LOADER_HPP
#define MAP_LOADER_HPP

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "utils.hpp"

#define MAP_WIDTH 800
#define MAP_HEIGHT 2000

#define N_BIOMES 4

enum tile_c {
    NO_TILE = '0',
    GROUND = '1',
    DIRT = '2',
    STONE = '3',
    GOLD = '4',
    BRONZE = '5',
    SILVER = '6',
    SNOW = '7',
    WOOD = '8',
    LAVA = '9',
    COPPER = 'a',
    IRON = 'b',
};

enum biome_id {
    none = -1,
    PLAINS = 0,
};



class Map_Generator{
    private:
    unsigned int seed;
    void create_map(void);
    void set_horizone(void);
    void fill_dirt(int x, int y);
    public:
    Map_Generator(unsigned int seed);
    tile_c tile_map[MAP_WIDTH][MAP_HEIGHT];
    void save_map();


};

#endif // MAP_LOADER_HPP
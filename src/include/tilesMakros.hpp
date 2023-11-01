#pragma once
#ifndef TILESMAKROS_HPP
#define TILESMAKROS_HPP

enum tileTypes {
    Empty,
    Dirt,
    Stone,
    Iron,
    MagmaStone,
    BottomBorder
};

typedef struct Tile  {
    tileTypes type;
    bool uncovered;
    unsigned short posY;
    unsigned short posX;
}Tile;

#endif //TILESMAKROS_HPP
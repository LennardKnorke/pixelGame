#pragma once
#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <iostream>
#include <filesystem>


#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"


#define nr_menu_textures 1      //!< maximum number of menu textures to load
#define nr_backgroundMusic 2    //!< maximum number of music files to be loaded

/// @brief makros: idx menu textures
enum menuTextureIdxS {
    background = 0,     //!< background texture index
};

/// @brief makros: idx background music
enum musicIdx {
    mainMenu = 0,           //!< main Menu
    game_main = 1,          //!< main Game
};


class asset_manager {
    private:
    public:
    ~asset_manager();

    sf::Music backgroundMusic[nr_backgroundMusic];      //!< background music
    struct Textures {
        sf::Texture menu[nr_menu_textures];             //!< menu textures
        sf::Texture cursors;                            //!< cursor textures
    } textures;

    sf::Font gameFont;                                  //!< game font

    bool load(void);

};


#endif // ASSET_MANAGER_HPP
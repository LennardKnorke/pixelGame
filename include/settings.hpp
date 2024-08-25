#pragma once
#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <iostream>

#include "control_structs.hpp"
#include "utils.hpp"

class settings_class {
    private:
    std::string userId;                         // User ID
    bool *p_dev;                               // Developer mode

    public:
    float volumne;                              // Volume of the game
    inGameInputKey controls[n_keyInputOptions]; // Controls
    short fps;                                 // Frames per second
    struct res_settings{
        bool fullscreen;
        unsigned int width;
        unsigned int height;
        sf::Vector2f scaling;
    } res;


    settings_class(bool file_exists, bool *dev);
    ~settings_class();

    std::string get_userId(void);

    void load(void);
    void save(void);
};


#endif // SETTINGS_HPP
#pragma once
#ifndef SAVE_MANAGEMENT_HPP
#define SAVE_MANAGEMENT_HPP

#include <iostream>
#include <filesystem>

#include "control_structs.hpp"
#include "utils.hpp"


/// @brief Structure that holds the name and path of a save file and the initialization status
typedef struct gamesave_summary{
    std::string name;   // Name of the save file
    std::string path;   // Path to the save file. ONLY ONE THAT IS NOT SAVED IN FILE
    bool initialized;   // True if the save file has been initialized
} gamesave_summary;


/**
 * @brief opens every .SAV file in the save directory and reads the name.
 * @return std::vector<gamesave_summary> Returns a list of summaries for available save files.
 */
std::vector<gamesave_summary> read_all_save_summaries (void);

/**
 * @brief opens a given .SAV file and reads the name and initialization status.
 * @param filepath path to file
 * @return gamesave_summary struct of the save file
 */
gamesave_summary load_summary(const std::string &filepath);


/// @brief Struct containing all settings of the application
struct app_settings{
    std::string userId;                         // User ID

    float volumne;                              // Volume of the game
    
    inGameInputKey controls[n_keyInputOptions]; // Controls
    
    short fps;                                  // Frames per second
    bool fullscreen;                            // Fullscreen mode
    unsigned int res_x;                         // Resolution width
    unsigned int res_y;                         // Resolution height
};

void save_settings(app_settings s);

app_settings load_settings(void);

#endif // SAVE_MANAGEMENT_HPP
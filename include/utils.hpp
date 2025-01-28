#pragma once
#ifndef UTILS_HPP
#define UTILS_HPP

//
#include <string>
#include <fstream>
#include <random>
#include <regex>
#include <optional>
#include <variant>
#include <SFML/Network.hpp>


#define GAMEFONT_SIZE 30                //!< Font size for the game
#define MAX_SAVES 5                     //!< Maximum number of saves load or create

#define DEFAULT_PORT 53000              //!< Default port for networking

#define DEFAULT_FPS 60                  //!< Default frames per second
#define DEFAULT_VOLUME 50.0             //!< Default volume level
#define DEFAULT_WINDOW_WIDTH 1920       //!< Default window width
#define DEFAULT_WINDOW_HEIGHT 1080       //!< Default window height
#define DEFAULT_FULLSCREEN 0            //!< Default fullscreen setting



enum appState{
    MENU,               //!< in the main menu
    GAME,               //!< in the main game loop
    EXIT                //!< quit application
}; 

enum gameMode{
    SINGLEPLAYER,               //!< singleplayer mode
    MULTIPLAYER_HOST,            //!< multiplayer mode
    MULTIPLAYER_JOIN,             //!< multiplayer mode
    undefined
};

struct stateInfo {
    appState state = EXIT; //!< default state of the application

    struct gameModeInfo {
        gameMode mode = undefined; //!< default game mode

        struct ConnectTo {
            std::optional<sf::IpAddress> ip = std::nullopt;
            unsigned short port = 0;
        };

        struct HostFile {
            std::string file = "";
        };

        // Use a variant to represent connectTo or hostFile
        std::variant<ConnectTo, HostFile> gameInfo = ConnectTo{};

        // Helper to set mode and initialize associated data
        void setMode(gameMode newMode) {
            mode = newMode;
            switch (newMode) {
                case MULTIPLAYER_JOIN:
                    gameInfo = ConnectTo{}; // Reset to default ConnectTo
                    break;
                case MULTIPLAYER_HOST || SINGLEPLAYER:
                    gameInfo = HostFile{}; // Reset to default HostFile
                    break;
                default:
                    // Undefined or fallback case
                    break;
            }
        }
        // Access helpers for connectTo and hostFile
        ConnectTo& getConnectTo() {
            return std::get<ConnectTo>(gameInfo);
        }

        HostFile& getHostFile() {
            return std::get<HostFile>(gameInfo);
        }
    } next;

    void setAppState(appState newState) {
        state = newState;
    }
};

/// @brief Maximum lengths various check ups
enum maxInputLengths {
    userId = 15,                        //!< maximum length of user identifer strings
    ip = 15,                            //!< maximum length of ip strings
    port = 5,                           //!< maximum length of port strings
    SaveName = 12                       //!< maximum length of save file names
};


/**
 * @brief Reads size and then string out of an open binary file.
 * @param file Opened file to read out of.
 * @param str String variable to read into.
 */
void readStrOfFile(std::ifstream& file, std::string& str);



/**
 * \brief Writes size and then string into an open binary file.
 * \param file Opened file to write into.
 * \param str String to save into binary.
 */
void writeStrToFile(std::ofstream& file, std::string& str);


/**
 * @brief Checks if a given string is a valid IP address.
 * @param ip The IP address to check.
 * @return True if the IP address is valid, else false. 
 */
bool validIP(const std::string &ip);


/**
 * @brief Finds a free port to use for networking
 * @return unsigned short 
 */
unsigned short findFreePort(void);


/**
 * @brief Get a random seed number
 * @return unsigned int 
 */
unsigned int getRandomSeed(void);


#endif //UTILS_HPP
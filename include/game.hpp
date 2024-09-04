#pragma once
#ifndef GAME_HPP
#define GAME_HPP

// Include standard libraries
#include <iostream>

// Include SFML libraries
#include <SFML/Graphics.hpp>

// Include project libraries
#include "control_structs.hpp"
#include "cursor.hpp"
#include "map_loader.hpp"
#include "settings.hpp"
#include "utils.hpp"

/// @brief Enumeration of possible return states
enum GameStates {
    RUNNING_,           //!< Game is running
    QUIT_ERROR,         //!< FATAL ERROR. Quiting Application (initiated by program)
    QUIT_NOERROR,       //!< Quit the game without error (initiated by user)
    MENU_
};

class game_save{
    private:
    bool *DEV_MODE;
    bool hosting;
    unsigned int seed;
    
    char map[MAP_WIDTH][MAP_HEIGHT];

    std::vector<std::string> players;
    int player_idxs[4];

    public:
    bool valid;
    gamesave_summary summary;
    game_save(gamesave_summary *s, std::string host_id, bool *dev);
    game_save();
    ~game_save();

    short add_player(std::string id);

    void init_save(std::string host_id);
    sf::Thread *save_thread;
    void save_save_t(void);
    void load_save(std::ifstream &inputFile);
};


class Game{
    public:
    /// @brief Constructor for playing singleplayer game
    Game(gameMode m, sf::RenderWindow *win, Cursor *cur, bool dev, settings_class *ps, asset_manager *am);


    /**
     * @brief Runs the game loop
     */ 
    bool DEV_MODE;                          //!< Developer mode
    settings_class *settings;               //!< Settings for the game
    asset_manager *assets;                  //!< Assets for the game


    bool loading_done;                      //!< Loading done
    GameStates gameState = RUNNING_;        //!< Current state of the game
    gameMode mode;                          //!< Mode of the game
    
    sf::RenderWindow *window;               //!< Window for the game
    Cursor *cursor;                         //!< Cursor for the game

    void loadingScreen_t(void);
    sf::Thread *loadingThread;
};


class Host_Game : public Game{
    public:
    Host_Game(gameMode m, sf::RenderWindow *win, Cursor *cur, bool dev, settings_class *ps, asset_manager *am, gamesave_summary s);

    private:
    game_save *save;

    sf::IpAddress ip;           //!< IP players can connect to
    unsigned short port;        //!< Port players can connect to
    sf::TcpListener listener;   //!< Listener for incoming connections
    
};

class Join_Game : public Game{
    public:
    Join_Game(gameMode m, sf::RenderWindow *win, Cursor *cur, bool dev, settings_class *ps, asset_manager *am, sf::IpAddress ip, unsigned short port);

    private:
    sf::IpAddress h_ip;           //!< IP to connect to
    unsigned short h_port;        //!< Port to connect to

};
#endif // GAME_HPP
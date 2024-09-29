#pragma once
#ifndef GAME_HPP
#define GAME_HPP

// Include standard libraries
#include <iostream>
#include <cstdlib>

#include <chrono>
#include <thread>

#include <Windows.h>


// Include SFML libraries
#include <SFML/Graphics.hpp>

// Include project libraries
#include "asset_manager.hpp"
#include "control_structs.hpp"
#include "cursor.hpp"
#include "settings.hpp"
#include "utils.hpp"

enum local_states{
    INIT_,
    CONNECTING_,
    RUNNING_,
    QUIT_
};

enum inGameLayer {
    menu,
    game,
    skilltree
};

class Local_Client_Game{
    private:
    bool *DEV_MODE;
    settings_class *settings;               //!< Settings for the game
    asset_manager *assets;                  //!< Assets for the game

    local_states state = INIT_;        //!< Current state of the game
    
    sf::TcpSocket socket;                   //!< Socket for the game
    struct {
        unsigned short port;
        sf::IpAddress ip;
    } host_adress;                          //!< Loading tools for the game

    sf::RenderWindow *window;               //!< Window for the game
    Cursor *cursor;                         //!< Cursor for the game

    void connectToServer(void);
    void loadingScreen_t(void);
    sf::Thread *loadingThread;
    void receiveWorld(void);

    //

    void draw(inGameLayer &layer, sf::RenderWindow &window);
    void handleUserInput(inGameLayer &layer, sf::RenderWindow &window);
        void handleGameInput(inGameLayer &layer, sf::RenderWindow &window);
        void handleMenuInput(inGameLayer &layer, sf::RenderWindow &window);
        void handleSkilltreeInput(inGameLayer &layer, sf::RenderWindow &window);
    void updateClient(void);


    public:
    /// @brief Constructor for playing singleplayer game
    Local_Client_Game(sf::RenderWindow *win, Cursor *cur, settings_class *set, asset_manager *ass, bool *dev, unsigned short host_port, std::string host_ip);
    
    GAME_STATE run(void);
};
#endif // GAME_HPP
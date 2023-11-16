#pragma once
#ifndef LOCAL_GAME_HPP
#define LOCAL_GAME_HPP
#include "stdlibs.hpp"


#define n_menugameButtons 3 //!< number of ingame menubuttons

////////////////////////////////////////////////////////////
/// \brief if host launch command line terminal. Parameters contain the command line arguments
///
/// \param adress pointer to host ip adress to set. used as command line argument
///
/// \param port short in which a port is set for hosting. used as command line argument
///
/// \param HostId command line argument: str with hostId for server command line argument
///
/// \param pathToSave command line argument: gamesave path for server to load 
///
/// \param Mode command line argument: mode for server to launch
///
/// \return true/false if command line launched succesfully
////////////////////////////////////////////////////////////
bool initServerProcess(sf::IpAddress *adress, unsigned short &port, std::string HostId, std::string pathToSave, gameMode Mode);

////////////////////////////////////////////////////////////
/// \brief set appropriate host ip adress to connect to and use in command line
///
/// \param adress pointer to host ip adress to set. used as command line argument
///
/// \param Mode online, local. decides which ip adress to set
///
/// \return true/false if VALID ip adress has been set
////////////////////////////////////////////////////////////
bool setHostIp(sf::IpAddress *adress, gameMode Mode);

////////////////////////////////////////////////////////////
/// \brief set open and appropriate host port to connect to and use in command line
///
/// \param port variable to use as command line
///
/// \param adress pointer to host ip adress to set. will be set to None if no port is found!
///
/// \param Mode online, local. decides which port to set
///
/// \return true/false if VALID port has been set
////////////////////////////////////////////////////////////
bool setHostPort(unsigned short &port, sf::IpAddress *adress, gameMode Mode);


// BASIC SET UP. THEREFORE NO EXTENSIVE INSTRUCTION. Update soon!
class inGameMenuButton{
    public:
    bool focus;
    std::string textString;
    sf::Text text;
    gameLoopState followUpState;
    sf::Sprite imageSprite;

    inGameMenuButton(int currentIdx, Application *app);
    void update(sf::Vector2f mousePos);
    void draw(sf::RenderWindow *window);
    ~inGameMenuButton(void);
};
#endif //LOCAL_GAME_CLIENT_HPP
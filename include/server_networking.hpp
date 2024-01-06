#pragma once
#ifndef SERVER_NETWORKING_HPP
#define SERVER_NETWORKING_HPP
#include "stdlibs.hpp"
#include "gamesave.hpp"

/// @brief timer to track timeouts.
struct timeOutTimer {
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> current;
    std::chrono::seconds difference;
};

/// @brief contains all information for a client thread to handle a single client
struct serverClient {
    bool active = false;                //!< active thread/ client
    bool connected = false;             //!< client connected
    unsigned short id = USHRT_MAX;      //!< id/position
    std::string key = "";               //!< client Id
    sf::Thread *threadPtr = nullptr;    //!< pointer to client thread. NULL if not active
    sf::TcpSocket socket;               //!< socket to handle exchange
    timeOutTimer timeOut;               //!< timer

    //Only the client gets to fill the player message
    //Only the server gets to fill the server message
    //Both are also present on each client

    combinedMsgPackage msgPackages;     //!< message exchange between client and server
};

////////////////////////////////////////////////////////////
/// @brief checks if a client structure is in use
///
/// @param c pointer to struct
///
/// @return true if client is in use and NOT available, else false
////////////////////////////////////////////////////////////
bool clientInUse(serverClient *c);

////////////////////////////////////////////////////////////
/// @brief frees client structure, end thread and reset values
///
/// @param c pointer to struct
////////////////////////////////////////////////////////////
void clearClient(serverClient *c);

////////////////////////////////////////////////////////////
/// @brief runs the server application
////////////////////////////////////////////////////////////
class Server {
    public:
    ////////////////////////////////////////////////////////////
    /// @brief only constructor. also runs the server by default or exits in case of error
    ///
    /// @param port assigned port for the server
    ///
    /// @param adress ip adress assigned to server
    ///
    /// @param savePath path to save
    ///
    /// @param hostId key
    ///
    /// @param modeToLoad singleplayer, online, local etc.
    ////////////////////////////////////////////////////////////
    Server(unsigned short port, sf::IpAddress adress, std::string savePath, std::string hostId, gameMode modeToLoad);
    
    private:
    ////////////////////////////////////////////////////////////
    /// \brief set up a server for a local or single player game
    ///
    /// \return true if successfull, else false
    ////////////////////////////////////////////////////////////
    bool setUp_LocalServer();

    ////////////////////////////////////////////////////////////
    /// \brief set up a server for online play
    ///
    /// \return true if successfull, else false
    ////////////////////////////////////////////////////////////
    bool setUp_OnlineServer();

    ////////////////////////////////////////////////////////////
    /// \brief inits a gameSave instance
    ///
    /// \param savePath path to save
    ///
    /// \return true if successfull, else false
    ////////////////////////////////////////////////////////////
    bool loadGameSave(std::string savePath);

    ////////////////////////////////////////////////////////////
    /// \brief runs in a seperate thread and accepts new connections
    ////////////////////////////////////////////////////////////
    void acceptConnections();

    ////////////////////////////////////////////////////////////
    /// \brief get a free position to handle a client
    ///
    /// \returns idx or MAX SHORT if none is available
    ////////////////////////////////////////////////////////////
    unsigned short getFreeClientPosition();
    
    ////////////////////////////////////////////////////////////
    /// \brief checks if the host is connected
    ///
    /// \return true if host is connected, else false
    ////////////////////////////////////////////////////////////
    bool hostConnectionCheck();

    ////////////////////////////////////////////////////////////
    /// \brief updates a timeout timer
    ///
    /// \param timer pointer to timeOutTimer
    ////////////////////////////////////////////////////////////
    void updateTimeOut(timeOutTimer *timer);

    ////////////////////////////////////////////////////////////
    /// \brief resets and starts a timeout timer
    ///
    /// \param timer pointer to timeOutTimer
    ////////////////////////////////////////////////////////////
    void initTimeOut(timeOutTimer *timer);


    unsigned short serverPort;                      //!< port
    sf::IpAddress serverAdress;                     //!< ipAdress
    sf::TcpListener serverListener;                 //!< sfml, listens to new connections
    struct hostinfo{
        bool connected;
        std::string hostId;                         //!< hostId
        timeOutTimer timeOutTimer;                  //!< timeouttimer for host
    }hostConnectionState;                           //!< tracks hostId and connection
    
    gameMode mode;                                  //!< singleplayer, etc.
    unsigned short maxPlayers;                      //!<
    
    std::vector<serverClient*> clientsAvailable;    //!< available client structures
    gameSave *GAME;                                 //!< gameSave to be run on server

};

void clientThread(serverClient *SC);
void readClientMessage(sf::Packet &package);

#endif //SERVER_NETWORKING_HPP
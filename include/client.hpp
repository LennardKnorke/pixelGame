#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "stdlibs.hpp"

////////////////////////////////////////////////////////////
///
class Clients {
    protected:

    sf::TcpSocket socket;   //!< socket to connect with client
    bool isConnected;       //!< true/false if active connection

    public:

    ////////////////////////////////////////////////////////////
    /// \brief brief description
    ///
    /// \param hostIp host ipadress
    ///
    /// \param hostPort host port
    ///
    /// \return true/false if succesful connection
    ////////////////////////////////////////////////////////////
    bool connect(sf::IpAddress hostIp, unsigned short hostPort);

    ////////////////////////////////////////////////////////////
    /// \brief send sfml package
    ///
    /// \param OutGoingPacket filled in package to send via sfml
    ///
    /// \return true/false if succesful exchange
    ////////////////////////////////////////////////////////////
    bool sendData(sf::Packet &OutGoingPacket);

    ////////////////////////////////////////////////////////////
    /// \brief receive sfml package
    ///
    /// \param incomingDestination sfml package to be filled
    ///
    /// \return true/false if succesful exchange
    ////////////////////////////////////////////////////////////
    bool receiveData(sf::Packet &incomingDestination);

    ////////////////////////////////////////////////////////////
    /// \brief disconnect from host
    ////////////////////////////////////////////////////////////
    void disconnect(void);
};
#endif //CLIENT_HPP
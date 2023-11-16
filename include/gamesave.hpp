#pragma once
#ifndef GAMESAVE_HPP
#define GAMESAVE_HPP
#include "stdlibs.hpp"


/// @brief saves player key and character attributes!
typedef struct Player{
    std::string key;        //!< key associated with this character
    
}Player;

class NPC;

////////////////////////////////////////////////////////////
/// \brief contains save/ world and player information of active savegame
////////////////////////////////////////////////////////////
class gameSave {
    public:
    
    ////////////////////////////////////////////////////////////
    /// \brief load and run a save upon initiation
    ///
    /// \param savePath path to game for loading
    ////////////////////////////////////////////////////////////
    gameSave(std::string savePath);

    ////////////////////////////////////////////////////////////
    /// \brief (NOT FINISHED) Create a new player
    ///
    /// \param userKey key associated with player
    ////////////////////////////////////////////////////////////
    Player addPlayer(std::string userKey);

    ////////////////////////////////////////////////////////////
    /// \brief return saveName
    ///
    /// \return str name given
    ////////////////////////////////////////////////////////////
    std::string getSaveName(void);

    ////////////////////////////////////////////////////////////
    /// \brief return path
    ///
    /// \return path as string
    ////////////////////////////////////////////////////////////
    std::string getSavePath(void);

    ////////////////////////////////////////////////////////////
    /// \brief return saveName
    ///
    /// \return str of file name
    ////////////////////////////////////////////////////////////
    std::string getFileName(void);

    ////////////////////////////////////////////////////////////
    /// \brief signals to server that the save has been loaded
    ///
    /// \return true/false whether succesfull
    ////////////////////////////////////////////////////////////
    bool getSaveLoadedState(void);

    private:
    
    ////////////////////////////////////////////////////////////
    /// \brief load Save
    ///
    /// \param savePath path to save
    ///
    /// \return true/false if loading was successfull
    ////////////////////////////////////////////////////////////
    bool loadSave(std::string savePath);

        ////////////////////////////////////////////////////////////
        /// \brief load an initialized save
        ///
        /// \param file opened file stream to read out
        ////////////////////////////////////////////////////////////
        void loadCompleteSafe(std::ifstream &file);

        ////////////////////////////////////////////////////////////
        /// \brief initialized a loaded but uninitialized save file (NOT FINISHED)
        ////////////////////////////////////////////////////////////
        void initUninitialized(void);
        ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    /// \brief save the active savefile
    ///
    /// \return true/false if successfully saved
    ////////////////////////////////////////////////////////////
    bool saveSave(void);


    /// START OF INFORMATION IN GAMESAVE_SUMMARIES

    std::string saveName;   //!< Name of world/save
    std::string fileName;   //!< name of file
    std::string pathName;   //!< path

    /// END OF INFORMATION IN GAMESAVE_SUMMARIES

    bool initialized = false;               //!< initialized save? seed set? tiles generated?
    bool loadedComplete = false;            //!< indicates whether saves has been loaded succesfully
    std::vector<int> seed;                  //!< World Seed
    Tile tileMap[10][10];                   //!< WORLD MAP
    std::vector<Player*> activePlayers;     //!< Pointers to active players
    std::vector<Player> clientsData;        //!< Players created in world
    std::vector<NPC*> npcs;                 //!< List of active nps (friendly + enemies)
    
};



#endif //GAMESAVE_HPP
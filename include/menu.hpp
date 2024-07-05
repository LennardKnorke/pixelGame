/**
 * @file menu.hpp
 * @brief Contains the declaration of the MainMenu class and its member functions.
 */

#pragma once
#ifndef MENU_HPP
#define MENU_HPP



#include "SFML/Audio.hpp"

#include "cursor.hpp"
#include "menuButtons.hpp"
#include "save_management.hpp"

enum menuPopUps{
    noPopUp,
    tooManySaves,
    invalidName,
    saveError,
    invalidIp
};

/**
 * @class MainMenu
 * @brief Represents the main menu of the game.
 */
class MainMenu {
    public:
    /**
     * @brief Constructs a MainMenu object.
     * @param window A pointer to the RenderWindow object.
     * @param res The resolution of the window.
     * @param bg_Texture pointer to the background texture.
     * @param font pointer to font to be used for the buttons and text.
     */
    MainMenu(sf::RenderWindow *window, Cursor *cursor, resolution_tools resolutions, sf::Texture *bg_Texture, sf::Font *font, sf::Music *bg_music);

    /**
     * @brief Destroys the MainMenu object.
     */
    ~MainMenu(void);

    /**
     * @brief Runs the main menu and returns the selected game state.
     * @return The selected game state.
     */
    GAME_STATE runMenu(void);

    gameMode getMode(void);
    sf::IpAddress getHostIp(void);
    unsigned short getHostPort(void);
    gamesave_summary getChosenSave(void);

    private:
    
    mainMenuLayerId currentLayer = mainMenuLayerId::base;   /**< The current layer of the main menu. */
    menuPopUps menuWarning = menuPopUps::noPopUp;           /**< The menu warning pop-up. */
    bool writing = false;                                   /**< Flag indicating whether text input is allowed. */
    
    sf::Font *font;                                         /**< The font to be used for the buttons and text. */
    sf::RenderWindow *window;    
    sf::Sprite backgroundSprite;                            /**< The background sprite. */
    
    resolution_tools res;                                   /**< The resolution of the window. */
    
    Cursor *cursor;
    std::vector<button *> menuButtons;
    sf::Text warningMessage;                                /**< The warning message text. */
    sf::Music *bg_music;


    gameMode mode = gameMode::undefined;                    /**< The game mode. */
    std::vector<gamesave_summary> availableSaveFiles;       /**< The available save files. */

    gamesave_summary chosen_save;                           /**< The chosen save file. */
    sf::IpAddress hostIp = sf::IpAddress::None;              /**< The host IP address. */
    unsigned short hostPort = DEFAULT_PORT;                         /**< The host port. */
    
    /**
     * @brief Initializes the error message text for the current menu pop up
     * @return The initialized error message text.
     */
    void initErrorMessage();

    /**
     * @brief Sets up the menu buttons.
     */
    void setUpMenuButtons(void);

    /**
     * @brief Draws the menu buttons.
     */
    void drawMenuButtons(void);

    /**
     * @brief Draws the menu pop-up.
     */
    void drawMenuPopUp(void);

    /**
     * @brief Handles the escape key press event.
     * @param mode The game mode.
     */
    void escapeKeyPressed(void);

    /**
     * @brief Handles the enter key press event.
     */
    void enterKeyPressed(void);

    /**
     * @brief Handles the delete key press event.
     */
    void deleteKeyPressed(void);

    /**
     * @brief Handles the text entered event.
     * @param ev The text entered event.
     */
    void textEntered(sf::Event ev);

    /**
     * @brief Handles the mouse button pressed event.
     */
    void mouseButtonPressed(void);

    /**
     * @brief Creates a new save file with the given name.
     * @param newSaveName The name of the new save file.
     * @return True if the save file was created successfully, false otherwise.
     */
    bool createSave(std::string newSaveName);

    /**
     * @brief Creates a new safe file with the given name and returns the path to load.
     * @param newSavename The name of the new save file.
     * @param pathToLoad The path to load.
     * @return True if the safe file was created successfully, false otherwise.
     */
    bool createNewSafeFile(std::string newSavename);

    /**
     * @brief Gets the previous layer based on the game mode.
     * @return The previous layer.
     */
    mainMenuLayerId getPreviousLayer(void);


    /**
     * @brief Gets the menu picks based on the game mode.
     */
    void getMenuPicks(void);
};

#endif //MENU_HPP
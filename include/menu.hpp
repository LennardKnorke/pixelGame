/**
 * @file menu.hpp
 * @brief Contains the declaration of the MainMenu class and its member functions.
 */

#pragma once
#ifndef MENU_HPP
#define MENU_HPP

#include <optional>

#include "SFML/Audio.hpp"

#include "asset_manager.hpp"
#include "cursor.hpp"
#include "menuButtons.hpp"
#include "save_management.hpp"
#include "settings.hpp"
#include "utils.hpp"

/// @brief Enum to index message pop ups in main menu loop.
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
     * @param res The resolution of the window.
     * @param bg_Texture pointer to the background texture.
     * @param font pointer to font to be used for the buttons and text.
     * @param bg_music pointer to the background music for the menu
     */
    MainMenu(sf::RenderWindow *window, Cursor *cursor, settings_class *ps, asset_manager *assets, stateInfo *stateTracker, bool dev);

    /**
     * @brief Destroys the MainMenu object.
     */
    ~MainMenu(void);

    void run(void);

    /**
     * @brief Draw the menu in the given window
     */
     void draw(void);

    /**
     * @brief handle user input events
     */
    void handleEvents(void);

    /**
     * @brief update menu button and control states
     */
     void update(void);
    
    bool running = true;                                    /**< Flag indicating whether the main menu is running. */                                  /**< The next game state. */
    mainMenuLayerId currentLayer = mainMenuLayerId::base;   /**< The current layer of the main menu. */

    // Private Variables
    private:
    bool DEV_MODE = false;                                  /**< Flag indicating whether the game is in development mode. */
    stateInfo *stateTracker; /**< The next game state. */
    menuPopUps menuWarning = menuPopUps::noPopUp;           /**< The menu warning pop-up. */
    bool writing = false;                                   /**< Flag indicating whether text input is allowed. */
    
    settings_class *settings;                                  /**< The resolution of the window. */
    asset_manager *assets;                                  /**< The asset manager. */
    Cursor *cursor;
    std::vector<button *> menuButtons;
    sf::Text *warningMessage;                                /**< The warning message text. */
    sf::RenderWindow *window;                               /**< The window to draw the menu in. */

    
    std::vector<gamesave_summary> availableSaveFiles;       /**< The available save files. */

    

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
    void drawMenuButtons();

    /**
     * @brief Draws the menu pop-up.
     */
    void drawMenuPopUp();

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
    void textEntered(std::optional<sf::Event> event);

    /**
     * @brief Handles the mouse button pressed event.
     */
    void mouseButtonPressed();

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

    void changeFullscreen();
};

#endif //MENU_HPP
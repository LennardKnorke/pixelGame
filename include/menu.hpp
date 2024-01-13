/**
 * @file menu.hpp
 * @brief Contains the declaration of the MainMenu class and its member functions.
 */

#pragma once
#ifndef MENU_HPP
#define MENU_HPP
#include "stdlibs.hpp"

/**
 * @class MainMenu
 * @brief Represents the main menu of the game.
 */
class MainMenu {
    public:
    /**
     * @brief Constructs a MainMenu object.
     * @param appPointer A pointer to the Application object.
     */
    MainMenu(Application *appPointer);
    gameMode mode; /**< The game mode. */

    /**
     * @brief Destroys the MainMenu object.
     */
    ~MainMenu(void);

    /**
     * @brief Runs the main menu and returns the selected game state.
     * @return The selected game state.
     */
    GAME_STATE runMenu(void);

    private:
    mainMenuLayerId currentLayer; /**< The current layer of the main menu. */
    menuPopUps menuWarning; /**< The menu warning pop-up. */
    sf::Text warningMessage; /**< The warning message text. */
    bool allowTextInput; /**< Flag indicating whether text input is allowed. */

    Application *applicationPointer; /**< A pointer to the Application object. */
    sf::RenderWindow *window; /**< A pointer to the RenderWindow object. */
    std::vector<button *> menuButtons; /**< The menu buttons. */
    std::vector<gameSaveSummary> availableSaveFiles; /**< The available save files. */
    sf::Sprite backgroundSprite; /**< The background sprite. */

    /**
     * @brief Initializes the error message text.
     * @param font The font to be used for the text.
     * @return The initialized error message text.
     */
    sf::Text initErrorMessage(sf::Font &font);

    /**
     * @brief Reads all save file summaries.
     */
    void readAllSaveSummaries(void);

    /**
     * @brief Loads a save file summary from a given filename.
     * @param filename The filename of the save file.
     * @return The loaded save file summary.
     */
    gameSaveSummary loadSaveSummary(const std::string &filename);

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
     * @brief Handles the input error pop-up.
     * @param ev The input event.
     */
    void inputErrorPopUp(sf::Event &ev);

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
    bool createNewSafeFile(std::string newSavename, std::string &pathToLoad);

    /**
     * @brief Gets the previous layer based on the game mode.
     * @return The previous layer.
     */
    mainMenuLayerId getPreviousLayer(void);

    /**
     * @brief Checks if the character input is valid.
     * @param c The character input.
     * @param activeLength The active length.
     * @param n The number of characters.
     * @return True if the character input is valid, false otherwise.
     */
    bool validCharacterInput(sf::Uint32 c, int activeLength, int n);

    /**
     * @brief Sets the active safe with the given save name and path to be loaded.
     * @param saveName The save name.
     * @param pathToBeLoaded The path to be loaded.
     */
    void setActiveSafe(std::string saveName, std::string *pathToBeLoaded);

    /**
     * @brief Gets the menu picks based on the game mode.
     */
    void getMenuPicks(void);

    /**
     * @brief Sets the menu pop-up message.
     */
    void setMenuPopUpMessage(void);
};

#endif //MENU_HPP
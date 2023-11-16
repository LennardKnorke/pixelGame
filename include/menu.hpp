#pragma once
#ifndef MENU_HPP
#define MENU_HPP
#include "stdlibs.hpp"

////////////////////////////////////////////////////////////
/// \brief sets up sfml text to be used for Error messages
///
/// \param font pointer to font to be used
///
/// \param scaling float about the 16:9 scaling (base: 1920x1080) to be applied. (OPTIONAL!)
///
/// \return sfml text
////////////////////////////////////////////////////////////
sf::Text initErrorMessage(sf::Font *font, float scaling);

////////////////////////////////////////////////////////////
/// \brief sets up sfml text to be used for Error messages
///
/// \param font pointer to font to be used
///
/// \param scaling float about the 16:9 scaling (base: 1920x1080) to be applied. (OPTIONAL!)
///
/// \return sfml text
////////////////////////////////////////////////////////////
void drawMenuPopUp(menuPopUps PopUp, sf::RenderWindow *window, sf::Text *warningMessage);

////////////////////////////////////////////////////////////
/// \brief handle user event input for different messages
///
/// \param menuPopUp pointer menu popup
///
/// \param ev current sfml events
////////////////////////////////////////////////////////////
void inputErrorPopUp(menuPopUps &menuPopUp, sf::Event *ev);


void escapeKeyPressed(bool *activeText, std::vector<button *> *menuButtons, mainMenuLayerId *currentLayer, gameMode mode);
mainMenuLayerId getPreviousLayer(mainMenuLayerId currentLayer, gameMode mode);
void enterKeyPressed(bool *activeText, std::vector<button *> *menuButtons, mainMenuLayerId *currentLayer, menuPopUps *menuWarning, Application *appPointer);
bool createSave(std::string newSaveName, menuPopUps *menuWarning, Application *appPointer);
void deleteKeyPressed(bool *activeText, std::vector<button *> *menuButtons, mainMenuLayerId currentLayer, Application *appPointer);
void textEntered(bool *activeText, std::vector<button *> *menuButtons, mainMenuLayerId currentLayer, Application *appPointer, sf::Event ev, gameMode mode);
bool validCharacterInput(mainMenuLayerId activeLayer, sf::Uint32 c, int activeLength, gameMode mode, int n);
void mouseButtonPressed(bool *activeText, std::vector<button *> *menuButtons, mainMenuLayerId *currentLayer, Application *appPointer, gameMode mode);
void setActiveSafe(std::string saveName, std::string *pathToBeLoaded, std::vector<gameSaveSummary> availableSaveFiles);
void getMenuPicks(std::vector<button *> *menuButtons, mainMenuLayerId currentLayer, gameMode *mode);


void setMenuPopUpMessage(void);


#endif //MENU_HPP
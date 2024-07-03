#pragma once
#ifndef MENUBUTTONS_HPP
#define MENUBUTTONS_HPP

// Include standard libraries
#include <iostream>

// Include SFML libraries
#include <SFML/Graphics.hpp>

// Include project libraries
#include "utils.hpp"


/// @brief that denote the layer of the main menu
enum mainMenuLayerId{
    leave,          //!< exit the game
    base,           //!< main main menu
    modeSelection,  //!< choose between singleplayer, hosting or joining
    hosting,        //!< hosting or singleplayer, layer in which one chooses a save file
    joining,        //!< joining, layer in which one enters host ip and port
    options,        //!< options, LAYER NOT IMPLEMENTED YET
    game            //!< ending the menu, start the game
};

/// @brief Base class of a text button, template not intended to be used
class button{
    public:
    /**
     * @brief Construct a new button object
     * @param i index of the button in its own layer (determines the y position)
     * @param tex std::string text to be displayed
     * @param currentLayer Layer the button is in
     * @param followLayer Layer the button leads to if used
     * @param pos   float[2] x,y CENTER position. is converted to top left corner using the actualy size of the text
     * @param font  sf::Font font to be used
     */
    button(short i, std::string tex, mainMenuLayerId currentLayer, mainMenuLayerId followLayer,  float pos[2], sf::Font &font);
    

    /**
     * @brief draws the button on the window
     * @param window sf::renderWindow window to draw on
     */
    void draw(sf::RenderWindow &window);


    /**
     * @brief Update the focuse state of the button/ may be overriden by child classes
     * @param mousePos x,y position of the cursor tip
     */
    virtual void update(sf::Vector2f mousePos);
    virtual ~button();

    mainMenuLayerId layer;      //!< layer the button is in
    mainMenuLayerId layer_next; //!< layer the button leads to
    std::string stringText;     //!< text to be displayed
    sf::Text text;              //!< text object
    bool focus;                 //!< true if the cursor hovering over the button         
    float position[2];          //!< x,y position of the top left corner of the button
    short idx;                  //!< index of the button in its layer
};


/// @brief function identifier of clickable buttons
enum clickbuttonTypes {
    standart,    //!< button only leads to a new layer
    graphic,     //!< button leads to a new layer and changes the graphic
    saveFile    //!< choses a save file
};

/// @brief child class of a clickable button
class ClickButton : public button {
    
    public:
    /**
     * @brief Construct a new Click Button object
     * @param i             index of the button in its own layer (determines the y position)
     * @param tex           std::string text to be displayed
     * @param currentLayer  Layer the button is in
     * @param followLayer   Layer the button leads to if used
     * @param pos           float[2] x,y CENTER position. is converted to top left corner using the actualy size of the text
     * @param font          sf::Font font to be used
     * @param type          clickbuttonTypes type of the button
     */
    ClickButton(short i, std::string tex, mainMenuLayerId currentLayer, mainMenuLayerId followLayer,  float pos[2], sf::Font &font, clickbuttonTypes type);
    
    /**
     * @brief Calls the parents update function
     * @param mousePos x,y position of the cursor tip
     */
    void update(sf::Vector2f mousePos);

    clickbuttonTypes type;  //!< type of the button

    private:
    ~ClickButton(void);
};


// TEXT BUTTONS that allow text input


/// @brief types of textinput buttons
enum textbuttonTypes{
    adress,     //!< button accepts the ip adress or port
    newSafe     //!< button accepts name of a new save file
};


/**
 * @brief Text Button that allows text input
 */
class textButton : public button{
    public:
    /**
     * @brief Construct a new text Button object
     * @param i             index of the button in its own layer (determines the y position)
     * @param tex           std::string text to be displayed
     * @param currentLayer  Layer the button is in
     * @param followLayer   Layer the button leads to if used
     * @param pos           float[2] x,y CENTER position. is converted to top left corner using the actualy size of the text
     * @param font          sf::Font font to be used
     * @param type          textbuttonTypes type of the button
     */
    textButton(short i, std::string tex, mainMenuLayerId currentLayer, mainMenuLayerId followLayer,  float pos[2], sf::Font &font, textbuttonTypes type);
    
    /**
     * @brief Either calls the parents update function or changes the color of the text if text input is active
     * @param mousePos 
     */
    void update(sf::Vector2f mousePos) override;

    /**
     * @brief Add a letter or number to the input string
     * @param input     sf::Uint32 input
     * @param res       sf::Vector2u resolution of the window
     */
    void addInput(sf::Uint32 input, sf::Vector2u res);

    /**
     * @brief Removes the last letter or number from the input string
     * @param res      sf::Vector2u resolution of the window
     */
    void delLastInput(sf::Vector2u res);

    /**
     * @brief Resets the input string to its original text and disables text input
     */
    void resetInput(void);

    textbuttonTypes type;           //!< type of the button
    unsigned short maxChars;        //!< maximum number of characters that can be input
    std::string userText = "";      //!< input string
    float ogPosition[2];            //!< original position of the text
    bool activeInput = false;       //!< true if text input is active
    
    private:
    ~textButton(void);
};

#endif
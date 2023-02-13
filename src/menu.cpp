////GAME
//Standart libs
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <thread>
#include <random>
#include <vector>
#include <time.h>
//External libs
#include "nlohmann/json.hpp"
#include <raylib.h>

//Own functions
#include "declr.h"

//Template for a display class. Actually used for the background and mouse
class displayImage {
    public:
        int textureIdx; //Index for the appropriate texture to draw
        Rectangle rec;  //contains size[width, height] and position[x,y]

        displayImage (int img, int size[], int pos[]){
            textureIdx = img;
            rec.width = size[0];
            rec.height = size[1];
            rec.x = pos[0];
            rec.y = pos[1];
        }

        //Given the texture list, draw the texture
        void draw(Texture2D textureList[]){
            DrawTexture(textureList[textureIdx], rec.x, rec.y, WHITE);
        }

        //Adjust size
        void changeSize (int s[2]){
            rec.width = s[0];
            rec.height = s[1];
        }

        //Adjust position (template. in menu only used for the mouse)
        void rePosition(int newX, int newY){
            rec.x = newX;
            rec.y = newY;
        }
};

class defaultButton {
    public:
        Rectangle rec;                      //Contins size[width, height] and x,y position of upper left corner
        int textPosition[2];                //screen position of the upper left corner
        int buttonType;                     //button type. 0:defaultButton, 1:create Button, 2: choose button, 3: optionsButton, 4:connectionButton
        bool focus;                         //true, if MouseRec colllides with button
        int currentLayer;                   //Assigned Layer
        int buttIdx;                        //First, second, third or fourth button? starts at 1!
        int followUpLayer;                  //Which layer does the button lead to
        std::string buttonText;             //Text displayed on Button
        int buttonTextLength;               //width of Button Text
        int textureIdx[2];                  //Index for the appropriate texture to draw
        bool directFollowOnClick = true;    //if true, then clicking will immideatly lead to the next layer
        bool inUse = false;                 //inUse, if directFollowOnClick is false, then the button can be used
        
        //Create a default button
        defaultButton(int spriteIdx[], int size[], std::string text, int lay, int pos, int maxPos, int funcIdx, int type){
            //Save general information
            buttonType = type;
            currentLayer = lay;
            focus = false;
            buttIdx = pos;
            buttonText = text;
            followUpLayer = funcIdx;

            //Save texture information, size, position, index in texturelist 
            textureIdx[0] = spriteIdx[1];
            textureIdx[1] = spriteIdx[0];
            rec.height = size[1];
            rec.width = size[0]; 
            rec.x = (gV::screenWidth / 2) - (rec.width / 2);
            rec.y = (((gV::screenHeight / (maxPos + 1)) * pos) - (rec.height / 2));

            //Set position and size of button text;
            buttonTextLength = MeasureText(buttonText.c_str(), 30 * gV::screenRatio);
            textPosition[0] = (gV::screenWidth / 2) - (buttonTextLength / 2);
            textPosition[1] = ((gV::screenHeight / (maxPos + 1)) * pos) - ((30 * gV::screenRatio) / 2);
        }

        //Draw button texture and text. Texture drawn depends whether the button is in focus
        virtual void draw(Texture2D textureList[]){
            if (focus == true){
                DrawTexture(textureList[textureIdx[0]], rec.x, rec.y, WHITE);
            } else {
                DrawTexture(textureList[textureIdx[1]], rec.x, rec.y, WHITE);
            }
            DrawText(buttonText.c_str(), textPosition[0], textPosition[1], 30 * gV::screenRatio, BLACK);
        }

        //Button Click
        virtual void click(void){
            return;
        }

        //General Update function.
        //Checks whether the mouse is on the button and will set it as "focus"ed
        virtual void update(Rectangle mouseRec){
            if (CheckCollisionRecs(mouseRec, rec) == true){
                focus = true;
            } else {
                focus = false;
            }
        }

        //in case of resolution or fullscreen changes
        virtual void updateSize(void){
            return;
        }
        //In response to grafic changes or addition/ removal of buttons in layer
        virtual void updatePosition(int maxButt){
            return;
        }
};

class createButton : public defaultButton {
    public:
        int maxInputChars = 16;     //Maximum allowed number of characters!
        char input[17] = "";        //Input of the user.
        std::string ogTextCopy;     //Copy of the original text to switch around between displayed texts
        int numberInputChars = 0;   //Number of chars entered by the user
        bool available = true;      //Checks whether it should be shown/updated/used!
        createButton(int spriteIdx[], int size[], std::string text, int lay, int pos, int maxPos, int funcIdx, int type) : defaultButton(spriteIdx, size, text, lay, pos, maxPos, funcIdx, type){
            ogTextCopy = buttonText;
            directFollowOnClick = false;
            if (pos >= 5){
                available = false;
            }
        }
        void draw(Texture2D textureList[]) override {
            if(available){
                if (focus == true){
                    DrawTexture(textureList[textureIdx[0]], rec.x, rec.y, WHITE);
                } else {
                    DrawTexture(textureList[textureIdx[1]], rec.x, rec.y, WHITE);
                }
                DrawText(buttonText.c_str(), textPosition[0], textPosition[1], 30 * gV::screenRatio, BLACK);
            }
        }

        //Change what to display on click
        void click(void) override {
            if (inUse == false && available){
                inUse = true;
                buttonText = input;
                updateHorizontalTextPosition();
            }
        }

        //add a char to the user input
        void addInputChar (int inputKey){
            if ((numberInputChars <= maxInputChars)){
                if ((inputKey >= 97 && inputKey <= 122)|| (inputKey >= 65 && inputKey <= 90) || (inputKey >=48 && inputKey <= 57)){
                    input[numberInputChars] = (char)inputKey;
                    input[numberInputChars + 1] = '\0';
                    numberInputChars += 1;
                    buttonText = input;
                    updateHorizontalTextPosition();
                }
            }
            
        }

        //Update the text position
        void updateHorizontalTextPosition(void){
            buttonTextLength = MeasureText(buttonText.c_str(), 30 * gV::screenRatio);
            textPosition[0] = (gV::screenWidth / 2) - (buttonTextLength / 2);
        }

        //confirm the current input/ return the input. End user input loop
        std::string confirmInput(void){
            inUse = false;
            return buttonText;
        }

        //Removes the latest char of the input.
        void removeInputChar(void){
            numberInputChars -= 1;
            if (numberInputChars < 0){
                numberInputChars = 0;
            } 
            input[numberInputChars] = '\0';
            buttonText = input;
            updateHorizontalTextPosition();
        }

        //reset user input and display the original button text
        void cancelInput(void){
            inUse = false;
            input[0] = '\0';
            numberInputChars = 0;
            buttonText = ogTextCopy;
            updateHorizontalTextPosition();
        }

        //Update Vertical position!
        void updatePosition(int maxButts) override{
            if (maxButts >= 5){
                available = false;
            } else {
                available = true;
                rec.y = (((gV::screenHeight / (maxButts + 1)) * buttIdx) - (rec.height / 2));
                textPosition[1] = ((gV::screenHeight / (maxButts + 1)) * buttIdx) - ((30 * gV::screenRatio) / 2);
            }
        }
};

class choiceButton : public defaultButton {
    using defaultButton::defaultButton;
    public:
    bool deleting = false;
    void updatePosition (int maxButts) override {
        if (maxButts >= 5){
            rec.y = (((gV::screenHeight / (5)) * (buttIdx - 1)) - (rec.height / 2));
            textPosition[1] = ((gV::screenHeight / (5)) * (buttIdx - 1)) - ((30 * gV::screenRatio) / 2);
        } else {
            rec.y = (((gV::screenHeight / (maxButts + 1)) * buttIdx) - (rec.height / 2));
            textPosition[1] = ((gV::screenHeight / (maxButts + 1)) * buttIdx) - ((30 * gV::screenRatio) / 2);
        }
    }

    void draw(Texture2D textureList[]) override {
        if (focus == true){
            DrawTexture(textureList[textureIdx[0]], rec.x, rec.y, WHITE);
        } else {
            DrawTexture(textureList[textureIdx[1]], rec.x, rec.y, WHITE);
        }
        DrawText(buttonText.c_str(), textPosition[0], textPosition[1], 30 * gV::screenRatio, BLACK);
        if (deleting){
            DrawLine(rec.x, rec.y, rec.x + rec.width, rec.y + rec.height, RED);
            DrawLine(rec.x, rec.y + rec.height, rec.x + rec.width, rec.y, RED);
        }
    }

    void click(void) override {
        std::cout << "Picked Profile: " << buttonText << std::endl;
    }
};

//Same as default. But changes window and overall screen information. 
//Can it take all textures and resize them?
class optionsButton : public defaultButton{
    using defaultButton::defaultButton;
    void click(void) override {
        std::cout << "Now it should change into " << buttonText << std::endl;
        return;
    }
};

//2 connectionButton's needed. To ask for host ip and host port number
class connectionButton : public defaultButton{
    public:
        char input[16] = "";     //User Input text.
        std::string ogTextCopy;     //Copy of the original text to switch around between displayed texts
        int numberInputChars = 0;   //Number of chars inputed for this field.
        int maxInputChars;
        
        connectionButton(int spriteIdx[], int size[], std::string text, int lay, int pos, int maxPos, int funcIdx, int type) : defaultButton(spriteIdx, size, text, lay, pos, maxPos, funcIdx, type){
            ogTextCopy = buttonText;
            directFollowOnClick = false;
            if (pos == 1){
                maxInputChars = 16;
            } else {
                maxInputChars = 6;
            }
        }

        //Change what to display on click
        void click(void) override {
            if (inUse == false){
                inUse = true;
                buttonText = input;
                updateHorizontalTextPosition();
            }
        }

        //add a char to the user input
        void addInputChar (int inputKey){
            if ((numberInputChars <= maxInputChars)){
                if ((buttIdx == 1) && (inputKey == 46 || (inputKey >=48 && inputKey <= 57))){
                    input[numberInputChars] = (char)inputKey;
                    input[numberInputChars + 1] = '\0';
                    numberInputChars += 1;
                } else if ((buttIdx == 2) && (inputKey >=48 && inputKey <= 57)){
                    input[numberInputChars] = (char)inputKey;
                    input[numberInputChars + 1] = '\0';
                    numberInputChars += 1;
                }
                buttonText = input;
            }
            updateHorizontalTextPosition();
        }

        //Update the text position
        void updateHorizontalTextPosition(void){
            buttonTextLength = MeasureText(buttonText.c_str(), 30 * gV::screenRatio);
            textPosition[0] = (gV::screenWidth / 2) - (buttonTextLength / 2);
        }

        //confirm the current input/ return the input

        std::string confirmInput(void){
            inUse = false;
            return buttonText;
        }

        //Removes the latest char of the input.
        void removeInputChar(void){
            numberInputChars -= 1;
            if (numberInputChars < 0){
                numberInputChars = 0;
            } 
            input[numberInputChars] = '\0';
            buttonText = input;
            updateHorizontalTextPosition();
        }

        //reset input and display the default text
        void cancelInput(void){
            if (inUse){
                inUse = false;
                input[0] = '\0';
                numberInputChars = 0;
                buttonText = ogTextCopy;
                updateHorizontalTextPosition();
            }
        }

};



int run_Menu(Image imageRefList[NUMBER_OF_DIFFERENT_MENU_TEXTURES], nlohmann::json configFile){

    //Textures in the menu include: {(0, Background);(1, Cursor); (2, Button_default); (3, Button_inUse); (4, World_default); (5, World_default-_inUse)}
    int textureSizes [NUMBER_OF_DIFFERENT_MENU_TEXTURES][2] = {{gV::screenWidth, gV::screenHeight}, {50, 50}, {300, 100}, {300, 100}, {300, 100}, {300, 100}};
    Texture2D scaledTextures[NUMBER_OF_DIFFERENT_MENU_TEXTURES];

    //Scale Textures
    for (int m = 0; m < NUMBER_OF_DIFFERENT_MENU_TEXTURES; m++){
        ImageResize(&imageRefList[m], textureSizes[m][0] * gV::screenRatio, textureSizes[m][1]* gV::screenRatio);
        scaledTextures[m] = LoadTextureFromImage(imageRefList[m]);
        std::cout << "scaled and loaded textures: " << m << std::endl;
    }

    //Info for non-default buttons (available profile and world buttons)
    int nWorldsAvailable;
    int nProfilesAvailable = configFile["nprofiles"];
    bool maxProfiles = false;
    bool maxWorlds = false;
    
    //
    std::vector<std::string> availableProfileNames;
    std::vector<std::string> availableProfileKeys;
    std::vector<std::string> availableWorlds;

    //Position for background and cursor
    int bgPos[2] = {0, 0}; int mousePos[2] = {GetMouseX(), GetMouseY()};

    //Create class instances
    

    //Background and Cursor
    displayImage BackgroundImage(0, textureSizes[0],bgPos);
    displayImage Cursor(1, textureSizes[1], mousePos);
    std::cout << "Background and Cursor created\n";
    

    //information to create 13 buttons
    std::vector<defaultButton*> Buttons;
    int nBUTTONS = 13;
    int buttonTypes[] =     {1, 0, 0, 0, 3, 3, 3, 3, 0, 0, 1, 4, 4};
    int spriteIdxs[2] =     {2, 3};
    std::string displayTexts[] = {"New Profile", "Play", "Options", "Exit", "Toggle Fullscreen", "High Res", "Mid Res", "Low Res", "Host", "Join", "New World", "Enter Host Ip", "Enter Host Port"};
    int layers[] =          {0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 4, 5, 5};
    int positions[] =       {1, 1, 2, 3, 1, 2, 3, 4, 1, 2, 1, 1, 2};
    int maxPositions[] =    {nProfilesAvailable + 1, 3, 3, 3, 4, 4, 4, 4, 2, 2, 4, 2, 2};
    int functionalIdxs[]=   {1, 3, 2, -1, 2, 2, 2, 2, 4, 5, 4, 5, 5};
    
    //
    for (int i = 0; i < nBUTTONS; i++){
        if (buttonTypes[i] == 1){
            Buttons.push_back(new createButton(spriteIdxs, textureSizes[3], displayTexts[i], layers[i], positions[i], maxPositions[i], functionalIdxs[i], buttonTypes[i]));
        } else if (buttonTypes[i] == 0){
            Buttons.push_back(new defaultButton(spriteIdxs, textureSizes[3], displayTexts[i], layers[i], positions[i], maxPositions[i], functionalIdxs[i], buttonTypes[i]));
        } else if (buttonTypes[i] == 3){
            Buttons.push_back(new optionsButton(spriteIdxs, textureSizes[3], displayTexts[i], layers[i], positions[i], maxPositions[i], functionalIdxs[i], buttonTypes[i]));
        } else if (buttonTypes[i] == 4){
            Buttons.push_back(new connectionButton(spriteIdxs, textureSizes[3], displayTexts[i], layers[i], positions[i], maxPositions[i], functionalIdxs[i], buttonTypes[i]));
        }
    }
    //Load in available profiles and create profile buttons
    for (int i = 0; i < nProfilesAvailable; i++){
        std::cout << "Create Profile Button for: " << configFile["profiles"][i]["name"] << std::endl;
        availableProfileNames.push_back(configFile["profiles"][i]["name"]);
        availableProfileKeys.push_back(configFile["profiles"][i]["key"]);
        Buttons.push_back(new choiceButton(spriteIdxs, textureSizes[3], availableProfileNames[i], 0, i + 2, nProfilesAvailable + 1, 1, 2));
    }
    for (auto &iterator : Buttons){
        if (iterator->currentLayer == 0){
            iterator->updatePosition(nProfilesAvailable + 1);
        }
    }

    //Navigating through the menu
    int Current_Layer = 0;
    bool activeUserInput = false;
    bool MenuRunning = true;

    //MENU LOOP
    while (MenuRunning) {
        //FIRST DRAW
        BeginDrawing();
            ClearBackground(GRAY);
            BackgroundImage.draw(scaledTextures);
            
            //Draw appropriate Layer
            for (auto &iterator : Buttons){
                if (iterator->currentLayer == Current_Layer){
                    //In the first and forth layer. DONT draw creation buttons if the max is full!
                    if ((Current_Layer == 0 && maxProfiles == true) || (Current_Layer == 4 && maxWorlds == true)){
                        choiceButton * choB = dynamic_cast<choiceButton *>(iterator);
                        if (choB){
                            choB->draw(scaledTextures);
                        }
                    //otherwise draw everything
                    } else {
                        iterator->draw(scaledTextures);
                    }
                }
            }
            
            //Draw Cursor
            if (activeUserInput == false){
                Cursor.draw(scaledTextures);
            }
            
            //DELETE LATER!!
            //helpful tools. fps and two lines in the middle of the screen
            DrawFPS(0,0);
            DrawLine(0, gV::screenHeight/2, gV::screenWidth, gV::screenHeight/2, ORANGE);
            DrawLine(gV::screenWidth/2, 0, gV::screenWidth/2, gV::screenHeight, ORANGE);
            //END DELETION!!

        EndDrawing();

        //MANAGE INPUT
        //Update Mouse Position
        if (activeUserInput == false){
            mousePos[0] = GetMouseX();
            mousePos[1] = GetMouseY();
            Cursor.rePosition(mousePos[0], mousePos[1]);
        }
        

        //Update all the buttons of the current Layer
        for (auto &iterator : Buttons){
            if (iterator->currentLayer == Current_Layer){
                iterator->update(Cursor.rec);
            }
        }

        //Handle Mouse Input seperate!
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if (activeUserInput == false){
                for (auto &iterator : Buttons){
                    //check if the layer and mouse focus fit
                    if (iterator->currentLayer == Current_Layer && iterator->focus == true){
                        //if this is a one click direct move button. change layer!
                        if (iterator->directFollowOnClick == true){
                            if (Current_Layer == 1){
                                gV::activeProfileName = iterator->buttonText;
                                //ADD THE KEY!!!!!

                                //LOAD WORLDS!!!

                                //Update Layer 4
                                for (auto &subiterator : Buttons){
                                    //Find worlds. load a button for all worlds found!!
                                }

                            }
                            Current_Layer = iterator->followUpLayer;
                            iterator->click();
                            break;
                        //Otherwise stop mouse activity!
                        }else {
                            createButton * creB = dynamic_cast<createButton *>(iterator);
                            if (!creB){
                                iterator->click();
                            } else if (creB->available){
                                activeUserInput = true;
                                iterator->click();
                            }
                        }
                    break;
                    }
                }
            }
        }

        
        //What if the escape button is pressed
        if(IsKeyPressed(KEY_ESCAPE)){
            if (activeUserInput){
                for (auto &iterator : Buttons){
                    if (iterator->inUse){
                        connectionButton * conB = dynamic_cast<connectionButton *>(iterator);
                        if (conB){
                            conB->cancelInput();
                            activeUserInput = false;
                            break;
                        }
                        createButton *creB = dynamic_cast<createButton *>(iterator);
                        if (creB){
                            creB->cancelInput();
                            activeUserInput = false;
                            break;
                        }
                    }
                }
            } else {
                if (Current_Layer == 3 || Current_Layer == 5){
                    Current_Layer -= 2;
                } else {
                    Current_Layer -= 1;
                }
                //lowest layers is supposed to exit? leave application!
                if (Current_Layer < 0){
                    MenuRunning = false;
                }
            }
        } 

        //What if Enter is pressed
        if (IsKeyPressed(KEY_ENTER)){
            if (activeUserInput){
                for (auto &iterator : Buttons){
                    if (iterator->inUse){

                        //entering a host ip/port to join as a client!
                        connectionButton * conB = dynamic_cast<connectionButton *>(iterator);
                        if (conB){
                            if (iterator->buttIdx == 1){
                                gV::host_Ip_Adress = conB->confirmInput();
                                std::cout << "Entered Host Ip: " << gV::host_Ip_Adress << std::endl;
                            } else {
                                gV::host_Port = std::atoi(conB->confirmInput().c_str());
                                std::cout << "Entered Host Port: " << gV::host_Port << std::endl;
                            }
                            activeUserInput = false;
                            break;
                        }
                        //creating a new profile/world
                        createButton *creB = dynamic_cast<createButton *>(iterator);
                        if (creB){
                            //create a new profile entry!, button!, look for associated worlds! AND update positions in layer 0 and 4!
                            if (Current_Layer == 0){
                                //save name, generate key
                                gV::activeProfileName = creB->confirmInput();
                                creB->cancelInput(); //Not really but will reset the text
                                std::string generatedProfileKey = generateString();
                                while (findStringIndex(generatedProfileKey, availableProfileKeys) != -1){
                                    generatedProfileKey = generateString();
                                }
                                //save and add to the available ones
                                addProfileToConfig(configFile, generatedProfileKey, gV::activeProfileName);
                                availableProfileKeys.push_back(generatedProfileKey);
                                availableProfileNames.push_back(gV::activeProfileName);
                                nProfilesAvailable += 1;
                                //updatePositions!
                                if (nProfilesAvailable >= 5){
                                    maxProfiles = true;
                                    Buttons.push_back(new choiceButton(spriteIdxs, textureSizes[3], gV::activeProfileName, 0, 5, 5, 1, 2));
                                } else {
                                    Buttons.push_back(new choiceButton(spriteIdxs, textureSizes[3], gV::activeProfileName, 0, nProfilesAvailable + 1, nProfilesAvailable + 1, 1, 2));
                                }
                                //Create new button and update positions
                                for (auto &iterator : Buttons){
                                    if (Current_Layer == iterator->currentLayer){
                                        iterator->updatePosition(nProfilesAvailable + 1);
                                    }
                                }

                                //Confirm and move on to next layer
                                std::cout << "Added Profile: " << gV::activeProfileName << ". With key : " << generatedProfileKey << std::endl;
                                Current_Layer = 1;

                            //Create a new world file and just positions in layer 4!
                            } else if (Current_Layer == 4){
                                gV::hosting_chosen_world = creB->confirmInput();
                                std::cout << "Added World: " << gV::hosting_chosen_world << std::endl;
                            }
                            activeUserInput = false;
                            break;
                        }
                    }
                }
            }
        }


        //What if backspace or delete have been pressed
        if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_DELETE))){
            if (activeUserInput){
                for (auto &iterator : Buttons){
                    if (iterator->inUse){
                        //Removes the last char of an input
                        connectionButton * conB = dynamic_cast<connectionButton *>(iterator);
                        if (conB){
                            conB->removeInputChar();
                            break;
                        }
                        createButton *creB = dynamic_cast<createButton *>(iterator);
                        if (creB){
                            creB->removeInputChar();
                            break;
                        }
                        //Gonna Delete a Profile!!
                        choiceButton * choB = dynamic_cast<choiceButton *>(iterator);
                        if (choB){

                            break;
                        }
                    }
                }
            }
        } else {
            if (activeUserInput){
                for (auto &iterator : Buttons){
                    if (iterator->inUse){
                        //Adding chars as user input
                        connectionButton * conB = dynamic_cast<connectionButton *>(iterator);
                        if (conB){
                            int inputKey = GetCharPressed();
                            while (inputKey != 0){
                                conB->addInputChar(inputKey);
                                inputKey = GetCharPressed();
                            }
                            break;
                        }
                        createButton *  creB = dynamic_cast<createButton *>(iterator);
                        if (creB){
                            int inputKey = GetCharPressed();
                            while (inputKey != 0){
                                creB->addInputChar(inputKey);
                                inputKey = GetCharPressed();
                            }
                            break;
                        }
                    }
                }
            }
        }

       
        //GENERAL CHECKS OF THE MENUSTAE
        //Keep track whether the user wants to host or not
        if (Current_Layer == 4 && (gV::wantsHost == false)){
            gV::wantsHost = true;
        } else if (Current_Layer == 5 && (gV::wantsHost == true)){
            gV::wantsHost = false;
        } else if (Current_Layer == 3){
            gV::host_Ip_Adress = "";
            gV::host_Port = -1;
            gV::hosting_chosen_world = "";
        } else if (Current_Layer == -1){
            MenuRunning = false;
        }

        //EITHER, the user has entered a port and ip to join as a client, or has chosen a world and is hosting
        if ((gV::wantsHost == false && Current_Layer == 5 && gV::host_Port != -1 && gV::host_Ip_Adress != "") || (gV::wantsHost == true && Current_Layer == 4 && gV::hosting_chosen_world != "")){
            //Chill all buttons!
            for (auto & iterator : Buttons){
                iterator->inUse = false;
            }
            MenuRunning = false;
        }

        if (!IsWindowFocused()){
            MinimizeWindow();
        }

        
    }

    //Unload Texture.    
    for (int i = 0; i < NUMBER_OF_DIFFERENT_MENU_TEXTURES; i++){
        UnloadTexture(scaledTextures[i]);
    }

    //Either we ended the reduced the layer enough to leave
    if (Current_Layer == -1) {
        return -1;
    //Or we are ready to play fix return to value to 1 to start loading, setting up server and stuff!
    } else {
        return -1;
    }
}



bool checkFile (void){
    return true;
}
////GAME
//Standart libs
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <thread>
#include <random>
#include <vector>

//External libs
#include "nlohmann/json.hpp"
#include <raylib.h>

//Own functions

#include "declr.h"

//Default Parent Class. Used only for background and the cursor
class displayImage {
    public:
        int textureIdx;
        Rectangle rec;
        //Constructor
        displayImage (int img, int size[], int pos[]){
            textureIdx = img;
            rec.width = size[0];
            rec.height = size[1];
            rec.x = pos[0];
            rec.y = pos[1];
        }
        //Draw this sprite
        void draw(Texture2D textureList[]){
            DrawTexture(textureList[textureIdx], rec.x, rec.y, WHITE);
        }

        //In case of size adjustments
        void changeSize (int s[2]){
            rec.width = s[0];
            rec.height = s[1];
        }

        //Reposition (only for mouse)
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
        int buttIdx;                        //First, second, third or fourth button?
        int followUpLayer;                  //Where does the button lead?
        std::string buttonText;             //Text on Button
        int buttonTextLength;               //width of Button Text
        int textureIdx[2];                  //saves the indexes of textures to be drawn for this button
        bool directFollowOnClick = true;    //if true, then clicking will immideatly follow to the next layer
        bool inUse = false;                 //inUse, if next layers does not follow on click, the button is "inUse"
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

        //Draw appropriate button texture and add text to it
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

        //Check collision with mouse
        virtual void update(Rectangle mouseRec){
            if (CheckCollisionRecs(mouseRec, rec) == true){
                focus = true;
            } else {
                focus = false;
            }
        }

        virtual void updatePosition(void){
            
        }
};

class createButton : public defaultButton {

};

class choiceButton : public defaultButton {

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
        std::string input = "";     //User Input text.
        std::string ogTextCopy;     //Copy of the original text to switch around between displayed texts
        int numberInputChars = 0;   //Number of chars inputed for this field.
        int maxInputChars;
        
        connectionButton(int spriteIdx[], int size[], std::string text, int lay, int pos, int maxPos, int funcIdx, int type) : defaultButton(spriteIdx, size, text, lay, pos, maxPos, funcIdx, type){
            ogTextCopy = buttonText;
            directFollowOnClick = false;
            if (pos == 1){
                maxInputChars = 13;
            } else {
                maxInputChars = 5;
            }
        }

        void click(void) override {
            if (inUse == false){
                inUse = true;
                buttonText = input;
                updateHorizontalTextPosition();
            }
        }

        //add a char to the user input
        void addInputChar (int inputKey){
            char inputCharStar[2];
            inputCharStar[0] = (char)inputKey;
            inputCharStar[1] = '\0';
            if ((inputKey >= 32) && (inputKey <= 125) && (numberInputChars <= maxInputChars)){
                input.append(inputCharStar); 
            }
            updateHorizontalTextPosition();
        }

        //Update the text position
        void updateHorizontalTextPosition(void){
            buttonTextLength = MeasureText(buttonText.c_str(), 30 * gV::screenRatio);
            textPosition[0] = (gV::screenWidth / 2) - (buttonTextLength / 2);
        }

        void cancelInput(void){
            if (inUse){
                inUse = false;
                input = "";
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




    //Info for non-default buttons (profile and world buttons)
    std::string *availableProfiles = (std::string *) malloc(sizeof(std::string) * (int) configFile["nprofiles"]);
    std::string *availableProfileKeys = (std::string *) malloc(sizeof(std::string) * (int) configFile["nprofiles"]);
    std::string *availableWorlds = nullptr;


    int bgPos[2] = {0, 0};
    int mousePos[2] = {GetMouseX(), GetMouseY()};
    //Load in available profiles
    for (int i = 0; i < configFile["nprofiles"]; i++){
        std::cout << "Found Profile: " << configFile["profiles"][i]["name"] << std::endl;
        availableProfiles[i] = configFile["profiles"][i]["name"];
        availableProfileKeys[i] = configFile["profiles"][i]["key"];
    }

    //Create class instances
    //Background and Cursor
    displayImage BackgroundImage(0, textureSizes[0],bgPos);
    displayImage Cursor(1, textureSizes[1], mousePos);
    std::cout << "Background and Cursor created\n";
    

    //create all buttons (except profile and worlds buttons!)
    std::vector<defaultButton*> Buttons;
    int nBUTTONS = 13;
    int buttonTypes[] =     {1, 0, 0, 0, 3, 3, 3, 3, 0, 0, 1, 4, 4};
    int spriteIdxs[2] =     {2, 3};
    std::string displayTexts[] = {"New Profile", "Play", "Options", "Exit", "Toggle Fullscreen", "High Res", "Mid Res", "Low Res", "Host", "Join", "New World", "Enter Host Ip", "Enter Host Port"};
    int layers[] =          {0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 4, 5, 5};
    int positions[] =       {1, 1, 2, 3, 1, 2, 3, 4, 1, 2, 1, 1, 2};
    int maxPositions[] =    {4, 3, 3, 3, 4, 4, 4, 4, 2, 2, 4, 2, 2};
    int functionalIdxs[]=   {1, 3, 2, -1, 2, 2, 2, 2, 4, 5, 4, 5, 5};//NOTE: needs to be adapted eventually!!. Returns the following layer
    
    for (int i = 0; i < nBUTTONS; i++){
        switch (buttonTypes[i])
        {
        case 1: std::cout << "yeee" << std::endl;// Buttons.push_back(new createButton(spriteIdxs, textureSizes[3], displayTexts[i], layers[i], positions[i], maxPositions[i], functionalIdxs[i]));

        case 0: Buttons.push_back(new defaultButton(spriteIdxs, textureSizes[3], displayTexts[i], layers[i], positions[i], maxPositions[i], functionalIdxs[i], buttonTypes[i]));

        case 3: Buttons.push_back(new optionsButton(spriteIdxs, textureSizes[3], displayTexts[i], layers[i], positions[i], maxPositions[i], functionalIdxs[i], buttonTypes[i]));

        case 4: Buttons.push_back(new connectionButton(spriteIdxs, textureSizes[3], displayTexts[i], layers[i], positions[i], maxPositions[i], functionalIdxs[i], buttonTypes[i]));

        }
    }


    //Navigating through the menu
    int Current_Layer = 1;
    bool activeUserInput = false;
    //MENU LOOP
    while (Current_Layer >= 0 && Current_Layer <= 5) {
        //FIRST DRAW
        BeginDrawing();
            ClearBackground(GRAY);
            BackgroundImage.draw(scaledTextures);
            
            //Draw appropriate Layer
            for (auto &iterator : Buttons){
                if (iterator->currentLayer == Current_Layer){
                    iterator->draw(scaledTextures);
                }
            }
            
            if (activeUserInput == false){
                Cursor.draw(scaledTextures);
            }
            
            //Draw helpful tools. fps and two lines in the middle of the screen
            DrawFPS(0,0);
            DrawLine(0, gV::screenHeight/2, gV::screenWidth, gV::screenHeight/2, ORANGE);
            DrawLine(gV::screenWidth/2, 0, gV::screenWidth/2, gV::screenHeight, ORANGE);
            
        EndDrawing();

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
            for (auto &iterator : Buttons){
                if (iterator->currentLayer == Current_Layer && iterator->focus == true){
                    iterator->click();
                    if (iterator->directFollowOnClick){
                        Current_Layer = iterator->followUpLayer;
                    }else {
                        iterator->inUse = true;
                        activeUserInput = true;
                    }
                //prevent multiple button pressing bugs
                break;
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
                    }
                }
            } else {
                if (Current_Layer == 3 || Current_Layer == 5){
                    Current_Layer -= 2;
                } else {
                    Current_Layer -= 1;
                }
            }
            //Jump between menu layers
            
        } 
        //What if Enter is pressed
        if (IsKeyPressed(KEY_ENTER)){

        }
        //What if backspace or delete have been pressed
        if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_DELETE)) && activeUserInput){

        } else {
            if (activeUserInput){
                for (auto &iterator : Buttons){
                    if (iterator->inUse){
                        //Create dynamic cast to all possible buttons. and try each until one works
                        connectionButton * conB = dynamic_cast<connectionButton *>(iterator);
                        if (conB){
                            int inputKey = GetCharPressed();
                            while (inputKey != 0){
                                std::cout << "Entered: " << inputKey << std::endl;
                                conB->addInputChar(inputKey);
                                inputKey = GetCharPressed();
                            }
                            break;
                        }
                        createButton *  creB = dynamic_cast<createButton *>(iterator);
                        if (creB){
                            break;
                        }
                        choiceButton * choB = dynamic_cast<choiceButton *>(iterator);
                        if (choB){
                            break;
                        }
                        
                    }
                    break;
                }
            }
        }
    }

    //Unload Texture.    
    free(availableProfileKeys);
    free(availableProfiles);
    free(availableWorlds);
    for (int i = 0; i < NUMBER_OF_DIFFERENT_MENU_TEXTURES; i++){
        UnloadTexture(scaledTextures[i]);
    }
    //initiate new part of the game loop
    return -1;
}

std::string generateKey(void){
    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, alphabet.length() - 1);
    std::string result;
    for (int i = 0 ; i < 15; i++){
        result += alphabet[distribution(generator)];
    }
    return result;
}

/*
std::string* findWorlds(std::string profKey){
    std::vector<std::string> matchingFiles;
    for (const auto &entry : std::filesystem::recursive_directory_iterator("../world/")){
        if (entry.path().extension() == ".json"){
            std::ifstream file(entry.path());
            nlohmann::json j;
            file >> j;
            if (j["ownerKey"] == profKey){
                matchingFiles.push_back(entry.path().string());
            }
        }
    }
    std::string *array = new std::string[matchingFiles.size()];
    for (std::size_t i = 0; i < matchingFiles.size(); i++){
        array[i] = matchingFiles[i];
    }
    return array;
}
*/



bool checkFile (void){
    return true;
}
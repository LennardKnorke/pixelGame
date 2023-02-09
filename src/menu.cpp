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
        Rectangle rec;
        int textPosition[2];
        bool focus;
        int currentLayer;
        int followUpLayer;
        std::string buttonText;
        int buttonTextLength;
        int textureIdx[2];
        //Create a default button
        defaultButton(int spriteIdx[], int size[], std::string text, int lay, int pos, int maxPos, int funcIdx){
            //Save general information
            currentLayer = lay;
            focus = false;
            buttonText = text;
            followUpLayer = funcIdx;
            //Set a reference to the texture aswell its positions 
            textureIdx[0] = spriteIdx[1];
            textureIdx[1] = spriteIdx[0];
            rec.height = size[1];
            rec.width = size[0]; 
            rec.x = (gV::screenWidth / 2) - (rec.width / 2);
            rec.y = (((gV::screenHeight / (maxPos + 1)) * pos) - (rec.height / 2));
            //Do the same for the displayed text.
            buttonTextLength = MeasureText(buttonText.c_str(), 30 * gV::screenRatio);
            textPosition[0] = (gV::screenWidth / 2) - (buttonTextLength / 2);
            textPosition[1] = ((gV::screenHeight / (maxPos + 1)) * pos) - ((30 * gV::screenRatio) / 2);
        }

        //Draw appropriate button texture and add text to it
        void draw(Texture2D textureList[]){
            if (focus == true){
                DrawTexture(textureList[textureIdx[0]], rec.x, rec.y, WHITE);
            } else {
                DrawTexture(textureList[textureIdx[1]], rec.x, rec.y, WHITE);
            }
            DrawText(buttonText.c_str(), textPosition[0], textPosition[1], 30 * gV::screenRatio, BLACK);
        }

        int click(void){
            return followUpLayer;
        }

        void update(Rectangle mouseRec){
            if (CheckCollisionRecs(mouseRec, rec) == true){
                focus = true;
            } else {
                focus = false;
            }
        }
};

class createButton : public defaultButton {
    public:
        std::string textcpy;
};

class choiceButton : public defaultButton {

};

class optionsButton {
    public:
        Rectangle rec;
        int textPosition[2];
        bool focus;
        int currentLayer;
        int followUpLayer;
        std::string buttonText;
        int buttonTextLength;
        int textureIdx[2];
        optionsButton (int spriteIdx[], int size[], std::string text, int pos){
            //Save general information
            currentLayer = 2;
            focus = false;
            buttonText = text;
            followUpLayer = 2;
            //Set a reference to the texture aswell its positions 
            textureIdx[0] = spriteIdx[1];
            textureIdx[1] = spriteIdx[0];
            rec.height = size[1];
            rec.width = size[0]; 
            rec.x = (gV::screenWidth / 2) - (rec.width / 2);
            rec.y = (((gV::screenHeight / (4)) * pos) - (rec.height / 2));
            //Do the same for the displayed text.
            buttonTextLength = MeasureText(buttonText.c_str(), 30 * gV::screenRatio);
            textPosition[0] = (gV::screenWidth / 2) - (buttonTextLength / 2);
            textPosition[1] = ((gV::screenHeight / (4)) * pos) - ((30 * gV::screenRatio) / 2);
        }

        //Draw appropriate button texture and add text to it
        void draw(Texture2D textureList[]){
            if (focus == true){
                DrawTexture(textureList[textureIdx[0]], rec.x, rec.y, WHITE);
            } else {
                DrawTexture(textureList[textureIdx[1]], rec.x, rec.y, WHITE);
            }
            DrawText(buttonText.c_str(), textPosition[0], textPosition[1], 30 * gV::screenRatio, BLACK);
        }

        int click(void){
            return followUpLayer;
        }

        void update(Rectangle mouseRec){
            if (CheckCollisionRecs(mouseRec, rec) == true){
                focus = true;
            } else {
                focus = false;
            }
        }


};

class connectionButton : public defaultButton{

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
    bool Max_Profiles = false;
    bool Max_Worlds = false;


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
    
    //Create Default Buttons
    std::vector<defaultButton> DefaultButtonList;
    std::string defaultsTexts[5] = {"Play", "Options", "Exit", "Host", "Join"};
    int defaultsLayers[5] = {1, 1, 1, 3, 3};
    int defaultsFuncIdx[5] = {3, 2, -1, 4, 5};
    int defaultsSpriteIdx[2] = {2,3};
    for (int i = 0; i < 5; i++){
        if (defaultsLayers[i] == 1){
            defaultButton button(defaultsSpriteIdx, textureSizes[3], defaultsTexts[i], 1, 1 + i, 3, defaultsFuncIdx[i]);
            DefaultButtonList.push_back(button);
            std::cout << "Button added in Layer: " << 1 << std::endl;

        } else {
            defaultButton button(defaultsSpriteIdx, textureSizes[3], defaultsTexts[i], 3, (1 + i) - 3, 2, defaultsFuncIdx[i]);
            DefaultButtonList.push_back(button);
            std::cout << "Button added in Layer: " << 3 << std::endl;
        }
    }
    std::cout << "Default Buttons created\n";

    //Option Buttons
    std::vector<optionsButton> optionsButtonList;
    std::string optionsText[] = {"High Res", "Mid Res", "Low Res"};
    for (int i = 0; i < 3; i++){
        optionsButton butt(defaultsSpriteIdx, textureSizes[3], optionsText[i], i+1);
        optionsButtonList.push_back(butt);
            std::cout << "Button added in Layer: " << 2 << std::endl;
    }

    //Create Buttons

    //choice Buttons
    std::vector<choiceButton> ChoiceButtonList;

    //connectionButton
    std::vector<connectionButton> ConnectionButtonList;


    
    //Navigating through the menu
    int Current_Layer = 1;
    bool MouseClick = false;
    //MENU LOOP
    while (Current_Layer >= 0 && Current_Layer <= 5) {
        //FIRST DRAW
        BeginDrawing();
            ClearBackground(GRAY);
            BackgroundImage.draw(scaledTextures);
            
            //Draw appropriate Layer
            if (Current_Layer == 1 || Current_Layer == 3){
                for (auto &iterate : DefaultButtonList){
                    if (iterate.currentLayer == Current_Layer){
                        iterate.draw(scaledTextures);
                    }
                }
            } else if (Current_Layer == 0){

            } else if (Current_Layer == 2){
                for (auto &iterate : optionsButtonList){
                    iterate.draw(scaledTextures);
                }
            } else if (Current_Layer == 4){

            } else if (Current_Layer == 5){

            }
            
            Cursor.draw(scaledTextures);
            //Draw helpful tools. fps and two lines in the middle of the screen
            DrawFPS(0,0);
            DrawLine(0, gV::screenHeight/2, gV::screenWidth, gV::screenHeight/2, ORANGE);
            DrawLine(gV::screenWidth/2, 0, gV::screenWidth/2, gV::screenHeight, ORANGE);
            
        EndDrawing();

        
 
        mousePos[0] = GetMouseX();
        mousePos[1] = GetMouseY();
        Cursor.rePosition(mousePos[0], mousePos[1]);


        for (auto &iterater : DefaultButtonList){
            if (iterater.currentLayer == Current_Layer){
                //General updates to the outlook of the button
                iterater.update(Cursor.rec);
                //button is clicked
                if (MouseClick == true && iterater.focus == true){
                    MouseClick = false;//Magical line that prevents that the play button leads to layer 4 instead of 3
                    Current_Layer = iterater.click();
                    std::cout << "Changed layer to: " << Current_Layer << std::endl;
                }
            }
        }

        //Manage KeyInput
        if(IsKeyPressed(KEY_ESCAPE) == true){
            if (Current_Layer == 3){
                Current_Layer = 1;
            } else if (Current_Layer == 4 || Current_Layer == 5){
                Current_Layer = 3;
            } else {
                Current_Layer -= 1;
            }
            std::cout << "layer to: " << Current_Layer << std::endl;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)== true){
            MouseClick = true;
        } else {
            MouseClick = false;
        }


        //while(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) == false && GetCharPressed() != 0){
        //    std::cout << "Waiting for Input to end\n";
        //}   
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
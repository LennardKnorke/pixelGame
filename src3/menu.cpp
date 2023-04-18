//Menu loop and menu classes
#include "declr.hpp"
#include "tilemakros.hpp"

//Template for a display class. Used for the background and mouse
class displayImage {
    public:
    short textureIdx; //Index for the appropriate texture to draw
    Rectangle rec;  //contains size[width, height] and position[x,y]
    //Save the size/ position and texture idx information.
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

//
class defaultButton {
    public:
    //Contins size[width, height] and x,y position of upper left corner
    Rectangle rec;

    //text screen position of the upper left corner             
    int TextPosition[2];

    //button type. 0:defaultButton, 1:create Button, 2: choose button, 3: optionsButton, 4:connectionButton             
    int ButtonType;

    //turns true, if MouseRec colllides with button
    bool Focus;

    //Assigned Layer
    int Layer;

    //First, second, third or fourth button? starts at 1!    
    int ButtIdx;

    //Which layer does the button lead to    
    int FollowUpLayer;

    //Text displayed on Button    
    std::string ButtonText;

    //width of Button Text    
    int ButtonText_Length;

    //Index for the appropriate texture to draw    
    int TextureIdx[2];

    //if true, then clicking will immideatly lead to the next layer
    bool DirectFollowOnClick = true;

    //If directFollowOnClick is false, then the button can be used to check for active interaction
    bool InUse = false;

    bool Available = true;

    //Create a default button
    defaultButton(int spriteIdx[], int size[], std::string text, int lay, int pos, int maxPos, int funcIdx, int type){
        //Save general information
        ButtonType = type;
        Layer = lay;
        Focus = false;
        ButtIdx = pos;
        ButtonText = text;
        FollowUpLayer = funcIdx;

        //Save texture information, size, position, index in texturelist 
        TextureIdx[0] = spriteIdx[1];
        TextureIdx[1] = spriteIdx[0];
        rec.height = size[1];
        rec.width = size[0]; 
        rec.x = (gV::windowWidth / 2) - (rec.width / 2);
        rec.y = (((gV::windowHeight / (maxPos + 1)) * pos) - (rec.height / 2));

        //Set position and size of button text;
        ButtonText_Length = MeasureText(ButtonText.c_str(), 30 / gV::screenRatio);
        TextPosition[0] = (gV::windowWidth / 2) - (ButtonText_Length / 2);
        TextPosition[1] = ((gV::windowHeight / (maxPos + 1)) * pos) - ((30 / gV::screenRatio) / 2);
    }

    //Draw button texture and text. Texture drawn depends whether the button is in focus
    virtual void draw(Texture2D textureList[]){
        if (Focus){
            DrawTexture(textureList[TextureIdx[0]], rec.x, rec.y, WHITE);
        } 
        else {
            DrawTexture(textureList[TextureIdx[1]], rec.x, rec.y, WHITE);
        }
        DrawText(ButtonText.c_str(), TextPosition[0], TextPosition[1], 30 / gV::screenRatio, BLACK);
    }

    //Button Click
    virtual void click(void){
        return;
    }

    //General Update function.
    //Checks whether the mouse is on the button and will set it as "focus"ed
    virtual void update(Rectangle mouseRec){
        if (CheckCollisionRecs(mouseRec, rec)){
            Focus = true;
        } 
        else {
            Focus = false;
        }
    }

    //in case of resolution or fullscreen changes
    virtual void updateSize(void){
        return;
    }
    //In response to grafic changes or addition/ removal of buttons in layer
    virtual void updatePosition(int maxButts){
        return;
    }

    virtual ~defaultButton(void){
        return;
    }
};

//Button for creating profiles or new worlds
class createButton : public defaultButton 
{
    public:
    //Input of the user.
    char input[MAX_INPUT_LENGTH] = {'\0'};        
    //Copy of the original text to switch around between displayed texts
    std::string ogTextCopy; 
    //Number of chars entered by the user    
    int numberInputChars = 0;   
    //Checks whether it should be shown/updated/used!
    createButton(int spriteIdx[], int size[], std::string text, int lay, int pos, int maxPos, int funcIdx, int type) : defaultButton(spriteIdx, size, text, lay, pos, maxPos, funcIdx, type){
        ogTextCopy = ButtonText;
        DirectFollowOnClick = false;
        if (pos >= 5){
            Available = false;
        }
    }
    void draw(Texture2D textureList[]) override {
        if(Available){
            if (Focus == true){
                DrawTexture(textureList[TextureIdx[0]], rec.x, rec.y, WHITE);
            } 
            else {
                DrawTexture(textureList[TextureIdx[1]], rec.x, rec.y, WHITE);
            }
            DrawText(ButtonText.c_str(), TextPosition[0], TextPosition[1], 30 / gV::screenRatio, BLACK);
        }
    }

    //Change what to display on click
    void click(void) override {
        if (InUse == false && Available){
            InUse = true;
            ButtonText = input;
            updateHorizontalTextPosition();
        }
    }

    //add a char to the user input
    void addInputChar (int inputKey){
        if ((numberInputChars <= MAX_INPUT_LENGTH)){
            if ((inputKey >= 97 && inputKey <= 122)|| (inputKey >= 65 && inputKey <= 90) || (inputKey >=48 && inputKey <= 57)){
                input[numberInputChars] = (char)inputKey;
                input[numberInputChars + 1] = '\0';
                numberInputChars += 1;
                ButtonText = input;
                updateHorizontalTextPosition();
            }
        }  
    }

    //Update the text position
    void updateHorizontalTextPosition(void){
        ButtonText_Length = MeasureText(ButtonText.c_str(), 30 / gV::screenRatio);
        TextPosition[0] = (gV::windowWidth / 2) - (ButtonText_Length / 2);
    }

    //confirm the current input/ return the input. End user input
    std::string confirmInput(void){
        InUse = false;
        return ButtonText;
    }

    //Removes the latest char of the input.
    void removeInputChar(void){
        numberInputChars -= 1;
        if (numberInputChars < 0){
            numberInputChars = 0;
        } 
        input[numberInputChars] = '\0';
        ButtonText = input;
        updateHorizontalTextPosition();
    }

    //reset user input and display the original button text
    void cancelInput(void){
        InUse = false;
        input[0] = '\0';
        numberInputChars = 0;
        ButtonText = ogTextCopy;
        updateHorizontalTextPosition();
    }

    //Update Vertical position!
    void updatePosition(int maxButts) override{
        if (maxButts >= 5){
            Available = false;
        } 
        else {
            Available = true;
            rec.y = (((gV::windowHeight / (maxButts + 1)) * ButtIdx) - (rec.height / 2));
            TextPosition[1] = ((gV::windowHeight / (maxButts + 1)) * ButtIdx) - ((30 / gV::screenRatio) / 2);
        }
    }
};

//Either a profile or world button
class choiceButton : public defaultButton {
    public:
    std::string associated_key;
    choiceButton(int spriteIdx[], int size[], std::string text, int lay, int pos, int maxPos, int funcIdx, int type, std::string key) : defaultButton(spriteIdx, size, text, lay, pos, maxPos, funcIdx, type){
        associated_key = key;
    }
    void update(Rectangle mouseRec)
    {
        if (CheckCollisionRecs(mouseRec, rec)){
            Focus = true;
        } 
        else {
            Focus = false;
        }
    }
    void updatePosition (int maxButts) override {
        if (maxButts >= 5){
            rec.y = (((gV::windowWidth / (maxButts)) * (ButtIdx - 1)) - (rec.height / 2));
            TextPosition[1] = ((gV::windowHeight / (maxButts)) * (ButtIdx - 1)) - ((30 / gV::screenRatio) / 2);
        } 
        else {
            rec.y = (((gV::windowHeight / (maxButts + 1)) * ButtIdx) - (rec.height / 2));
            TextPosition[1] = ((gV::windowHeight / (maxButts + 1)) * ButtIdx) - ((30 / gV::screenRatio) / 2);
        }
    }

    void draw(Texture2D textureList[]) override {
        if (Available){
            if (Focus == true){
                DrawTexture(textureList[TextureIdx[0]], rec.x, rec.y, WHITE);
            } 
            else {
                DrawTexture(textureList[TextureIdx[1]], rec.x, rec.y, WHITE);
            }
            DrawText(ButtonText.c_str(), TextPosition[0], TextPosition[1], 30 / gV::screenRatio, BLACK);
        }
    }
    //Removing will remove entries and files associated with the profile.
    //Button deletion in menu loop!
    void removeButt(std::string associatedKey, std::vector<std::string> &names, std::vector<std::string> &keys){
        //remove info from vectors and update files here!
        return;
    }
    void click(void) override{
        std::cout << "Picked Profile: " << ButtonText << std::endl;
    }
};

//Same as default. But changes window and overall screen information. 
//Can it take all textures and resize them?
class optionsButton : public defaultButton{
    using defaultButton::defaultButton;
    void click(void) override {
        std::cout << "Now it should change into " << ButtonText << std::endl;
        return;
    }
};

//2 connectionButton's needed. To ask for host ip and host port number
class connectionButton : public defaultButton {
    public:
    char input[16] = "";     //User Input text.
    std::string ogTextCopy;     //Copy of the original text to switch around between displayed texts
    int numberInputChars = 0;   //Number of chars inputed for this field.
    int maxInputChars;
        
    connectionButton(int spriteIdx[], int size[], std::string text, int lay, int pos, int maxPos, int funcIdx, int type) : defaultButton(spriteIdx, size, text, lay, pos, maxPos, funcIdx, type){
        ogTextCopy = ButtonText;
        DirectFollowOnClick = false;
        if (pos == 1){
            maxInputChars = 16;
        } 
        else {
            maxInputChars = 6;
        }
    }

    //Change what to display on click
    void click(void) override {
        if (InUse == false){
            InUse = true;
            ButtonText = input;
            updateHorizontalTextPosition();
        }
    }

    //add a char to the user input
    void addInputChar (int inputKey){
        if ((numberInputChars <= maxInputChars)){
            if ((ButtIdx == 1) && (inputKey == 46 || (inputKey >=48 && inputKey <= 57))){
                input[numberInputChars] = (char)inputKey;
                input[numberInputChars + 1] = '\0';
                numberInputChars += 1;
            } 
            else if ((ButtIdx == 2) && (inputKey >=48 && inputKey <= 57)){
                input[numberInputChars] = (char)inputKey;
                input[numberInputChars + 1] = '\0';
                numberInputChars += 1;
            }
            ButtonText = input;
        }
        updateHorizontalTextPosition();
    }

    //Update the text position
    void updateHorizontalTextPosition(void){
        ButtonText_Length = MeasureText(ButtonText.c_str(), 30 / gV::screenRatio);
        TextPosition[0] = (gV::windowWidth / 2) - (ButtonText_Length / 2);
    }

    //confirm the current input/ return the input
    std::string confirmInput(void){
        InUse = false;
        return ButtonText;
    }

    //Removes the latest char of the input.
    void removeInputChar(void){
        numberInputChars -= 1;
        if (numberInputChars < 0){
            numberInputChars = 0;
        } 
        input[numberInputChars] = '\0';
        ButtonText = input;
        updateHorizontalTextPosition();
    }

    //reset input and display the default text
    void cancelInput(void){
        if (InUse){
            InUse = false;
            input[0] = '\0';
            numberInputChars = 0;
            ButtonText = ogTextCopy;
            updateHorizontalTextPosition();
        }
    }

};



int run_Menu(Image imageRefList[NR_MENU_TEXTURES], nlohmann::json configFile){
    //Textures in the menu include: {(0, Background);(1, Cursor); (2, Button_default); (3, Button_inUse); (4, World_default); (5, World_default-_inUse)}
    int textureSizes [NR_MENU_TEXTURES][2] = {{gV::windowWidth, gV::windowHeight}, {50, 50}, {300, 100}, {300, 100}, {300, 100}, {300, 100}};
    Texture2D scaledTextures[NR_MENU_TEXTURES];

    //Scale textures
    for (int m = 0; m < NR_MENU_TEXTURES; m++){
        ImageResize(&imageRefList[m], textureSizes[m][0] / gV::screenRatio, textureSizes[m][1] / gV::screenRatio);
        scaledTextures[m] = LoadTextureFromImage(imageRefList[m]);
        std::cout << "scaled and loaded textures: " << m << std::endl;
    }

    //Info for non-default buttons (available profile and world buttons)
    std::vector<ProfileData> Profiles;
    
    //Position for background and cursor
    int bgPos[2] = {0, 0}; 
    int mousePos[2] = {GetMouseX(), GetMouseY()};

    //Create class instances
    //Background and Cursor
    displayImage BackgroundImage(0, textureSizes[0],bgPos);
    displayImage Cursor(1, textureSizes[1], mousePos);
    std::cout << "Loaded: Background Sprite\nLoaded:Cursor Sprite\n";
    

    //information to create 13 buttons
    std::vector<defaultButton*> Buttons;
    int nBUTTONS = 11;
    int buttonTypes[] =     {1, 0, 0, 0, 3, 3, 0, 0, 1, 4, 4};
    int spriteIdxs[2] =     {2, 3};
    std::string displayTexts[] = {"New Profile", "Play", "Options", "Exit", "High Res", "Low Res", "Host", "Join", "New World", "Enter Host Ip", "Enter Host Port"};
    int layers[] =          {0, 1, 1, 1, 2, 2, 3, 3, 4, 5, 5};
    int positions[] =       {1, 1, 2, 3, 1, 2, 1, 2, 1, 1, 2};
    int maxPositions[] =    {((int)configFile["nprofiles"]) + 1, 3, 3, 3, 2, 2, 2, 2, 1, 2, 2};
    int functionalIdxs[]=   {1, 3, 2, -1, 2, 2, 4, 5, 4, 5, 5};
    
    //
    for (int i = 0; i < nBUTTONS; i++){
        if (buttonTypes[i] == 1){
            Buttons.push_back(new createButton(spriteIdxs, textureSizes[3], displayTexts[i], layers[i], positions[i], maxPositions[i], functionalIdxs[i], buttonTypes[i]));
        } 
        else if (buttonTypes[i] == 0){
            Buttons.push_back(new defaultButton(spriteIdxs, textureSizes[3], displayTexts[i], layers[i], positions[i], maxPositions[i], functionalIdxs[i], buttonTypes[i]));
        } 
        else if (buttonTypes[i] == 3){
            Buttons.push_back(new optionsButton(spriteIdxs, textureSizes[3], displayTexts[i], layers[i], positions[i], maxPositions[i], functionalIdxs[i], buttonTypes[i]));
        } 
        else if (buttonTypes[i] == 4){
            Buttons.push_back(new connectionButton(spriteIdxs, textureSizes[3], displayTexts[i], layers[i], positions[i], maxPositions[i], functionalIdxs[i], buttonTypes[i]));
        }
    }

    //Load in available profiles and create profile buttons and world buttons
    for (int i = 0; i < (int)configFile["nprofiles"]; i++){
        std::cout << "Loading Profile: " << configFile["profiles"][i]["name"] << std::endl;
        ProfileData Profile;
        Profile.Name = configFile["profiles"][i]["name"];
        Profile.Key = configFile["profiles"][i]["key"];
        Buttons.push_back(new choiceButton(spriteIdxs, textureSizes[3], Profile.Name, 0, i + 2, ((int)configFile["nprofiles"]) + 1, 1, 2, Profile.Key));
        Profiles.push_back(Profile);
    }
    //Update Layer 0 positions!
    for (auto &iterator : Buttons){
        if (iterator->Layer == 0){
            iterator->updatePosition(Profiles.size() + 1);
        }
    }

    for (auto &fileIterator : std::filesystem::directory_iterator("../worlds/")){
        if (fileIterator.exists() && fileIterator.is_regular_file()){
            std::ifstream instream(fileIterator.path().string(), std::ios::binary);
            if (instream){
                SaveFile tmpSave;
                tmpSave.readFromFile(instream);
            } 
            else {
                std::cout<<"ERROR\n";
            }
        }
        
    }




    //Navigating through the menu
    int Current_Layer = 0;
    bool activeUserInput = false;
    bool MenuRunning = true;
    bool deleting = false;
    std::string ItemToDelete = "";
    std::string warning = "Press 'Enter' to confirm deletion, press 'Escape' to cancel";
    //MENU LOOP
    while (MenuRunning){
        //FIRST DRAW
        BeginDrawing();
            ClearBackground(GRAY);
            BackgroundImage.draw(scaledTextures);
            
            if (deleting){
                if (Current_Layer == 0){
                    DrawText("About to delete profile:", (gV::windowWidth / 2) - (MeasureText("About to delete profile:",30 / gV::screenRatio)/ 2), ((gV::windowHeight / 2) - (30 / gV::screenRatio)) - (60 / gV::screenRatio), 30 / gV::screenRatio, BLACK);
                }
                else if (Current_Layer == 4){
                    DrawText("About to delete World:", (gV::windowWidth / 2) - (MeasureText("About to delete world:",30 / gV::screenRatio)/ 2), ((gV::windowHeight / 2) - (30 / gV::screenRatio)) - (60 / gV::screenRatio), 30 / gV::screenRatio, BLACK);
                }
                DrawText(ItemToDelete.c_str(), (gV::windowWidth / 2) - (MeasureText(ItemToDelete.c_str(),30 / gV::screenRatio)/ 2), (gV::windowHeight / 2) - (30 / gV::screenRatio), 30 / gV::screenRatio, BLACK);
                DrawText(warning.c_str(), (gV::windowWidth / 2) - (MeasureText(warning.c_str(),30 / gV::screenRatio)/ 2), ((gV::windowHeight / 2) - (30 / gV::screenRatio)) + (90 / gV::screenRatio), 30 / gV::screenRatio, BLACK);

            }
            else{//Draw appropriate Layer
                for (auto &iterator : Buttons){
                    if (iterator->Layer == Current_Layer){
                        iterator->draw(scaledTextures);
                    }
                }
                //Draw Cursor
                if (!activeUserInput){
                    Cursor.draw(scaledTextures);
                }
            }
            
            //DELETE LATER!!
            //helpful tools. fps and two lines in the middle of the screen
            DrawFPS(0,0);
            DrawLine(0, gV::windowHeight/2, gV::windowWidth, gV::windowHeight/2, ORANGE);
            DrawLine(gV::windowWidth/2, 0, gV::windowWidth/2, gV::windowHeight, ORANGE);
            //END DELETION!!

        EndDrawing();

        //MANAGE INPUT
        //Update Mouse Position
        if (activeUserInput == false){
            mousePos[0] = GetMouseX();
            mousePos[1] = GetMouseY();
            Cursor.rePosition(mousePos[0], mousePos[1]);
        }
        

        //Run 'Update' on all the buttons of the current Layer
        for (auto &iterator : Buttons){
            if (iterator->Layer == Current_Layer){
                iterator->update(Cursor.rec);
            }
        }

        //Handle Mouse Input seperate!
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if (activeUserInput == false){
                for (auto &iterator : Buttons){
                    //check if the layer and focus fit
                    if (iterator->Layer == Current_Layer && iterator->Focus){
                        //if this is a one click direct move button. change layer!
                        if (iterator->DirectFollowOnClick == true){   
                            iterator->click();
                            if (Current_Layer == 0){
                                gV::activeProfileName = iterator->ButtonText;
                                choiceButton * choB = dynamic_cast<choiceButton *>(iterator);
                                if (choB){
                                    gV::activeProfileKey = choB->associated_key;
                                }
                                for (auto &subiterator : std::filesystem::directory_iterator("../worlds/")){
                                    if (subiterator.is_regular_file()){
                                        std::cout << subiterator << std::endl;
                                    }
                                }
                            } 
                            else if (Current_Layer == 2){
                                //Update Resolution

                                //Resize Window

                                //Reload Textures

                                //Recalculate button positions
                            }
                            else if (Current_Layer == 4){
                                gV::hosting_chosen_world = iterator->ButtonText;
                            }
                            Current_Layer = iterator->FollowUpLayer;
                            break;
                        
                        }
                        //Otherwise stop mouse activity! and start receiving input
                        else {
                            createButton * creB = dynamic_cast<createButton *>(iterator);
                            if (!creB || creB->Available){//its either a connection button or a creation button which needs to be available!
                                iterator->click();
                                activeUserInput = true;
                            }
                        }
                    break;
                    }
                }
            }
        }

        //Button management
        //What if the escape button is pressed
        if(IsKeyPressed(KEY_ESCAPE)){
            if (activeUserInput){
                for (auto &iterator : Buttons){
                    if (iterator->InUse){
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
            }
            else if (deleting){
                deleting = false;
            } 
            else {
                if (Current_Layer == 3 || Current_Layer == 5){
                    Current_Layer -= 2;
                } 
                else {
                    Current_Layer -= 1;
                    if (Current_Layer == 0){
                        for (auto &subiterator : Buttons){
                            if (subiterator->Layer == 0 && subiterator->ButtonType == 5){
                                delete subiterator;
                            }
                        }
                        //Update Profile List (remove worlds)
                    }
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
                    if (iterator->InUse){
                        //entering a host ip/port to join as a client!
                        connectionButton * conB = dynamic_cast<connectionButton *>(iterator);
                        if (conB && conB->ButtonText != ""){
                            if (iterator->ButtIdx == 1){
                                gV::host_Ip_Adress = conB->confirmInput();
                                std::cout << "Entered Host Ip: " << gV::host_Ip_Adress << std::endl;
                            } 
                            else {
                                gV::host_Port = std::atoi(conB->confirmInput().c_str());
                                std::cout << "Entered Host Port: " << gV::host_Port << std::endl;
                            }
                            activeUserInput = false;
                            break;
                        }
                        //creating a new profile/world
                        createButton *creB = dynamic_cast<createButton *>(iterator);
                        if (creB && creB->ButtonText != ""){
                            //create a new profile entry!, button!! AND update positions in layer 0 and 4!
                            if (Current_Layer == 0){
                                //Note name and generate key
                                gV::activeProfileName = creB->confirmInput();
                                creB->cancelInput(); //Not really but will reset the text
                                std::string generatedProfileKey = generateString();
                                std::vector <std::string> takenKeys;
                                for (long long unsigned int i = 0; i < Profiles.size(); i++){
                                    takenKeys.push_back(Profiles[i].Key);
                                }
                                while (findStringIndex(generatedProfileKey, takenKeys) != -1){
                                    generatedProfileKey = generateString();
                                }
                                gV::activeProfileKey = generatedProfileKey;
                                //save and add to the available ones
                                addProfileToConfig(configFile, gV::activeProfileKey, gV::activeProfileName);
                                ProfileData Profile;
                                Profile.Name = gV::activeProfileName;
                                Profile.Key = gV::activeProfileKey;
                                //updatePositions!
                                Profiles.push_back(Profile);
                                Buttons.push_back(new choiceButton(spriteIdxs, textureSizes[3], gV::activeProfileName, 0, Profiles.size() + 1, Profiles.size() + 1, 1, 2, generatedProfileKey));
                                //Create new button and update positions
                                for (auto &subiterator : Buttons){
                                    if (subiterator->Layer == 0){
                                        iterator->updatePosition(Profiles.size() + 1);
                                    } 
                                    else if (subiterator->Layer == 4){
                                        iterator->updatePosition(1);
                                    }
                                }
                                //Confirm and move on to next layer
                                std::cout << "Added Profile: " << gV::activeProfileName << ". With key : " << generatedProfileKey << std::endl;
                                Current_Layer = 1;

                            //Create a new world file and adjust positions in layer 4!
                            } 
                            else if (Current_Layer == 4){
                                std::cout << "Creating World: ";
                                gV::hosting_chosen_world = creB->confirmInput();
                                std::cout << gV::hosting_chosen_world << std::endl;
                                createWorldFile(gV::activeProfileName, gV::activeProfileKey, gV::hosting_chosen_world);
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
                    if (iterator->InUse){
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
                    }
                }
            }
            else if (!activeUserInput && deleting){

            }
            else if (!activeUserInput && !deleting && (Current_Layer == 0 || Current_Layer == 4)){
                for (auto &iterator : Buttons){
                    if (iterator->Focus){
                        ItemToDelete = iterator->ButtonText;
                        deleting = true;
                    }
                }
            }
        } 
        else {//ANY OTHER BUTTONS PRESSED??
            if (activeUserInput){
                for (auto &iterator : Buttons){
                    if (iterator->InUse){//only allowed for specific buttons in specific states
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
        if (Current_Layer == 4 && (gV::wantsHost != true)){
            gV::wantsHost = true;
        } 
        else if (Current_Layer == 5 && (gV::wantsHost != false)){
            gV::wantsHost = false;
        } 
        else if (Current_Layer == 3){
            gV::host_Ip_Adress = "";
            gV::host_Port = -1;
            gV::hosting_chosen_world = "";
        } 
        else if (Current_Layer == -1){
            MenuRunning = false;
        }

        //EITHER, the user has entered a port and ip to join as a client, or has chosen a world and is hosting
        if ((gV::wantsHost == false && Current_Layer == 5 && gV::host_Port != -1 && gV::host_Ip_Adress != "") || (gV::wantsHost == true && Current_Layer == 4 && gV::hosting_chosen_world != "")){
            //Chill all buttons!
            for (auto & iterator : Buttons){
                iterator->InUse = false;
            }
            MenuRunning = false;
        }

        if (!IsWindowFocused()){
            MinimizeWindow();
        }

        
    }

    //Unload Texture.    
    for (int i = 0; i < NR_MENU_TEXTURES; i++){
        UnloadTexture(scaledTextures[i]);
    }

    //Either we ended the reduced the layer enough to leave
    if (Current_Layer == -1) {
        return -1;
    //Or we are ready to play fix return to value to 1 to start loading, setting up server and stuff!
    } 
    else {
        return -1;
    }
}
#include "application.hpp"

int GameApplication::runMenu(){

    //Create Layer
        //Create Buttons

    //Changes in response to errors!

    //Menu Loop
        //Draw Mouse
        //Draw Layer
            //Draw Buttons
        //Update Layer
            //Update Buttons
        //Update Mosue
        //Register Input


    while (1){
        BeginDrawing();
        ClearBackground(WHITE);
        DrawTexture(sprites.menuSprites[idx_Menu_Button], 0, 0, WHITE);
        EndDrawing();
        if (IsKeyPressed(KEY_ESCAPE)){
            break;
        }
    }

    return APPLICATIONSTATE_QUIT;
}

//Downloaded from https://www.sfml-dev.org/files/SFML-2.6.0-windows-gcc-13.1.0-mingw-64-bit.zip. for mingw (w64, over mysys)
//copied files according to https://www.youtube.com/watch?v=rZE700aaT5I
#include "application.hpp"

//Structure explained so far

//Application (class)
    //constructor reads/creates a setting file
    //Set up the window
    //Load assets (sounds + textures)
    //Then basically set up the game assets (connect textures and class instances)
    //Load available Savefiles
    //GameLoop, switches between menu, and game
        //menu
            //set up buttons (positions, text)
            //set up variables to guide through menu
            //Loop
                //Draw Menu
                //Handle Input
                //Update Menu items (cursor, buttons)
        //game (not done yet, probably something likes this:)
            //start server if host
            //Connect with host
            //Load World
            //Loop
                //Draw
                //Notice input
                //Send input
                //Receive changes
                //update
                //possibility for an escape menu


int main(){
    Application Game;
    if (Game.error != NoErr){
        system("pause");
        return 1;
    }
    return 0;
}

///

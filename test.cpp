#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <thread>
#include <random>
#include <vector>
#include <time.h>
#include <filesystem>
//External libs
#include <raylib.h>

int main (void){


    Image img = LoadImage("assets/sprites/menu/1.png");
    //Texture2D textureTmp = LoadTextureFromImage(img);
    Color *co = LoadImageColors(img);

    std::cout << img.height << "\t" << img.width <<std::endl;

    for (long i = 0; i < img.width * img.height; i++){
        
    }
    

    InitWindow(500,500, "Test");
    while (1){

        BeginDrawing();

        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE)){
            break;
        }
    }
    CloseWindow();
    UnloadImageColors(co);
    //UnloadTexture(textureTmp);
    UnloadImage(img);
    return 0;
}
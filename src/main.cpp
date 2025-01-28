#include <cstring>
#include <windows.h>
#include "application.hpp"



int main(int argc, char *argv[])
{
    bool devMode = false;
    if (argc == 2 && strcmp(argv[1], "-d") == 0){
        devMode = true;
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        freopen("CONIN$", "r", stdin);
        std::cout << "Running in developer mode\n";
    }

    Application app(devMode);




    if (devMode){
        std::cout << "Application ended" << std::endl;
        system("pause");

    }
}
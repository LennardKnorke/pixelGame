//Downloaded from https://www.sfml-dev.org/files/SFML-2.6.0-windows-gcc-13.1.0-mingw-64-bit.zip. for mingw (w64, over mysys)
//copied files according to https://www.youtube.com/watch?v=rZE700aaT5I
#include "application.hpp"

#include <cstring>

bool run_as_dev(char *argv1){
    return strcmp(argv1, "-d") == 0;
}

int main(int argc, char *argv[]){
    
    bool dev;
    if (argc > 1){
        dev = run_as_dev(argv[1]);
    }
    else{
        dev = false;
    }

    Application Game(dev);
    if (Game.error != NoErr){
        system("pause");
        return 1;
    }
    
    return 0;
}

///

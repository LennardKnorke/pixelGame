STRUCTURE
/build			        Compile and Run from this folder! 
    .Makefile           Compiles using "mingw32-make" in the console
    .Game.exe           Here the .exe will be placed

/include 		        For all the extra libraries and headerfiles (raylib, json stuff)

/lib		            Contains the .a file for raylib (header stuff)

/src			        Self produced sourcecode
    .Main.cpp           Main game loop
    .menu.cpp           Menu loop
    .localGameLoop.cpp  In game loop here
    .server.cpp         Server script
    .overallFunction.cpp    filled with all kinds of extra functions
    .loadingScreen.cpp  attempts to connect with the host and then load textures.

/world			    contains world files.

.config			    json. contains infos about the last used screen, screen settings, last used profile and etc

.readme			    this.


To do next:
1. seed generator/ generate world tiles out of it.
1. Socket implementation
2. send data to server
2. Implement grafics in response to server as send by server
3. Update menu for world file development and error problems in from the server.
4. Profit.
5.Start on the game


List of ideas:
- 1-4 Players
- Permadeath?
- Skilltree?
- Mining and crafting.
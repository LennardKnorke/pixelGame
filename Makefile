all: game server#server #Update all sourcefiles and compile to exe

################################
##########STDLIBS###############
################################
compile_STDLIBS:
	g++ -I./src/include -c ./src/code/stdlibs.cpp

################################
##########Game+Server###########
################################
game: compile_game link_game clean_o
compile_game:
	g++ -I./src/include -c ./src/code/*.cpp
link_game:
	g++ *.o -o Game -L src/lib -lsfml-graphics -lsfml-audio -lsfml-network -lsfml-system -lsfml-window

server: compile_STDLIBS compile_server link_server clean_o
compile_server:
	g++ -c src/server_code/*.cpp -Isrc/include
link_server:
	g++ *.o -o Server -L src/lib -lsfml-graphics -lsfml-audio -lsfml-network -lsfml-system -lsfml-window

################################
##########TEST##################
################################
test: compile_test link_test

compile_test:
	g++ -I./src/include -c ./src/testing_code/*.cpp

link_test:
	g++ test.o -o Test -L src/lib -lsfml-graphics -lsfml-audio -lsfml-network -lsfml-system -lsfml-window

################################
##########Cleaning##############
################################
clean_o:
	del test.o application.o client.o gamesave.o gameSaveSummary.o local_game.o main.o menu.o menuButtons.o stdlibs.o
clean_exe:
	del Test.exe Game.exe Server.exe
clean: clean_o clean_exe
clean_all: clean
	del settings.bin multiplayer.txt

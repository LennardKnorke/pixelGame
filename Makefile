all: game server#server #Update all sourcefiles and compile to exe

game: compile_game link_game
server: compile_server link_server


compile_game:
	g++ -I./src/include -c ./src/code/*.cpp


compile_server:
	g++ -c src/server_code/*.cpp -Isrc/include

##
link_game:
	g++ *.o -o Game -L src/lib -lsfml-graphics -lsfml-audio -lsfml-network -lsfml-system -lsfml-window

link_server:
	g++ *.o -o Server -L src/lib -lsfml-graphics -lsfml-audio -lsfml-network -lsfml-system -lsfml-window
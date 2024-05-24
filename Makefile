all: clean client server #Update all sourcefiles and compile to exe
all_static:clean client_static server_static


#Compile tools
COMPILER = g++ -std=c++2a -m64
COMPILER_s = g++ -std=c++2a -m64 -DSFML_STATIC -static
WFLAGS = -Wall -Wfatal-errors -pedantic -Wextra -O2

#Path tools
INCLUDE_PATH = ./include
LIB_PATH = ./lib
BIN_PATH = ./bin

#Linking tools
sfml_links =  -l sfml-graphics -l sfml-window -l sfml-audio -l sfml-network -l sfml-system
other_links = -lopengl32 -lwinmm -lgdi32

sfml_links_static = -l sfml-graphics-s -l sfml-window-s -l sfml-audio-s -l sfml-network-s -l sfml-system-s
extra_std_statics = -lfreetype -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lws2_32

LINK = -L $(LIB_PATH) $(sfml_links) $(other_links)
LINK_STATIC = -L $(LIB_PATH) $(sfml_links_static) $(other_links) $(extra_std_statics)


################################
##########STDLIBS###############
################################
compile_STDLIBS:
	$(COMPILER) $(WFLAGS) -c ./src/client_side/stdlibs.cpp -I $(INCLUDE_PATH)
compile_STDLIBS_s:
	$(COMPILER_s) $(WFLAGS) -c ./src/client_side/stdlibs.cpp -I $(INCLUDE_PATH)
################################
##########Game+Server###########
################################
client: compile_client
	$(COMPILER) *.o -o Game.exe $(LINK)
compile_client:
	$(COMPILER) $(WFLAGS) -c ./src/client_side/*.cpp -I $(INCLUDE_PATH)
client_static: compile_client_s
	$(COMPILER_s) *.o -o Game.exe $(LINK_STATIC)
compile_client_s:
	$(COMPILER_s) $(WFLAGS) -c ./src/client_side/*.cpp -I $(INCLUDE_PATH)	

server: compile_STDLIBS compile_server
	$(COMPILER) *.o -o Server.exe $(LINK)
compile_server:
	$(COMPILER) $(WFLAGS) -c ./src/server_side/*.cpp -I $(INCLUDE_PATH)
server_static: compile_STDLIBS_s compile_server_s
	$(COMPILER_s) *.o -o Server.exe $(LINK_STATIC)
compile_server_s:
	$(COMPILER_s) $(WFLAGS) -c ./src/server_side/*.cpp -I $(INCLUDE_PATH)

	
################################
##########Cleaning##############
################################
clean_o:
	del *.o
clean_exe:
	del Test.exe Game.exe Server.exe
clean: clean_o clean_exe
clean_all: clean
	del settings.bin multiplayer.txt
all: clean client server#server #Update all sourcefiles and compile to exe

COMPILER = g++ -std=c++17 -m64
WFLAGS = -Wall -Wfatal-errors -pedantic -Wextra -O2

INCLUDE_PATH = ./include
LIB_PATH = ./lib

sfml_links =  -lsfml-main -lsfml-system -lsfml-graphics -lsfml-window -lsfml-network -lsfml-audio
other_links = -lopengl32 -lwinmm -lgdi32 
LINK = -L $(LIB_PATH) $(other_links) $(sfml_links) 


################################
##########STDLIBS###############
################################
compile_STDLIBS:
	$(COMPILER) $(WFLAGS) -c ./src/client_side/stdlibs.cpp -I $(INCLUDE_PATH)

################################
##########Game+Server###########
################################
client: compile_client
	$(COMPILER) *.o -o Game.exe $(LINK)
compile_client:
	$(COMPILER) $(WFLAGS) -c ./src/client_side/*.cpp -I $(INCLUDE_PATH)
	

server: compile_STDLIBS compile_server
	$(COMPILER) *.o -o Server.exe $(LINK)
compile_server:
	$(COMPILER) $(WFLAGS) -c ./src/server_side/*.cpp -I $(INCLUDE_PATH)
	

################################
##########TEST##################
################################
test: compile_STDLIBS compile_test
	$(COMPILER) *.o -o Test.exe $(LINK)
compile_test:
	$(COMPILER) $(WFLAGS) -c ./src/testing_code/*.cpp -I $(INCLUDE_PATH)

	
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
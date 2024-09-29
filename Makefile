all: client server
#default all, otherwise all_static, all_dynamic

# Variables
WFLAGS = -Wall -Wfatal-errors -pedantic -Wextra -O2
INCLUDE_PATH = ./include
LIB_PATH = ./lib

BIN_PATH = ./bin
CLIENT_BIN_PATH := $(BIN_PATH)/client
SERVER_BIN_PATH := $(BIN_PATH)/server

SRC_PATH = ./src
CLIENT_SRC_PATH := $(SRC_PATH)/client
SERVER_SRC_PATH := $(SRC_PATH)/server
SHARED_SRC_PATH := $(SRC_PATH)/shared


# Static build variables
COMPILER_s = x86_64-w64-mingw32-g++ -std=c++2a -DSFML_STATIC -static 
# removed -m64 
LINK_STATIC = -L $(LIB_PATH) $(sfml_links_static) $(other_links) $(extra_std_statics)
sfml_links_static = -l sfml-graphics-s -l sfml-window-s -l sfml-audio-s -l sfml-network-s -l sfml-system-s
extra_std_statics = -lfreetype -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lws2_32
other_links = -lopengl32 -lwinmm -lgdi32 -lOpenAL32

# Source files
#SRC := $(wildcard $(SRC_PATH)/*.cpp)
#OBJ := $(patsubst $(SRC_PATH)/%.cpp, $(BIN_PATH)/%.o, $(SRC))


CLIENT_SRC := $(wildcard $(CLIENT_SRC_PATH)/*.cpp)
SERVER_SRC := $(wildcard $(SERVER_SRC_PATH)/*.cpp)
SHARED_SRC := $(wildcard $(SHARED_SRC_PATH)/*.cpp)


SHARED_OBJ_c := $(patsubst $(SHARED_SRC_PATH)/%.cpp,$(CLIENT_BIN_PATH)/%.o,$(SHARED_SRC))
CLIENT_OBJ := $(patsubst $(CLIENT_SRC_PATH)/%.cpp,$(CLIENT_BIN_PATH)/%.o,$(CLIENT_SRC))


SHARED_OBJ_s := $(patsubst $(SHARED_SRC_PATH)/%.cpp,$(SERVER_BIN_PATH)/%.o,$(SHARED_SRC))
SERVER_OBJ := $(patsubst $(SERVER_SRC_PATH)/%.cpp,$(SERVER_BIN_PATH)/%.o,$(SERVER_SRC))

# Single file compilation
domain ?=
file ?=
single:
ifeq ($(file),)
	$(error No File specified)
endif
ifeq ($(domain),c)
	echo $(CLIENT_SRC_PATH)/$(file).cpp
else ifeq ($(domain),s)
	echo $(SERVER_SRC_PATH)/$(file).cpp	
else ifeq ($(domain),sh)
	echo $(SHARED_SRC_PATH)/$(file).cpp
else
	$(error No Domain specified)
endif


	


# Compile and linking client ressources
$(CLIENT_BIN_PATH)/%.o: $(SHARED_SRC_PATH)/%.cpp
	$(COMPILER_s) $(WFLAGS) -c $< -I $(INCLUDE_PATH) -o $@ 
$(CLIENT_BIN_PATH)/%.o: $(CLIENT_SRC_PATH)/%.cpp
	$(COMPILER_s) $(WFLAGS) -c $< -I $(INCLUDE_PATH) -o $@
link_client:
	$(COMPILER_s) ./bin/client/*.o -o Game.exe $(LINK_STATIC)
client: clean_client $(SHARED_OBJ_c) $(CLIENT_OBJ) link_client
#$(COMPILER_s) ./bin/client/*.o $(LINK_STATIC) -o Game.exe


# Compile and linking server ressources
$(SERVER_BIN_PATH)/%.o: $(SHARED_SRC_PATH)/%.cpp
	$(COMPILER_s) $(WFLAGS) -c $< -o $@ -I $(INCLUDE_PATH)
$(SERVER_BIN_PATH)/%.o: $(SERVER_SRC_PATH)/%.cpp
	$(COMPILER_s) $(WFLAGS) -c $< -o $@ -I $(INCLUDE_PATH)
link_server:
	$(COMPILER_s) ./bin/server/*.o -o Server.exe $(LINK_STATIC)
server: clean_server $(SHARED_OBJ_s) $(SERVER_OBJ) link_server
#$(COMPILER_s) $(WFLAGS) ./bin/server/*.o -o Server.exe $(LINK_STATIC)
	


# Clean Up Tools
clean_all: clean_client clean_server

clean_client:
	for %%f in ($(CLIENT_BIN_PATH)/*.o) do del "bin\client\%%f"
	if exist Game.exe del Game.exe
	if exist settings.bin del settings.bin
clean_server:
	for %%f in ($(SERVER_BIN_PATH)/*.o) do del "bin\server\%%f"
	if exist Server.exe del Server.exe
	if exist settings.bin del settings.bin


# FORGOTTEN STUFF
# Dynamic build variables
COMPILER_d = g++ -std=c++2a -m64
LINK_DYNAMIC = -L $(LIB_PATH) $(sfml_link_d) $(other_links)
sfml_link_d =  -l sfml-graphics -l sfml-window -l sfml-audio -l sfml-network -l sfml-system

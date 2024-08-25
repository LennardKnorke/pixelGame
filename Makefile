
all: all_static
tmp:
	$(COMPILER_s) $(WFLAGS) -c ./src/map_loader.cpp -I $(INCLUDE_PATH) -o $(BIN_PATH)/map_loader.o
	$(COMPILER_s) ./$(BIN_PATH)/map_loader.o -o get_map.exe $(LINK_STATIC)
	
#default all, otherwise all_static, all_dynamic

# Variables
WFLAGS = -Wall -Wfatal-errors -pedantic -Wextra -O2
INCLUDE_PATH = ./include
LIB_PATH = ./lib
BIN_PATH = ./bin
SRC_PATH = ./src
other_links = -lopengl32 -lwinmm -lgdi32 -lOpenAL32

# Source files
SRC := $(wildcard $(SRC_PATH)/*.cpp)
OBJ := $(patsubst $(SRC_PATH)/%.cpp, $(BIN_PATH)/%.o, $(SRC))


# Static build variables
COMPILER_s = g++ -std=c++2a -m64 -DSFML_STATIC -static
LINK_STATIC = -L $(LIB_PATH) $(sfml_links_static) $(other_links) $(extra_std_statics)
sfml_links_static = -l sfml-graphics-s -l sfml-window-s -l sfml-audio-s -l sfml-network-s -l sfml-system-s
extra_std_statics = -lfreetype -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lws2_32



# Dynamic build variables
COMPILER_d = g++ -std=c++2a -m64
LINK_DYNAMIC = -L $(LIB_PATH) $(sfml_link_d) $(other_links)
sfml_link_d =  -l sfml-graphics -l sfml-window -l sfml-audio -l sfml-network -l sfml-system



# Targets
all_static: clean $(OBJ)
	$(COMPILER_s) $(OBJ) -o Game.exe $(LINK_STATIC)

all_dynamic: clean $(OBJ)
	$(COMPILER_d) $(OBJ) -o Game.exe $(LINK_DYNAMIC)


$(BIN_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(COMPILER_s) $(WFLAGS) -c $< -I $(INCLUDE_PATH) -o $@


compile_static:
	$(COMPILER_s) $(WFLAGS) -c ./src/*.cpp -I $(INCLUDE_PATH) -o $(BIN_PATH)/$@F)

compile_dynamic:
	$(COMPILER_d) $(WFLAGS) -c ./src/*.cpp -I $(INCLUDE_PATH) -o $(BIN_PATH)/$@F)


compile_file_static:
	$(COMPILER_s) $(WFLAGS) -c $(SRC_PATH)/$(file) -I $(INCLUDE_PATH) -o $(BIN_PATH)/$(basename $(file)).o


# single file compilation
file ?=
single:
ifeq ($(file),)
	@echo "No file specified"
else
	@echo "Compiling $(file).cpp"
	$(COMPILER_s) $(WFLAGS) -c $(SRC_PATH)/$(file).cpp -I $(INCLUDE_PATH) -o $(BIN_PATH)/$(basename $(file)).o
	$(COMPILER_s) $(OBJ) -o Game.exe $(LINK_STATIC)
endif

# Clean Up Tools
clean:
	for %%f in ($(BIN_PATH)/*.o) do del "bin\%%f"
	if exist Game.exe del Game.exe
clean_all: clean# can add more clean ups for other files if needed
	if exist settings.bin del settings.bin


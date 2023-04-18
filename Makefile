CC = g++
CFLAGS = -Wall -pedantic -O2 -Wno-missing-braces
INCLUDE = -I include/
LINK = -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
FILES = src/*.cpp -o Game.exe
FILES2 = src3/*.cpp -o Game.exe
DE = -g
TIME = -ftime-report

default:
	$(CC) $(FILES) $(CFLAGS) $(INCLUDE) $(LINK)
debug: 
	$(CC) $(FILES) $(CFLAGS) $(INCLUDE) $(LINK) $(DE)

#compile a file which runs tests on specific new/unknown features
test:
	$(CC) test.cpp -o test.exe $(CFLAGS) $(INCLUDE) $(LINK) 

#old files/ version
version2:
	$(CC) $(FILES2) $(CFLAGS) $(INCLUDE) $(LINK)

version2debug:
	$(CC) $(FILES2) $(CFLAGS) $(INCLUDE) $(LINK) $(DE)
all: compile_ALL link #Update all sourcefiles and compile to exe
application: compile_Application link#Update only changes in application, then compile to execute
#Optional: Add a command to leave the .o files in another folder for more organization.
compile_ALL:
	g++ -c src/code/*.cpp -Isrc/include

compile_Application:
	g++ -c src/code/application.cpp -Isrc/include






###########################################################################################
###OUTPUT FOR THE APPLICATION
###########################################################################################
#If optional above implemented, then also adapt second command to {folderAsAbove}/*.o
link:
	g++ *.o -o Game -L src/lib -lsfml-graphics -lsfml-audio -lsfml-network -lsfml-system -lsfml-window

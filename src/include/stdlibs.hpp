#pragma once
#ifndef STDLIBS_HPP
#define STDLIBS_HPP
#include <iostream>
#include <fstream>
#include <cassert>
#include <thread>
#include <string>
#include <cstring>
#include <thread>
#include <random>
#include <vector>
#include <filesystem>
#include <cstdlib> 
//Downloaded from https://www.sfml-dev.org/files/SFML-2.6.0-windows-gcc-13.1.0-mingw-64-bit.zip. for mingw (w64, over mysys)
//copied files according to https://www.youtube.com/watch?v=rZE700aaT5I
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "tilesMakros.hpp"


#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#endif //STDLIBS_HPP
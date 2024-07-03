#include "utils.hpp"


bool fileExists(const std::string &filename){
    std::ifstream file(filename);
    bool works = file.good();
    file.close();
    return works;
}



void readStrOfFile(std::ifstream &file, std::string &str){
    //Get Size of String
    size_t size;
    file.read(reinterpret_cast<char *>(&size), sizeof(size));
    //Read string
    str.resize(size);
    file.read(reinterpret_cast<char *>(&str[0]), size);
}



void writeStrToFile(std::ofstream &file, std::string &str){
    size_t size = str.size();
    //Write size of string
    file.write(reinterpret_cast<char *>(&size), sizeof(size));
    //Write string
    file.write(reinterpret_cast<char*>(str.data()), size);
}


bool validIP(const std::string &ip){
    std::regex ipPattern("^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$");
    return std::regex_match(ip, ipPattern);
}
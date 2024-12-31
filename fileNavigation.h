#ifndef FILENAVIGATION_H
#define FILENAVIGATION_H
#include <string>
#include <filesystem>
#include <iostream>
#include<unordered_map>
namespace fs=std::filesystem;




class fileNavigation{
public:
    static void cd(std::string action, fs::path& currentDir);
    static void ls(fs::path& currentDir, std::unordered_map<std::string,std::string>& inventoryRev, std::unordered_map<std::string,std::string>& inventoryRevAlbums);



};

#endif
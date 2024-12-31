#ifndef FOLDEROPERATIONS_H
#define FOLDEROPERATIONS_H

#include <string>
#include <filesystem>
namespace fs = std::filesystem;


class folderOperations{
public:
    static void addFolder(std::string folderName, fs::path currentDir);
    static void deleteFolder(std::string folderName, fs::path currentDir);
    static void moveFolder(std::string folderName, std::string location, fs::path currentDir);
};

#endif

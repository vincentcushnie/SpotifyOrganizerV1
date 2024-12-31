
#include "folderOperations.h"
#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;



void folderOperations::addFolder(std::string folderName, fs::path currentDir){
    fs::path folderPath = currentDir / folderName;
    if(!(fs::exists(folderPath))){
         if (fs::create_directory(folderPath)) {
            std::cout << "Folder created successfully.\n";
        } else {
            std::cerr << "Failed to create folder.\n";
        }
    }
    else{
        std::cout<<"Folder of the same name already exists"<<std::endl;
    }
}
void folderOperations::deleteFolder(std::string folderName, fs::path currentDir){
    fs::path folderPath = currentDir / folderName;
    if (fs::exists(folderPath) && fs::is_directory(folderPath)) {
            fs::remove_all(folderPath);
    }
    else{
        std::cout<<"that directory either does not exist or cannot be deleted"<<std::endl;
    }
}
void folderOperations::moveFolder(std::string folderName, std::string location, fs::path currentDir){
    try{
    fs::path folderPath = currentDir / folderName;
    fs::path moveLocation = fs::path(location) / folderName;
     if (fs::exists(moveLocation)) {
        fs::remove_all(moveLocation); 
    }
    std::cout<<"before copy"<<std::endl;
    fs::copy(folderPath, moveLocation, fs::copy_options::recursive);
    std::cout<<"after copy"<<std::endl;
    fs::remove_all(folderPath);
    } catch (const std::exception& e) {
        std::cout << "Operation was unsuccessful (Error: " << e.what()<<")" << std::endl;
    }

}

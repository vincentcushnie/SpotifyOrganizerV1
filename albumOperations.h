#ifndef ALBUMOPERATIONS_H
#define ALBUMOPERATIONS_H
#include <fstream>
#include <filesystem>
#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>
namespace fs=std::filesystem;
using json = nlohmann::json;
namespace fs = std::filesystem;


class albumOperations{
public:
    static void moveAlbum(std::string albumName, std::string location, fs::path currentDir);
    static void deleteAlbum(std::string accessToken, std::string playlistName, std::unordered_map<std::string, std::string>& inventory, std::unordered_map<std::string, std::string>& inventoryRev, fs::path currentDir);
    static void importAlbums(std::string accessToken, std::string userId);
    static void createImportedAlbumFile(std::string accessToken, std::string albumId, std::string albumName);
};

#endif
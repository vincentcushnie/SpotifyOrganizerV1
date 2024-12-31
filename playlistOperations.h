#ifndef PLAYLISTOPERATIONS_H
#define PLAYLISTOPERATIONS_H
#include <fstream>
#include <filesystem>

#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>
namespace fs=std::filesystem;
using json = nlohmann::json;
namespace fs = std::filesystem;


class playlistOperations{
public:
    // static void addPlaylist(std::string accessToken);
    static void deletePlaylist(std::string accessToken, std::string playlistName, std::unordered_map<std::string, std::string>& inventory, std::unordered_map<std::string, std::string>& inventoryRev, fs::path currentDir);
    static void movePlaylist(std::string playlistName, std::string location, fs::path currentDir);
    static void importPlaylists(std::string accessToken, std::string userId);
    static void createImportedPlaylistFile(std::string accessToken, std::string playlistId, std::string playlistName);
};

#endif

#include "playlistOperations.h"
#include "curls_utils.h"
#include <string>
#include <iostream>
#include <filesystem>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;    

namespace fs = std::filesystem;

void playlistOperations::movePlaylist(std::string playlistName, std::string location, fs::path currentDir){
    try{
    fs::path playlistPath = currentDir / playlistName;
    fs::path moveLocation = fs::path(location) / playlistName;
     if (fs::exists(moveLocation)) {
        fs::remove_all(moveLocation); 
    }
    fs::copy(playlistPath, moveLocation, fs::copy_options::recursive);
    fs::remove_all(playlistPath);
    } catch (const std::exception& e) {
        std::cout << "Operation was unsuccessful (Error: " << e.what()<<")" << std::endl;
    }
}


void playlistOperations::importPlaylists(std::string accessToken, std::string userId){
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    std::string url = "https://api.spotify.com/v1/users/"+ userId+"/playlists";
    

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        std::string headerString = "Authorization: Bearer "+accessToken;
        const char* headerCString= headerString.c_str();
        // Set the Content-Type header
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, headerCString);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the write callback function to capture the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curls_utils::WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the POST request
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Print the response (should contain the access token)
        }

        // Clean up
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    // Global cleanup
    curl_global_cleanup();
    json userPlaylists = json::parse(readBuffer);
    std::ofstream file("inventory.txt");  // Create or open the file in the current directory

    json playlistsArray = userPlaylists["items"];
    for(const auto& item: playlistsArray){
        playlistOperations::createImportedPlaylistFile(accessToken, item["id"], item["name"]);
        if (file.is_open()) {
            file << item["name"] <<":" <<item["id"] << "\n";
        } else {
        std::cerr << "Unable to open file\n";
        }
    }
    file.close();
}

void playlistOperations::createImportedPlaylistFile(std::string accessToken, std::string playlistId, std::string playlistName){
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    std::string url = "https://api.spotify.com/v1/playlists/"+ playlistId;
    

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        std::string headerString = "Authorization: Bearer "+accessToken;
        const char* headerCString= headerString.c_str();
        // Set the Content-Type header
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, headerCString);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the write callback function to capture the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curls_utils::WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the POST request
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Print the response (should contain the access token)
        }

        // Clean up
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

  
    // Global cleanup
    curl_global_cleanup();
    json playlist = json::parse(readBuffer);
    std::ofstream file("musicData/playlists/"+playlistId);  // Create or open the file in the current directory
    file<<playlistName<<"\n\n";
    if (file.is_open()) {
        for (const auto& item : playlist["tracks"]["items"]) {
            file << "Track Name: " << item["track"]["name"] << "\n";
        }
        file.close();  // Close the file when done
    } else {
        std::cerr << "Unable to open file\n";
    }
    
     
    
}

void playlistOperations::deletePlaylist(std::string accessToken, std::string playlistName, std::unordered_map<std::string, std::string>& inventory, std::unordered_map<std::string, std::string>& inventoryRev, fs::path currentDir){
     if(inventory.find(playlistName)==inventory.end()){
        std::cout<<"that playlist does not exist"<<std::endl;
        return;
    }
    fs::path filePath = currentDir / inventory[playlistName];

    if(!fs::exists(filePath)){
        std::cout<<"that playlist does not exist in this directory"<<std::endl;
        return;
    }

    std::string playlistId = inventory[playlistName];
   
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init(); // Initialize CURL
    if (curl) {
        const std::string url = "https://api.spotify.com/v1/playlists/"+playlistId+"/followers"; // Replace with your API URL

        // Set the URL for the DELETE request
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Specify DELETE method
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

        // Optional: Set headers (e.g., for authentication)
        struct curl_slist* headers = nullptr;
        std::string header = "Authorization: Bearer "+accessToken;
        const char* header_cstr=header.c_str();
        headers = curl_slist_append(headers, header_cstr); // Optional
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Perform the DELETE request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "DELETE request sent successfully!" << std::endl;
        }

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Failed to initialize CURL" << std::endl;
    }
    if (fs::exists(filePath)) {
            fs::remove_all(filePath);
    }
    else{
        std::cout<<"that directory either does not exist or cannot be deleted"<<std::endl;
    }
    inventory.erase(playlistName);
    inventoryRev.erase(playlistId);

}
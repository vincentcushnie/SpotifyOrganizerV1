#include "folderOperations.h"
#include "curls_utils.h"
#include "playlistOperations.h"
#include "albumOperations.h"
#include "shutdown.h"
#include "fileNavigation.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <curl/curl.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <microhttpd.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include "startup.h"
#include "utility.h"
#include <thread>
namespace fs = std::filesystem;
using json = nlohmann::json;
#define PORT 8888

//error handling for the callback
//error throwing when authentication doesn't work or when state checking doesn't match
//remove redundancy of move (folder vs playlist)
//choose where to put the imported playlists


std::unordered_map<std::string,std::string> inventory;
std::unordered_map<std::string,std::string> inventoryRev;
std::unordered_map<std::string,std::string> inventoryAlbums;
std::unordered_map<std::string,std::string> inventoryRevAlbums;
static json config;
static std::string accessToken;
static std::string authorizationToken;
static std::string userId;
static int startTime;
static json userInformation;


enum Command {
    mkfldr,
    dlfldr,
    mvfldr,
    mkpylt,
    dlpylt,
    mvpylt,
    impylt,
    adalbm,
    search,
    none  
};


int main(){

    
    config = startup::getStartupStatus();
    int run_count=config["run_count"];
    if(config["first_time"]==true){
        authorizationToken = startup::getAuthorizationToken();
        json accessTokenReturn = curls_utils::GetAccessToken(authorizationToken);
        accessToken=accessTokenReturn["access_token"];
        config["refreshToken"]=accessTokenReturn["refresh_token"];
        config["first_time"]=false;
        
        config["run_count"]=run_count+1;
        userInformation=curls_utils::GetCurrentUserInfo(accessToken);
        userId = userInformation["id"];
        playlistOperations::importPlaylists(accessToken, userId);
        albumOperations::importAlbums(accessToken, userId);
    }else{
        json accessTokenReturn = curls_utils::GetAccessTokenRefresh(config["refreshToken"]);
        accessToken=accessTokenReturn["access_token"];
        config["run_count"]=run_count+1;
        userInformation=curls_utils::GetCurrentUserInfo(accessToken);
        userId = userInformation["id"];
    }

    startTime=time(0);
    
    
    
    
    
    //initializing two-way inventory tracker which helps with syncing local and spotify playlists
    startup::inventoryInitialization(inventory, true, "inventory.txt");
    startup::inventoryInitialization(inventoryRev, false, "inventory.txt");
    startup::inventoryInitialization(inventoryAlbums, true, "inventoryAlbums.txt");
    startup::inventoryInitialization(inventoryRevAlbums, false, "inventoryAlbums.txt");

    for(const auto& entry: inventoryRevAlbums){
        std::cout<<entry.first<<":"<<entry.second<<std::endl;
    }

    if(inventory.size()!=inventoryRev.size()){
        std::cout<<"inventories are unequal!!!"<<std::endl;
        return 1;
    }
    std::string action;
    std::string moveLocation;
    Command command=none;
    fs::path currentDir = "musicData";

    std::cout<<"this is your current location: "+ currentDir.string() +". Please enter your desired action. Enter help to see list of commands. Standard file navication commands work"<<std::endl;
    while(true){

        command=none;
        std::string inputName;
        std::cout << "> ";
        std::cin>>std::ws;
        std::getline(std::cin, action);
        if(action=="q"){
            break;
        }
        else if(action.find("cd ")==0){
            fileNavigation::cd(action, currentDir);
            command=none;
        }
        else if(action=="ls"){
            fileNavigation::ls(currentDir, inventoryRev, inventoryRevAlbums);
            command=none;
        }
        else if(action=="mkfldr"){
            command =mkfldr;
        }
        else if(action =="dlfldr"){
            command = dlfldr;
        }
        else if(action == "mvfldr"){
            command = mvfldr;
        }
        else if(action == "mvpylt"){
            command = mvpylt;
        }
        else if(action=="help"){
            std::cout<<"mkfldr,dlfldr,mvfldr,mkpylt,dlpylt,mvpylt,adpylt,adalbm,none "<<std::endl;
            command=none;
        }
        else if(action=="search"){
            command=search;
        }
        else if(action=="dlpylt"){
            command= dlpylt;
        }

        if(difftime(time(0), startTime) >= 3600){
            json accessTokenReturn = curls_utils::GetAccessTokenRefresh(config["refreshToken"]);
            accessToken=accessTokenReturn["access_token"];
            startTime=time(0);
        }
        switch (command){
            case mkfldr:
                std::cout<<"what would you like the new folder to be called:  ";
                std::cin>>inputName;
                folderOperations::addFolder(inputName, currentDir);
                break;
            case dlfldr:
                std::cout<<"what is the name of the folder to be deleted:  ";
                std::cin>>inputName;
                folderOperations::deleteFolder(inputName, currentDir);
                break;
            case mvfldr:
                std::cout<<"what is the name of the folder to be moved:  ";
                getline(std::cin, inputName);
                std::cout<<"what is the path (starting from musicData) of the move location"<<std::endl;
                getline(std::cin, moveLocation);
                folderOperations::moveFolder(inputName, moveLocation, currentDir);
                break;
            case mvpylt:
                std::cout<<"what is the name of the playlist to be moved:  ";
                getline(std::cin, inputName);
                std::cout<<"what is the path (starting from musicData) of the move location"<<std::endl;
                getline(std::cin, moveLocation);
                if(inventory.find(inputName)==inventory.end()){
                    std::cout<<"no such playlist."<<std::endl;
                    break;
                }
                folderOperations::moveFolder(inventory[inputName], moveLocation, currentDir);
                break;
            case dlpylt:
                std::cout<<"what is the name of the playlist to be deleted:  ";
                std::cin>>inputName;
                playlistOperations::deletePlaylist(accessToken, inputName, inventory, inventoryRev, currentDir);
                break;
            case none:
                break;
            case search:
                utility::searchQuery(accessToken, 0);
                break;
            default:
                std::cout<<"no valid option received.";
                break;
        }
        std::cout<<"this is your current location: "+ currentDir.string() +". Please enter your desired action."<<std::endl;
    }

    std::ofstream file13("config.json");

    if (file13.is_open()) {
        file13 << config;

    } else {
        std::cerr << "Could not open the config.json file!" << std::endl;
    }

    file13.close();
    shutdown::rewriteInventory(inventory);

    return 0;



}

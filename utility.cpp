#include "utility.h"

json utility::searchQuery(std::string accessToken, int offset){
    std::string searchingFor="";
    std::string queryFilters="";
    std::vector<std::string> queryFilterVector; 
    std::cout<<"what are you searching for: ";
    getline(std::cin, searchingFor);
    std::cout<<"enter any of the following filters you want seperated by commas (options are album, artist, track, year, upc, tag:new, tag:hipster, isrc, and genre)"<<std::endl;
    getline(std::cin, queryFilters);
    std::stringstream ss(queryFilters);
    std::string token;
    
     while (std::getline(ss, token, ',')) {
        queryFilterVector.push_back(token);
    }
    searchingFor.append(" ");
    for (const auto& str : queryFilterVector) {
        std::string temp;
        std::cout<<"enter value for "<<str<<std::endl;
        getline(std::cin, temp);
        searchingFor.append(str+":"+temp+" ");
    }
    searchingFor.pop_back();
    queryFilterVector.clear();


    std::stringstream aa(searchingFor);

    if(searchingFor.find(" ")!=std::string::npos){
        while (std::getline(aa, token, ' ')) {
            queryFilterVector.push_back(token);
        }
    }
    searchingFor="";
    for(const auto& str : queryFilterVector){
        searchingFor.append(str+"%2520");
    }
    searchingFor.pop_back();
    searchingFor.pop_back();
    searchingFor.pop_back();
    searchingFor.pop_back();
    searchingFor.pop_back();
    queryFilterVector.clear();
    std::stringstream bb(searchingFor);
    if(searchingFor.find(":")!=std::string::npos){
        while (std::getline(bb, token, ':')) {
            queryFilterVector.push_back(token);
        }
    }
    searchingFor="";
    for(const auto& str : queryFilterVector){
        searchingFor.append(str+"%3A");
    }
    searchingFor.pop_back();
    searchingFor.pop_back();
    searchingFor.pop_back();
    queryFilterVector.clear();
    std::string types;
    std::cout<<"What categories would you like to search. Enter as a comma seperated list with no spaces. (Options: album, artist, playlist, track, show, episode, audiobook) All for every category."<<std::endl;
    getline(std::cin, types);
    int limit = 20;
    int currentOffset=offset;


    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    
    std::string url; 
    if(types=="All"){
        url= "https://api.spotify.com/v1/search?q="+searchingFor+"&type=album,artist,playlist,track,show,episode,audiobook&limit=20&offset="+std::to_string(currentOffset);
    }
    else{
        url= "https://api.spotify.com/v1/search?q="+searchingFor+"&type="+types+"&limit=20&offset="+std::to_string(currentOffset);
    }
    
    

    // Initialize libcurl
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
        }

        // Clean up
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    std::cout<<"made it here!!!"<<std::endl;
    json result = json::parse(readBuffer);

    if(types.find("track")!=std::string::npos){
        int i=1+currentOffset;
        std::cout<<">Tracks results: "<<std::endl;
        for (const auto& item : result["tracks"]["items"]) {
            std::cout << ">>>>>>>("<<i<<") " << item["name"] << " from "<<item["album"]["name"]<<std::endl;;
            ++i;
        }

    }
    if(types.find("album")!=std::string::npos){
        int i=1+currentOffset;
        std::cout<<">Album results: "<<std::endl;
        for (const auto& item : result["albums"]["items"]) {
            std::cout << ">>>>>>>("<<i<<") " << item["name"]<<std::endl;;
            ++i;
        }

    }

    
    // Global cleanup
    curl_global_cleanup();


    return result;








    



}
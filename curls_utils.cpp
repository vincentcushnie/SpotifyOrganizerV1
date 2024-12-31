#include "curls_utils.h"
#include <curl/curl.h>
#include<nlohmann/json.hpp>
#include<iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string curls_utils::base64_encode(const std::string &in) {
    std::string out;
    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}


size_t curls_utils::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}



json curls_utils::GetCurrentUserInfo(std::string accessToken){
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    
    std::string url = "https://api.spotify.com/v1/me";

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

    // Global cleanup
    curl_global_cleanup();
    return json::parse(readBuffer);
}

json curls_utils::GetAccessToken(std::string authCode, std::string clientId, std::string clientSecret) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    std::string url = "https://accounts.spotify.com/api/token";
    std::string postData = "code=" + authCode +
                           "&redirect_uri=http://localhost:8888/callback" +
                           "&grant_type=authorization_code";

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set URL and post data
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

        // SSL verification (keep enabled if possible)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

        // Set headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        headers = curl_slist_append(headers, ("Authorization: Basic " + curls_utils::base64_encode(clientId+":"+clientSecret)).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set write callback
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curls_utils::WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();
    
    return json::parse(readBuffer);
}

json curls_utils::GetAccessTokenRefresh(std::string refreshToken, std::string clientId, std::string clientSecret){
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    std::string url = "https://accounts.spotify.com/api/token";
    std::string postData = "grant_type=refresh_token"
                           "&refresh_token="+refreshToken;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set URL and post data
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

        // SSL verification (keep enabled if possible)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

        // Set headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        headers = curl_slist_append(headers, ("Authorization: Basic " + curls_utils::base64_encode(clientId+":"+clientSecret)).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set write callback
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curls_utils::WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();
    
    return json::parse(readBuffer);
}
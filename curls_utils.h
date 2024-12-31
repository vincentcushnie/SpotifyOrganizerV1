#ifndef CURL_UTILS_H
#define CURL_UTILS_H

#include<nlohmann/json.hpp>


#include <string>  // Required for std::string

using json =nlohmann::json;

// Declare the WriteCallback function
class curls_utils{
public:
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
static json GetCurrentUserInfo(std::string accessToken);
static json GetAccessToken(std::string authCode, std::string clientId="clientId", std::string clientSecret="clientSecret");
static json GetAccessTokenRefresh(std::string refreshToken, std::string clientId="clientId", std::string clientSecret="clientSecret");
static std::string base64_encode(const std::string &in);



};
#endif 
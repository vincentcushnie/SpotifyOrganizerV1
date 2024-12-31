#ifndef UTILITY_H
#define UTILITY_H

#include "curls_utils.h"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <curl/curl.h>
#include<nlohmann/json.hpp>
using json = nlohmann::json;



class utility{
public:
    static json searchQuery(std::string accessToken, int offset);
    
};
#endif
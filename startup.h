#ifndef STARTUP_H
#define STARTUP_H
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <microhttpd.h>
#include <iostream>

#define PORT 8888


using json=nlohmann::json;



class startup{
public:
    static json getStartupStatus();
    static MHD_Result handle_connection(void *cls,
                                  struct MHD_Connection *connection,
                                  const char *url,
                                  const char *method,
                                  const char *version,
                                  const char *upload_data,
                                  size_t *upload_data_size,
                                  void **con_cls);
    static char* received_code; 
    static char* state; 
    static std::string getAuthorizationToken(std::string clientID="clientId", std::string clientSecret="clientSecret", std::string redirect_uri="http://localhost:8888/callback", std::string scopes="user-read-private user-read-email playlist-modify-private playlist-modify-public user-library-read", std::string stateVal = "funk");
    static void inventoryInitialization(std::unordered_map<std::string,std::string>& inventory, bool forward, std::string fileName);


};

#endif
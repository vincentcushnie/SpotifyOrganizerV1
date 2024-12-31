#include "startup.h"

json startup::getStartupStatus(){
    std::string filename = "config.json";
    nlohmann::json config;

    // Try to load the configuration file
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> config;
        file.close();
    } else {
        // If file doesn't exist, initialize default values
        config["run_count"] = 0;
        config["first_time"] = true;
    }
    return config;
}

char* startup::received_code = NULL;
char* startup::state = NULL;  


MHD_Result startup::handle_connection(void *cls,
                                  struct MHD_Connection *connection,
                                  const char *url,
                                  const char *method,
                                  const char *version,
                                  const char *upload_data,
                                  size_t *upload_data_size,
                                  void **con_cls) {
    
    
    
    const char *response_text;
    struct MHD_Response *response;

    const char* query_params = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "code");
    const char* state_value = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "state");
    if (query_params != NULL) {
        if (received_code) {
            free(received_code);
        }
        if (state){
            free(state);
        }
        
        size_t code_length = strlen(query_params);
        received_code = (char*)malloc(code_length + 1);
        strcpy(received_code, query_params);
        size_t state_length = strlen(state_value);
        state = (char*)malloc(state_length + 1);
        strcpy(state, state_value);
        
        // HTML response with auto-close JavaScript
        response_text = "<html><body>"
                       "<h2>Authorization Successful!</h2>"
                       "<p>This window will close automatically.</p>"
                       "<script>"
                       "setTimeout(function() { window.close(); }, 100000);"
                       "</script>"
                       "</body></html>";
        
    } else {
        response_text = "Waiting for authorization...";
    }

    response = MHD_create_response_from_buffer(strlen(response_text),
                                             (void*)response_text,
                                             MHD_RESPMEM_PERSISTENT);
    
    // Set content type to HTML
    MHD_add_response_header(response, "Content-Type", "text/html");
    
    MHD_Result ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    
    return ret;
}

std::string startup::getAuthorizationToken(std::string clientID, std::string clientSecret, std::string redirect_uri, std::string scopes, std::string stateVal){
    struct MHD_Daemon *daemon;
    daemon = MHD_start_daemon (MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL, &handle_connection, NULL, MHD_OPTION_END);
    if (NULL == daemon){
        return "Daemon Null";
    }

    
    std::string auth;
    std::cout<<"Hello welcome to spotify organizer"<<std::endl;
    std::cout<<"To begin we need to handle Spotify API Authentication: "<<std::endl;
    std::string entryCommand = "wslview \"https://accounts.spotify.com/authorize?response_type=code&client_id="+clientID+"&scope="+scopes+"&redirect_uri="+redirect_uri+"&state="+stateVal+"\"";
    std::system(entryCommand.c_str());
    while (received_code == NULL) {
        sleep(1);
    }    
    if(state!=stateVal){
        std::cout<<"state (value from callback): "<<state<<std::endl;
        std::cout<<"stateVal (value passed to function): "<<stateVal<<std::endl;
        std::cout<<"Authorization state mismatch"<<std::endl;
    }
    auth=received_code;

    
    MHD_stop_daemon (daemon);
    return auth;
}

void startup::inventoryInitialization(std::unordered_map<std::string,std::string>& inventory, bool forward, std::string fileName){
    std::ifstream inputFile(fileName); // Open the file
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
    }

    std::string line;
    while (std::getline(inputFile, line)) { // Read the file line by line
        size_t delimiterPos = line.find(':'); // Find the position of the ':'
        if (delimiterPos != std::string::npos) { // Ensure ':' exists in the line
            std::string key = line.substr(0, delimiterPos); // Extract the key (before ':')
            std::string value = line.substr(delimiterPos + 1); // Extract the value (after ':')
            if (!key.empty() && key.front() == '"' && key.back() == '"') {
            key = key.substr(1, key.size() - 2);
            }
            if (!value.empty() && value.front() == '"' && value.back() == '"') {
            value = value.substr(1, value.size() - 2);
            }
            if(forward==true){
                inventory[key] = value;
            }
            else{
                inventory[value]=key;
            }
        }
    }
}

#include "shutdown.h"

void shutdown::rewriteInventory(const std::unordered_map<std::string, std::string>& inventory){
    std::ofstream file("inventory.txt");
    if (file.is_open()) {
        for (const auto& pair : inventory) {
            file << pair.first <<":" <<pair.second << "\n";
        }    
    } else {
        std::cerr << "Unable to open file\n";
    }
    file.close();
}
